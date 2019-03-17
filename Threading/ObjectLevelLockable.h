#ifndef ObjectLevelLockable_634F43763A1841B08B462CF4E697433D
#define ObjectLevelLockable_634F43763A1841B08B462CF4E697433D

#include <mutex>

class ObjectLevelLockable {
public:
  ObjectLevelLockable() { m_Mtx.lock(); }

  ObjectLevelLockable(const ObjectLevelLockable &Obj) = delete;
  ObjectLevelLockable &operator=(const ObjectLevelLockable &&) = delete;

  virtual ~ObjectLevelLockable() { m_Mtx.unlock(); }

private:
  std::mutex m_Mtx;
};

#endif // #ifndef CObjectLevelLockable_634F43763A1841B08B462CF4E697433D
