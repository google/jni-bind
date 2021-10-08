#ifndef JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_
#define JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_

#include <array>
#include <string_view>

namespace jni::metaprogramming {

struct StringConcatenate {
  template <std::string_view const&... Vs>
  struct Helper {
    static constexpr auto BuildConcatenation() noexcept {
      constexpr std::size_t len = (Vs.size() + ... + 0);
      std::array<char, len + 1> arr{};
      auto append_single_string =
          [i = 0, &arr](auto const& string_to_concatenate) mutable {
            for (auto c : string_to_concatenate) arr[i++] = c;
          };
      (append_single_string(Vs), ...);
      arr[len] = 0;

      return arr;
    }

    static constexpr auto arr = BuildConcatenation();
    static constexpr std::string_view value{arr.data(), arr.size() - 1};
  };

  template <std::string_view const&... Vs>
  static constexpr std::string_view value = Helper<Vs...>::value;
};

template <std::string_view const&... Vs>
static constexpr auto StringConcatenate_v = StringConcatenate::value<Vs...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_
