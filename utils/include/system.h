#pragma once 

#include "types.h"
#include "result.h"
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>


// Time

void SleepMs(U64 milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Filesystem

bool FileExists(const String& path)
{
    std::filesystem::path fsPath(path.c_str());
    return std::filesystem::exists(fsPath);
}

bool IsFile(const String& path)
{
    return  std::filesystem::is_regular_file(path);
}

bool IsDirectory(const String& path)
{
    return  std::filesystem::is_directory(path);
}

Result<> CreateFile(const String& path)
{
    if(FileExists(path))
        return Success;
    std::fstream file(path.c_str());
    if(!!file)
        return Success;
    else
        return { ErrorCode::UnableToCreateFile };
}

Result<> OpenFile(const String& path)
{
    if(!FileExists(path))
        return { ErrorCode::FileDoesNotExist };
    std::fstream file(path.c_str());
    if(!file)
        return { ErrorCode::UnableToGetFileHandle };
    if(!file.is_open())
        return { ErrorCode::UnableToOpenFile };
    return Success;
}

Result<> CloseFile(const String& path)
{
    if(!FileExists(path))
        return { ErrorCode::FileDoesNotExist };
    std::fstream file(path.c_str());
    if(!file)
        return { ErrorCode::UnableToGetFileHandle };
    file.close();
    return Success;
}

Result<> DeleteFile(const String& path)
{
    if(!FileExists(path))
        return { ErrorCode::FileDoesNotExist };

    
}
