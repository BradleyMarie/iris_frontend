#ifndef _SRC_SHARED_PTR_
#define _SRC_SHARED_PTR_

namespace iris {

template <typename Type, void (*Retain)(Type*), void (*Release)(Type*)>
class SharedPtr {
 public:
  SharedPtr() : m_ptr(nullptr) {}
  SharedPtr(const SharedPtr& other) : m_ptr(other.m_ptr) { Retain(m_ptr); }
  ~SharedPtr() { Release(m_ptr); }
  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      Release(m_ptr);
      m_ptr = other.m_ptr;
      Retain(m_ptr);
    }
    return *this;
  }

  Type* get() { return m_ptr; }
  Type* get() const { return m_ptr; }

  void reset() {
    Release(m_ptr);
    m_ptr = nullptr;
  }

  Type* detach() {
    auto result = m_ptr;
    m_ptr = nullptr;
    return result;
  }

  Type** release_and_get_address() {
    Release(m_ptr);
    m_ptr = nullptr;
    return &m_ptr;
  }

 private:
  Type* m_ptr;
};

template <typename Type, void (*Retain)(Type*), void (*Release)(Type*)>
bool operator<(const SharedPtr<Type, Retain, Release>& left,
               const SharedPtr<Type, Retain, Release>& right) {
  return left.get() < right.get();
}

}  // namespace iris

#endif  // _SRC_SHARED_PTR_