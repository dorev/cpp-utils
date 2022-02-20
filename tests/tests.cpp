#include "gtest/gtest.h"
#include "result.h"
#include "threadpool.h"
#include "objectpool.h"

class Tests : public ::testing::Test
{
};

TEST_F(Tests, AsyncResult)
{
    AsyncResult<int> async;
    EXPECT_EQ(async.GetState(), AsyncResultState::Pending);
    EXPECT_FALSE(async.IsReady());

    AsyncResultUpdater::StartProcessing(async);
    EXPECT_EQ(async.GetState(), AsyncResultState::Processing);
    EXPECT_FALSE(async.IsReady());

    Error updateError = AsyncResultUpdater::Update(async, 1, AsyncResultState::Successful);
    EXPECT_EQ(Error{0}, updateError);
    EXPECT_EQ(async.GetValue(), 1);
    EXPECT_EQ(async.GetState(), AsyncResultState::Successful);
    EXPECT_TRUE(async.IsSuccessful());
    EXPECT_TRUE(async.IsReady());

    updateError = AsyncResultUpdater::Update(async, 2, AsyncResultState::Cancelled);
    EXPECT_EQ(Error{0}, updateError);
    EXPECT_EQ(async.GetValue(), 2);
    EXPECT_EQ(async.GetState(), AsyncResultState::Cancelled);
    EXPECT_TRUE(async.IsCancelled());
    EXPECT_TRUE(async.IsReady());

    updateError = AsyncResultUpdater::Update(async, Error{1, "Test error"}, AsyncResultState::Failed, true);
    EXPECT_EQ(Error{0}, updateError);
    EXPECT_EQ(async.GetState(), AsyncResultState::Failed);
    EXPECT_TRUE(async.IsFailed());
    EXPECT_TRUE(async.IsReady());
    Error testError{1, "Test error"};
    EXPECT_EQ(async.GetError(), testError);

    updateError = AsyncResultUpdater::Update(async, 4, AsyncResultState::Successful);
    Error lockedError{ErrorCode::AsyncResultLocked, "AsyncResult already set and locked"};
    EXPECT_EQ(lockedError, updateError);
}

TEST_F(Tests, ErrorBoolConversion)
{
    Error trueValue1{1};
    if(!trueValue1)
        FAIL();

    Error trueValue2{};
    if(!trueValue2)
        FAIL();

    Error falseValue{ErrorCode::NoError};
    if(falseValue)
        FAIL();
}

TEST_F(Tests, ResultBoolConversion)
{
    Result<> falseValue1{};
    if(falseValue1)
        FAIL();

    Result<> falseValue2{1};
    if(falseValue2)
        FAIL();

    Result<> trueValue{Success};
    if(!trueValue)
        FAIL();
}

class SampleTask : public IThreadPoolTask
{
public:

    SampleTask(U32 value)
        : _asyncResultPtr(MakeShared<AsyncResultType>())
        , _value(value)
    {
    }

    // Mandatory definition to satisfy the constraints of TaskManager::emplaceTask
    using AsyncResultType = AsyncResult<U32>;

    // Mandatory definition to satisfy the constraints of TaskManager::emplaceTask
    SharedPtr<AsyncResultType> getAsyncResult()
    {
        return _asyncResultPtr;
    }

    void Execute() override
    {
        AsyncResultUpdater::Update(*_asyncResultPtr, ++_value, AsyncResultState::Successful);
    }

    void Cancel() override
    {
        AsyncResultUpdater::Update(*_asyncResultPtr, Error{ErrorCode::TaskCancelled}, AsyncResultState::Cancelled);
    }

    SharedPtr<AsyncResultType> _asyncResultPtr;
    U32 _value;
};

TEST_F(Tests, ThreadPoolTest)
{
    ThreadPool threadPool(2);
    EXPECT_EQ(threadPool.GetState(), ThreadPool::State::Idle);
    SharedPtr<SampleTask::AsyncResultType> asyncResult = threadPool.Post<SampleTask>(1);
    EXPECT_TRUE(asyncResult->IsPending());
    threadPool.Start();
    EXPECT_EQ(threadPool.GetState(), ThreadPool::State::Running);
    SleepMs(10);
    EXPECT_TRUE(asyncResult->IsSuccessful());
    EXPECT_EQ(asyncResult->GetValue(), 2);
}

TEST_F(Tests, FileTests)
{
    CreateFile("./");
}

TEST_F(Tests, ObjectPoolTest)
{
    struct TestObj : Poolable<TestObj>
    {
        TestObj() : value1(0), value2(0) {}
        TestObj(int value1) : value1(value1) {}
        TestObj(int value1, int value2) : value1(value1), value2(value2) {}
        int value1;
        int value2;
    };
    ObjectPool<TestObj> pool(3);
    TestObj* obj1 = pool.GetObject();
    TestObj* obj2 = pool.ConstructObject(1);
    TestObj* obj3 = pool.ConstructObject(1, 2);
    pool.FreeObject(obj1);
    pool.FreeObject(obj3);
    TestObj* obj4 = pool.GetObject();
    TestObj* obj5 = pool.GetObject();
}