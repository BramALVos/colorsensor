#pragma once

#include <Arduino.h>

typedef uint8_t u8;
typedef size_t usize;

class Arena {
public:
    Arena(usize alloc_size) {
        this->ptr = malloc(alloc_size);

        if (!this->ptr) {
            abort();
        }

        this->alloc_size = alloc_size;
        this->index = 0;
    }

    Arena() = delete; 
    Arena(Arena&) = delete;
    ~Arena() {
        free(this->ptr);
        this->ptr = nullptr;
    }

    void * alloc(usize size) {
        if (size + this->index >= this->alloc_size || this->ptr == nullptr) {
            abort();
        }

        this->index += size;
        return reinterpret_cast<void*>((usize)this->ptr + this->index - size);
    }

private:
    void * ptr;
    usize alloc_size;
    usize index;
};

