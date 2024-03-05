/*
 * Copyright 2024 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JNI_BIND_METAPROGRAMMING_COLOUR_H_
#define JNI_BIND_METAPROGRAMMING_COLOUR_H_

#include <string>
#include <string_view>

namespace jni::metaprogramming {

struct Color {
  static constexpr std::string_view kNone{"\033[0m"};

  // --- Text Colors ---
  static constexpr std::string_view kBlack = "\033[0;30m";
  static constexpr std::string_view kBlackBold = "\033[1;30m";
  static constexpr std::string_view kRed = "\033[0;31m";
  static constexpr std::string_view kRedBold = "\033[1;31m";
  static constexpr std::string_view kGreen = "\033[0;32m";
  static constexpr std::string_view kGreenBold = "\033[1;32m";
  static constexpr std::string_view kYellow = "\033[0;33m";
  static constexpr std::string_view kYellowBold = "\033[1;33m";
  static constexpr std::string_view kBlue = "\033[0;34m";
  static constexpr std::string_view kBlueBold = "\033[1;34m";
  static constexpr std::string_view kPurple = "\033[0;35m";
  static constexpr std::string_view kPurpleBold = "\033[1;35m";
  static constexpr std::string_view kCyan = "\033[0;36m";
  static constexpr std::string_view kCyanBold = "\033[1;36m";
  static constexpr std::string_view kWhite = "\033[0;37m";
  static constexpr std::string_view kWhiteBold = "\033[1;37m";

  // --- Background Colors ---
  static constexpr std::string_view kBgBlack = "\033[40m";
  static constexpr std::string_view kBgBlackBold = "\033[100m";
  static constexpr std::string_view kBgRed = "\033[41m";
  static constexpr std::string_view kBgRedBold = "\033[101m";
  static constexpr std::string_view kBgGreen = "\033[42m";
  static constexpr std::string_view kBgGreenBold = "\033[102m";
  static constexpr std::string_view kBgYellow = "\033[43m";
  static constexpr std::string_view kBgYellowBold = "\033[103m";
  static constexpr std::string_view kBgBlue = "\033[44m";
  static constexpr std::string_view kBgBlueBold = "\033[104m";
  static constexpr std::string_view kBgPurple = "\033[45m";
  static constexpr std::string_view kBgPurpleBold = "\033[105m";
  static constexpr std::string_view kBgCyan = "\033[46m";
  static constexpr std::string_view kBgCyanBold = "\033[106m";
  static constexpr std::string_view kBgWhite = "\033[47m";
  static constexpr std::string_view kBgWhiteBold = "\033[107m";
};

// Colorize function for runtime manipulation of text. This is suitable when
// the string is unknowable until runtime.
inline std::string Colorize(std::string_view colour, std::string_view str,
                            bool colourize = true) {
  return !colourize ? std::string{str}
                    : std::string{colour} + std::string{str} +
                          std::string{Color::kNone};
}

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_COLOUR_H_
