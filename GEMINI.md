# Project Context and Helpful Hints

## Project Structure
- **root**: `/google/src/cloud/jwhpryor/vogo_glass_new/google3/third_party/jni_wrapper`
- **implementation**: Core C++ implementation of the JNI wrapper.
    - `local_object.h`, `global_object.h`: RAII wrappers for JNI objects.
    - `local_exception.h`, `global_exception.h`: Wrappers for Java exceptions.
    - `jni_helper/`: Helper utilities for JNI interaction.
- **javatests**: Java and JNI integration tests.
    - `com/jnibind/test/`: General tests.
    - `com/jnibind/android/`: Android-specific instrumentation tests.
- **metaprogramming**: Template metaprogramming utilities used by the library.
- **class_defs**: Pre-defined JNI class definitions for common Java classes.

## Helpful Hints

### Running Tests
To run all tests from the root directory:
```bash
blaze test --tool_tag=codemindcli-extension --noshow_progress //third_party/jni_wrapper/...
```

### debugging Test Failures
If a test fails, use `blaze run` on the failing target to see detailed output and potential fix instructions. For example, if `readme_up_to_date_test` fails:
```bash
blaze run --tool_tag=codemindcli-extension --noshow_progress //third_party/jni_wrapper:readme_up_to_date_test
```
This specific test will often provide the exact commands needed to regenerate the `README.md` files if they are out of date.

### Regenerating READMEs
If you modify `README.md`, you likely need to regenerate the expected output files to pass `readme_up_to_date_test`. The commands (from the test failure output) are typically:

```bash
blaze build //third_party/jni_wrapper:gen_readme && cp ../../../blaze-genfiles/third_party/jni_wrapper/README.md.expected README.md
blaze build //third_party/jni_wrapper:gen_readme_public && cp ../../../blaze-genfiles/third_party/jni_wrapper/README.md.public.expected README.md.public
```
*Note: Adjust the relative path to `blaze-genfiles` based on your current working directory.*

### Avoiding Circular Dependencies
When adding new types (like `LocalException` or `GlobalException`), always add them to `implementation/forward_declarations.h`. This helps break circular dependencies and reduces header inclusion bloat, which can lead to `clang-tidy` errors and compilation failures. Clients can include `forward_declarations.h` when they only need pointers or references to these types.

### `IWYU pragma: private, include`
When `clang-tidy` or `IWYU` complains about headers that are meant to be internal, such as `implementation/local_exception.h` or `implementation/global_exception.h`, ensure that these headers include `// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"`. This pragma indicates that these headers should only be included through `jni_bind.h` and not directly by external consumers. If you need to use the types declared in these internal headers without including `jni_bind.h` (e.g., to break circular dependencies or avoid bloat in other internal headers), you should add forward declarations to `implementation/forward_declarations.h` instead.

### Open Source Appropriateness
Since this project is shared as open source, all code, documentation, and guidance stored in files like `GEMINI.md` must be appropriate for both internal Google use and external consumption. Avoid including any internal-only links, tool names, or instructions that would not be relevant or accessible to external users. Ensure that all contributions align with open-source best practices and licensing requirements.
