#pragma once

#include "definitions.h"
#include "error.h"
#include "types.h"
#include "result.h"

enum class AsyncResultState
{
    Pending,
    Processing,
    Successful,
    Cancelled,
    Failed
};

class AsyncResultUpdater;

template <class ValueType = Empty>
class AsyncResult : public Result<ValueType>
{
public:

    AsyncResult()
        : Result<ValueType>()
        , _resultIsAlreadySet(false)
        , _state(AsyncResultState::Pending)
    {
    }

    AsyncResult(const AsyncResult& other)
        : Result<ValueType>(other)
        , _resultIsAlreadySet(other._resultIsAlreadySet)
        , _state(other._state)
    {
    }

    bool IsReady() const
    {
        return _state != AsyncResultState::Pending
            && _state != AsyncResultState::Processing;
    }

    bool IsPending() const
    {
        return _state == AsyncResultState::Pending;
    }

    bool IsProcessing() const
    {
        return _state == AsyncResultState::Processing;
    }

    bool IsSuccessful() const
    {
        return _state == AsyncResultState::Successful;
    }

    bool IsCancelled() const
    {
        return _state == AsyncResultState::Cancelled;
    }

    bool IsFailed() const
    {
        return _state == AsyncResultState::Failed;
    }

    const AsyncResultState& GetState() const
    {
        return _state;
    }

private:

    // Using the AsyncResultUpdater is a way to prevent the user of the AsyncResult
    // from directly (or discretly) update its content and/or state while keeping
    // a simple way to do so
    friend class AsyncResultUpdater;
    using ResultType = Result<ValueType>;

    void StartProcessing()
    {
        _state = AsyncResultState::Processing;
    }

    Error Update(const ValueType& value, AsyncResultState state, bool lock = false)
    {
        if(_resultIsAlreadySet)
            return Error{ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};
        ResultType::_value = value;
        _state = state;
        return Success;
    }

    Error Update(ValueType&& value, AsyncResultState state, bool lock = false)
    {
        if(_resultIsAlreadySet)
            return Error{ ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};
        ResultType::_value = std::move(value);
        _state = state;
        _resultIsAlreadySet = lock;
        return Success;
    }

    Error Update(Error&& error, AsyncResultState state, bool lock = false)
    {
        if(_resultIsAlreadySet)
            return Error{ ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};
        ResultType::_error = std::move(error);
        _state = state;
        _resultIsAlreadySet = lock;
        return Success;
    }

    bool _resultIsAlreadySet;
    AsyncResultState _state;
};


// Keeping this class private will prevent users to alter AsyncResult internal members
class AsyncResultUpdater
{
public:

    DELETE_DEFAULT_CONSTRUCTOR(AsyncResultUpdater);
    DELETE_COPY_CONSTRUCTOR(AsyncResultUpdater);

    template <class ValueType>
    static void StartProcessing(AsyncResult<ValueType>& asyncResult)
    {
        asyncResult.StartProcessing();
    }

    template <class ValueType>
    static Error Update(AsyncResult<ValueType>& asyncResult, const ValueType& value, AsyncResultState state, bool lock = false)
    {
        return asyncResult.Update(value, state, lock);
    }

    template <class ValueType>
    static Error Update(AsyncResult<ValueType>& asyncResult, ValueType&& value, AsyncResultState state, bool lock = false)
    {
        return asyncResult.Update(std::forward<ValueType>(value), state, lock);
    }

    template <class ValueType>
    static Error Update(AsyncResult<ValueType>& asyncResult, Error&& error, AsyncResultState state, bool lock = false)
    {
        return asyncResult.Update(std::forward<Error>(error), state, lock);
    }
};
