#ifndef WEAK_PTR_H
#define WEAK_PTR_H
#include "ControlBlock.h"

template <typename T>
class shared_ptr;

template <typename T> 
class weak_ptr {
public:
    weak_ptr(const shared_ptr<T>& sharedPtr);
    weak_ptr(const weak_ptr<T>& src);
    weak_ptr(weak_ptr&& src);
    ~weak_ptr();

    weak_ptr& operator=(const weak_ptr& src);
    weak_ptr& operator=(weak_ptr&& src);

    shared_ptr<T> lock() const;
    bool expired() const;
    void reset();
    
    friend class shared_ptr<T>;

private:
    ControlBlock<T>* cb = nullptr;
};

template <typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& sharedPtr)
    : cb{sharedPtr.cb}
{
    ++cb->weak_count;
}

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr<T>& src) 
    : cb{src.cb}
{
    ++cb->weak_count;
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr<T>&& src)
    : cb{src.cb}
{
    src.cb = nullptr;
}

template <typename T>
weak_ptr<T>::~weak_ptr() {}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<T>& src) {
    --cb->weak_count;
    cb = src.cb;
    ++cb->weak_count;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<T>&& src) {
    cb = src.cb;
    src.cb = nullptr;
}

template <typename T>
bool weak_ptr<T>::expired() const {
    return cb->shared_count == 0;
}

template <typename T>
void weak_ptr<T>::reset() {
    delete cb->ptr;
    delete cb;
}

template <typename T>
shared_ptr<T> weak_ptr<T>::lock() const {
    return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
}

#endif // WEAK_PTR_H