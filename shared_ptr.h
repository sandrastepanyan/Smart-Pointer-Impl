#ifndef sHARED_PTR_H
#define sHARED_PTR_H
#include "ControlBlock.h"

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
public:
    shared_ptr() {}
    shared_ptr(const T& val);
    shared_ptr(T* ptr);
    shared_ptr(const shared_ptr& src);
    shared_ptr(shared_ptr&& src);
    shared_ptr(const weak_ptr<T>& weakPtr);
    ~shared_ptr();

    shared_ptr& operator=(const shared_ptr& src);
    shared_ptr& operator=(shared_ptr&& src);
    T& operator*() const;
    T* operator->() const;

    friend class weak_ptr<T>;

private:
    ControlBlock<T>* cb = nullptr;
};



template <typename T>
shared_ptr<T>::shared_ptr(const T& val)
    : cb{new ControlBlock<T>()}
{
    cb->ptr = new T(val);
    ++cb->shared_count;
}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr)
    : cb{new ControlBlock<T>()}
{
    cb->ptr = ptr;
    ++cb->shared_count;
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& src)
    : cb{src.cb}
{
    ++cb->shared_count;
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& src)
    : cb{src.cb}
{
    src.cb = nullptr;
}

template <typename T>
shared_ptr<T>::shared_ptr(const weak_ptr<T>& weakPtr) {
    cb = weakPtr.cb;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& src) {
    if (this != &src) {
        if (cb->shared_count == 0) {
            delete cb->ptr;
            delete cb;
        }
        cb = src.cb;
        ++cb->shared_count;
    }
    return *this;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& src) {
    if (this != &src) {
        if (cb->shared_count == 0) {
            delete cb->ptr;
            delete cb;
        }
        cb = src.cb;
        src.cb = nullptr;
    }
    return *this;
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    --cb->shared_count;
    if (cb->shared_count == 0) {
        delete cb->ptr;
        delete cb;
    }
}

template <typename T>
T& shared_ptr<T>::operator*() const {
    return *(cb->ptr);
}

template <typename T>
T* shared_ptr<T>::operator->() const{
    return cb->ptr;
}

#endif // sHARED_PTR_H