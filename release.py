#!/usr/bin/env python3
import argparse
import os
import re
import subprocess
import sys


def run_command(command, shell=False):
  print(
      f"Running: {' '.join(command) if isinstance(command, list) else command}"
  )
  try:
    subprocess.check_call(command, shell=shell)
  except subprocess.CalledProcessError as e:
    print(f"Error running command: {e}")
    sys.exit(1)


def increment_version(version, release_type):
  parts = [int(x) for x in version.split(".")]
  if release_type == "major":
    parts[0] += 1
    parts[1] = 0
    parts[2] = 0
  elif release_type == "minor":
    parts[1] += 1
    parts[2] = 0
  elif release_type == "dot":
    parts[2] += 1
  return ".".join(map(str, parts))


def main():
  # Change to workspace directory if running via bazel run
  if "BUILD_WORKSPACE_DIRECTORY" in os.environ:
    os.chdir(os.environ["BUILD_WORKSPACE_DIRECTORY"])
    os.chdir("third_party/jni_wrapper")
  else:
    # Fallback to script dir if running manually
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

  version_file = "JNI_BIND_VERSION.inc"
  with open(version_file, "r") as f:
    current_version = f.read().strip()

  parser = argparse.ArgumentParser(description="Cut a new release for JNI Bind")
  parser.add_argument(
      "--release-type",
      choices=["major", "minor", "dot"],
      help="Type of release",
  )

  if len(sys.argv) == 1:
    print("JNI Bind Release Tool")
    print(f"Current version: {current_version}")
    print("\nUsage: release.py --release-type {major,minor,dot}")
    sys.exit(0)

  args = parser.parse_args()

  if not args.release_type:
    parser.error("the following arguments are required: --release-type")

  # 0. Check jj status
  try:
    output = (
        subprocess.check_output(["jj", "log", "-r", "@", "-T", "(empty)"])
        .decode("utf-8")
        .strip()
    )
    if "true" not in output:
      print("Error: The current jj commit is not empty.")
      subprocess.run(["jj", "status"])
      sys.exit(1)
  except subprocess.CalledProcessError as e:
    print(f"Error checking jj status: {e}")
    sys.exit(1)

  # 1. Update Version
  new_version = increment_version(current_version, args.release_type)
  print(f"Bumping version from {current_version} to {new_version}")

  with open(version_file, "w") as f:
    f.write(new_version)

  # Update README_inc.md URL *before* Godbolt script (so it propagates)
  readme_inc_path = "README_inc.md"
  with open(readme_inc_path, "r") as f:
    content = f.read()

  # Replace Release-X.Y.Z with Release-NEW_VERSION
  # Also handle the strip_prefix line: strip_prefix = "jni-bind-Release-1.2.3",
  pattern_url = r"(Release-)[0-9.]+(\.zip)"
  pattern_prefix = r"(jni-bind-Release-)[0-9.]+"

  new_content = re.sub(pattern_url, rf"\g<1>{new_version}\g<2>", content)
  new_content = re.sub(pattern_prefix, rf"\g<1>{new_version}", new_content)

  if content != new_content:
    with open(readme_inc_path, "w") as f:
      f.write(new_content)
    print(f"Updated version in {readme_inc_path}")
  else:
    print(f"Warning: No version string found to update in {readme_inc_path}")

  # 2. Regenerate jni_bind_release.h
  print("Regenerating jni_bind_release.h...")
  run_command(
      ["blaze", "build", "//third_party/jni_wrapper:gen_jni_bind_release"]
  )

  # Copy generated file
  try:
    genfiles_root = (
        subprocess.check_output(["blaze", "info", "blaze-genfiles"])
        .decode("utf-8")
        .strip()
    )
    gen_file_path = os.path.join(
        genfiles_root, "third_party/jni_wrapper/jni_bind_release.h"
    )
    if os.path.exists(gen_file_path):
      run_command(["cp", gen_file_path, "jni_bind_release.h"])
      run_command(["chmod", "+w", "jni_bind_release.h"])
      print("jni_bind_release.h updated.")
    else:
      print(f"Error: Generated file not found at {gen_file_path}")
      sys.exit(1)
  except Exception as e:
    print(f"Error copying generated file: {e}")
    sys.exit(1)

  # 3. Run godbolt upload (this regenerates README.md from README_inc.md)
  print("Running godbolt update script...")
  run_command(
      ["blaze", "run", "//third_party/jni_wrapper/godbolt:update_godbolt"]
  )

  # 4. Run tests
  print("Running tests...")
  # Using -c opt as requested
  run_command(["blaze", "test", "-c", "opt", "//third_party/jni_wrapper/..."])

  # 5. Describe the commit
  description = f"""BEGIN_PUBLIC

Rev JNI Bind release header to v {new_version}

END_PUBLIC

BUG=143908983
TESTED=TAP."""
  run_command(["jj", "describe", "-m", description])

  print("Release cut successfully!")


if __name__ == "__main__":
  main()
