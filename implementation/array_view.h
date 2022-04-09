#ifndef JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_
#define JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_

#include "implementation/jni_helper/jni_array_helper.h"
#include "jni_dep.h"

namespace jni {

template <typename SpanType>
class ArrayView {
 public:
  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  ArrayView(jarray array, bool copy_on_completion)
      : array_(array),
        get_array_elements_result_(
            JniArrayHelper<SpanType>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion) {}

  ~ArrayView() {
    JniArrayHelper<SpanType>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  SpanType* ptr() { return get_array_elements_result_.ptr_; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType>
ArrayView(ArrayView<SpanType>&&) -> ArrayView<SpanType>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_
