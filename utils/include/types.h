#pragma once

#include <filesystem>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <string>
#include <regex>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
// Helper types
///////////////////////////////////////////////////////////////////////////////

struct Empty
{
};

template <int I>
struct IntToType
{
    static constexpr int value = I;
    using next = IntToType<I + 1>;
    using prev = IntToType<I - 1>;
};

///////////////////////////////////////////////////////////////////////////////
// Primitive type aliases
///////////////////////////////////////////////////////////////////////////////

using U8 = uint8_t;
constexpr U8 U8MAX = UINT8_MAX;
using U16 = uint16_t;
constexpr U16 U16MAX = UINT16_MAX;
using U32 = uint32_t;
constexpr U32 U32MAX = UINT32_MAX;
using U64 = uint64_t;
constexpr U64 U64MAX = UINT64_MAX;
using S8 = int8_t;
constexpr S8 S8MAX = INT8_MAX;
constexpr S8 S8MIN = INT8_MIN;
using S16 = int16_t;
constexpr S16 S16MAX = INT16_MAX;
constexpr S16 S16MIN = INT16_MIN;
using S32 = int32_t;
constexpr S32 I32MAX = INT32_MAX;
constexpr S32 S32MIN = INT32_MIN;
using S64 = int64_t;
constexpr S64 S64MAX = INT64_MAX;
constexpr S64 S64MIN = INT64_MIN;
using F32 = float;
constexpr F32 F32MAX = FLT_MAX;
constexpr F32 F32MIN = FLT_MIN;
using F64 = double;
constexpr F64 F64MAX = DBL_MAX;
constexpr F64 F64MIN = DBL_MIN;

///////////////////////////////////////////////////////////////////////////////
// Containers
///////////////////////////////////////////////////////////////////////////////

template <class SelfType, class U>
using Pair = std::pair<SelfType, U>;

template <class SelfType>
using Vector = std::vector<SelfType>;

template <class SelfType>
using Queue = std::queue<SelfType>;

template <class SelfType>
using Deque = std::deque<SelfType>;

template <class SelfType>
using List = std::list<SelfType>;

using String  = std::string;

template <class SelfType>
using Set = std::set<SelfType>;

template <class SelfType>
using HashSet = std::unordered_set<SelfType>;

template <class SelfType, class U>
using Map = std::map<SelfType, U>;

template <class SelfType,class U>
using HashMap = std::unordered_map<SelfType, U>;

///////////////////////////////////////////////////////////////////////////////
// Pointers
///////////////////////////////////////////////////////////////////////////////

template <class SelfType>
using SharedPtr = std::shared_ptr<SelfType>;

template <class SelfType, class... Args>
SharedPtr<SelfType> MakeShared(Args... args)
{
    return std::make_shared<SelfType>(std::forward(args)...);
}

template <class SelfType>
using WeakPtr = std::weak_ptr<SelfType>;

template <class SelfType>
using UniquePtr = std::unique_ptr<SelfType>;

template <class SelfType, class... Args>
UniquePtr<SelfType> MakeUnique(Args... args)
{
    return std::make_unique<SelfType>(std::forward(args)...);
}

///////////////////////////////////////////////////////////////////////////////
// Concurrency
///////////////////////////////////////////////////////////////////////////////

using Thread = std::thread;

template<class SelfType>
using Atomic = std::atomic<SelfType>;

using ConditionVariable = std::condition_variable;

using Mutex = std::mutex;

template <class... MutexTypes>
using ScopedLock = std::scoped_lock<MutexTypes...>;

using UniqueLock = std::unique_lock<Mutex>;

using WriteLock = std::unique_lock<Mutex>;

using ReadLock = std::shared_lock<Mutex>;
