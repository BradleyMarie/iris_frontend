#ifndef _SRC_COMMON_UNIQUE_PTR_
#define _SRC_COMMON_UNIQUE_PTR_

namespace iris {

template <typename Type, void (*Release)(Type*)>
class UniquePtr {
 public:
  UniquePtr(const UniquePtr& other) = delete;
  UniquePtr& operator=(const UniquePtr& other) = delete;

  UniquePtr() : m_ptr(nullptr) {}
  UniquePtr(UniquePtr&& other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
  ~UniquePtr() { Release(m_ptr); }
  UniquePtr& operator=(UniquePtr&& other) {
    if (this != &other) {
      m_ptr = other.m_ptr;
      other.m_ptr = nullptr;
    }
    return *this;
  }

  Type* get() { return m_ptr; }
  const Type* get() const { return m_ptr; }

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

}  // namespace iris

#endif  // _SRC_COMMON_UNIQUE_PTR_