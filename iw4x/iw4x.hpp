#pragma once

#include <windows.h>
#include <functional>

template <typename T> inline std::function<T> call(std::uintptr_t callback)
{
    return std::function<T>(reinterpret_cast<T*>(callback));
}

template<typename T> inline T variable(std::uintptr_t ref)
{
    __asm mov eax, [ref]
}
