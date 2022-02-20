#pragma once 

#include "types.h"
#include <string>

template<class SelfType>
String&& ToString(const SelfType& value)
{
    return std::move(std::to_string(value));
}
