#include "metaprogramming/string_concatenate.h"

#include <string_view>

namespace {

using jni::metaprogramming::StringConcatenate_v;

// Empty list string concatenation.
static constexpr auto kVoidSetResultsInVoid = StringConcatenate_v<>;

static_assert(kVoidSetResultsInVoid.empty());

// Simple String concatenation.
static constexpr std::string_view kEmptyString = "";

static constexpr std::string_view kThe = "The ";
static constexpr std::string_view kQuick = "quick ";
static constexpr std::string_view kBrown = "brown ";
static constexpr std::string_view kFox = "fox ";

static constexpr auto kTheQuickBrownFox =
    StringConcatenate_v<kThe, kQuick, kBrown, kFox>;
static_assert(kTheQuickBrownFox == "The quick brown fox ");

static_assert(StringConcatenate_v<kEmptyString, kTheQuickBrownFox> ==
              "The quick brown fox ");
static_assert(
    StringConcatenate_v<kEmptyString, kEmptyString, kTheQuickBrownFox> ==
    "The quick brown fox ");

}  // namespace
