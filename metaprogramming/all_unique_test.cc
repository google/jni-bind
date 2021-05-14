// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "all_unique.h"

#include <utility>

namespace {

using jni::metaprogramming::AllUnique_v;
using jni::metaprogramming::AllUniqueValues;

static_assert(AllUnique_v<>);
static_assert(AllUnique_v<void>);
static_assert(AllUnique_v<int>);
static_assert(AllUnique_v<float>);
static_assert(AllUnique_v<double>);

static_assert(!AllUnique_v<void, void>);
static_assert(!AllUnique_v<void, int, void>);
static_assert(!AllUnique_v<int, void, void>);
static_assert(!AllUnique_v<int, void, int>);

struct S {};
static_assert(AllUnique_v<void, int, float, double, S, std::pair<int, float> >);
static_assert(!AllUnique_v<S, int, float, double, S, std::pair<int, float> >);

// Constexpr value tests.
static_assert(AllUniqueValues());
static_assert(AllUniqueValues(1));
static_assert(AllUniqueValues(1, 2));
static_assert(AllUniqueValues(1, 2, 3));
static_assert(AllUniqueValues(1, 2, 3, 4));

static_assert(!AllUniqueValues(1, 1, 2, 3, 4));
static_assert(!AllUniqueValues(1, 2, 1, 3, 4));
static_assert(!AllUniqueValues(1, 2, 3, 1, 4));
static_assert(!AllUniqueValues(1, 2, 3, 4, 1));

static_assert(!AllUniqueValues(2, 1, 2, 3, 4));
static_assert(!AllUniqueValues(3, 2, 1, 3, 4));
static_assert(!AllUniqueValues(4, 2, 3, 1, 4));

struct Foo {
  int val;

  constexpr bool operator==(const Foo& rhs) const { return val == rhs.val; };
};

static_assert(AllUniqueValues(Foo{1}));
static_assert(AllUniqueValues(Foo{1}, Foo{2}));
static_assert(AllUniqueValues(Foo{1}, Foo{2}, Foo{3}));

static_assert(!AllUniqueValues(Foo{1}, Foo{1}));
static_assert(!AllUniqueValues(Foo{1}, Foo{1}, Foo{2}));
static_assert(!AllUniqueValues(Foo{1}, Foo{1}, Foo{2}, Foo{3}));

}  // namespace
