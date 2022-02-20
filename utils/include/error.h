#pragma once

#include "definitions.h"
#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

class ErrorCode
{
public:

    static constexpr U64 GenericCategory = 0ull << 32;
    static constexpr U64 NoError = GenericCategory | 0;
    static constexpr U64 Undefined = GenericCategory | 1;
    static constexpr U64 InvalidParameter = GenericCategory | 2;

    static constexpr U64 ResultCategory = 1ull << 32;
    static constexpr U64 UninitializedError = ResultCategory | 0;
    static constexpr U64 AsyncResultLocked = ResultCategory | 1;

    static constexpr U64 TaskCategory = 2ull << 32;
    static constexpr U64 NoTaskToExecute = TaskCategory | 0;
    static constexpr U64 TaskManagerAlreadyStarted = TaskCategory | 1;
    static constexpr U64 TaskCancelled = TaskCategory | 2;

    static constexpr U64 CsvCategory = 3ull << 32;
    static constexpr U64 ColumnCountMismatch = CsvCategory | 0;

    static constexpr U64 FileCategory = 4ull << 32;
    static constexpr U64 UnableToCreateFile = FileCategory | 0;
    static constexpr U64 UnableToOpenFile = FileCategory | 1;
    static constexpr U64 FileDoesNotExist = FileCategory | 2;
    static constexpr U64 UnableToGetFileHandle = FileCategory | 3;
};


///////////////////////////////////////////////////////////////////////////////
// Error
///////////////////////////////////////////////////////////////////////////////

class Error
{
public:

    constexpr Error()
        : _code(ErrorCode::UninitializedError)
        , _description("Uninitialized Error")
    {
    }

    constexpr Error(U64 code, const char* description)
        : _code(code)
        , _description(description)
    {
    }

    constexpr Error(U64 code)
        : _code(code)
        , _description(nullptr)
    {
    }

    constexpr bool operator==(const Error& other) const
    {
        if(_code != other._code)
            return false;
        if(_description == nullptr)
            return other._description == nullptr;
        if(other._description == nullptr)
            return false;
        return strcmp(_description, other._description) == 0;
    }

    constexpr U64 GetCode() const
    {
        return _code;
    }

    constexpr const char* GetDescription() const
    {
        return _description;
    }

    constexpr explicit operator bool() const
    {
        return _code != ErrorCode::NoError;
    }

private:

    const char* _description;
    U64 _code;
};
