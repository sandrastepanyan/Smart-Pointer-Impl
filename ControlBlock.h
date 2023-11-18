#ifndef CONTROL_BLOCK_H
#define CONTROL_BLOCK_H 

template <typename T>
struct ControlBlock {
    T* ptr{};
    int shared_count{};
    int weak_count{};
};

#endif // CONTROL_BLOCK_H