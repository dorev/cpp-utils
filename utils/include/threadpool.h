#pragma once 

#include "asyncresult.h"
#include "system.h"

class IThreadPoolTask
{

public:

    virtual void Execute() = 0;
    virtual void Cancel() = 0;
};

// Constraints a Task declaration must satisfy to be injectable into TaskManager::emplaceTask
template <class SelfType>
concept ThreadPoolTask =
    // is a child of task interface
    std::is_base_of<IThreadPoolTask, SelfType>::value
    // defines a member AsyncResultType
    && requires
    {
        typename SelfType::AsyncResultType;
    }
    // has a method .getAsyncResult() returning a SharedPointer of the same AsyncResultType
    && requires (SelfType t)
    {
        { t.getAsyncResult() } -> std::same_as<SharedPtr<typename SelfType::AsyncResultType>>;
    };

class ThreadPool
{
public:

    DELETE_COPY_CONSTRUCTOR(ThreadPool);

    enum class State
    {
        Idle,
        Running,
        Stopping
    };

    ThreadPool(U32 threadCount)
        : _state(State::Idle)
        , _threadCount(threadCount > 0 ? threadCount : 1)
    {
    }

    ~ThreadPool()
    {
        Join();
    }

    ThreadPool::State GetState()
    {
        return _state;
    }

    ThreadPool& SetThreadCount(U32 threadCount)
    {
        _threadCount = threadCount > 0 ? threadCount : 1;
        return *this;
    }

    U32 GetThreadCount()
    {
        return _threadCount;
    }

    // Start worker threads, restart if already running
    Result<State> Start()
    {
        return Start(_threadCount);
    }

    // Start worker threads, restart if already running
    Result<State> Start(U32 threadCount)
    {
        if (_state == State::Running)
            Join();
        SetThreadCount(threadCount);
        for (U32 i = 0; i < _threadCount; ++i)
            _threads.emplace_back([this](){ WorkerLoop(); });
        return _state = State::Running;
    }

    // Join all threads, cancel remaining tasks by default
    void Join(bool cancelRemainingTasks = true)
    {
        _state = State::Stopping;
        _conditionVariable.notify_all();
        for(Thread& thread : _threads)
            thread.join();
        _threads.clear();
        if (cancelRemainingTasks)
        {
            for (IThreadPoolTask* task : _taskQueue)
            {
                task->Cancel();
                delete task;
            }
            _taskQueue.clear();
        }
        _state = State::Idle;
    }

    // Lock queue, instantiate and store task, notify a worker, returns handle
    template <ThreadPoolTask TaskType, class... Args>
    SharedPtr<typename TaskType::AsyncResultType> Post(Args&&... args)
    {
        ScopedLock lock(_taskQueueMutex);
        TaskType* task = new TaskType(std::forward<Args>(args)...);
        _taskQueue.emplace_back(static_cast<IThreadPoolTask*>(task));
        _conditionVariable.notify_one();
        return task->getAsyncResult();
    }

private:

    // Lock the queue and attempt to execute a task when signaled. Once signaled,
    // it will loop until the queue is empty or until the ThreadPool is stopped
    void WorkerLoop()
    {
        while (_state == State::Running)
        {
            IThreadPoolTask* task = nullptr;
            {
                UniqueLock lock(_taskQueueMutex);
                _conditionVariable.wait(lock, [this]()
                {
                    return _state == State::Stopping || !_taskQueue.empty();
                });
                if(_state == State::Stopping)
                    return;
                task = _taskQueue.front();
                _taskQueue.pop_front();
            }
            if(task != nullptr)
                task->Execute();
        }
    }

    ThreadPool::State _state;
    U32 _threadCount;
    Deque<IThreadPoolTask*> _taskQueue;
    Mutex _taskQueueMutex;
    ConditionVariable _conditionVariable;
    Vector<Thread> _threads;
};
