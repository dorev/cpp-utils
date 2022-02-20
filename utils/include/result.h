#pragma once

#include "definitions.h"
#include "error.h"
#include "types.h"

static constexpr Error Success(ErrorCode::NoError);

///////////////////////////////////////////////////////////////////////////////
// Result
///////////////////////////////////////////////////////////////////////////////

template <class ValueType = Empty>
class Result
{
public:
    Result()
        : _error()
        , _value()
    {
    }

    Result(const Result& other)
        : _error(other._value)
        , _value(other._error)
    {
    }

    Result(const ValueType& value)
        : _error(ErrorCode::NoError)
        , _value(value)
    {
    }

    Result(ValueType&& value)
        : _error(ErrorCode::NoError)
        , _value(value)
    {
    }

    Result(const Error& error)
        : _error(error)
        , _value()
    {
    }

    Result(Error&& error)
        : _error(error)
        , _value()
    {
    }

    virtual ~Result()
    {
    }

    bool IsValid() const
    {
        return _error.GetCode() == ErrorCode::NoError;
    }

    explicit operator bool() const
    {
        return IsValid();
    }

    bool HasError()
    {
        return !IsValid();
    }

    const Error& GetError()
    {
        return _error;
    }

    const ValueType& GetValueConst()
    {
        return _value;
    }

    ValueType& GetValue()
    {
        return _value;
    }

protected:
    Error _error;
    ValueType _value;
};
