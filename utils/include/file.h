#pragma once 

#include "types.h"
#include "system.h"
#include "result.h"

class File
{
public:
    enum class State
    {
        Uninitialized,
        Available,
        Writing,
        Reading,
        Deleted
    };

    enum class Mode
    {
        Unknown = 0,
        Write = 1 << 0,
        Read = 1 << 1,
        Open = 1 << 2,
        Append = 1 << 3,
        Create = 1 << 4
    };

    File()
        : _path("")
        , _size(0)
        , _mode(static_cast<U32>(Mode::Unknown))
        , _state(State::Uninitialized)
    {
    }

    File(const String& path, U32 mode = (static_cast<U32>(Mode::Open) | static_cast<U32>(Mode::Append)))
        : _path(path)
        , _size(0)
        , _mode(mode)
        , _state(State::Uninitialized)
    {
        if (!FileExists(path))
        {
            



        }
    }

    static Result<> DeleteFile(File& path)
    {
    }

    File& Append(const String& text)
    {
        return *this;
    }

    File& Append(const void* rawMemory, U32 length)
    {
        return *this;
    }

    inline const String& GetPath() const
    {
        return _path;
    }

protected:
    State _state;
    String _path;
    U32 _mode;
    U64 _size;
    Mutex _lock;
};
