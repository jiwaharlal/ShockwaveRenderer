#include <iostream>
#include <thread>
#include <chrono>
#include <list>
#include <memory>
#include <condition_variable>
#include <vector>

using namespace std;

class Job
{
public:
                    Job(
                        const std::function<void()>& aFunction)
        : myIsDone(false)
        , myFunction(aFunction)
    {

    }
    void            waitForCompletion()
    {
        std::unique_lock<std::mutex> lk(myMutex);
        cout << "Waiting..." << endl;
        myCV.wait(lk, [&]{return myIsDone;});
        cout << "Finished waiting" << endl;
    }

    void            execute()
    {
        myFunction();
        myIsDone = true;
        myCV.notify_all();
    }
private:
    bool                        myIsDone;
    std::function<void()>       myFunction;
    std::condition_variable     myCV;
    std::mutex                  myMutex;
};

class ThreadPool
{
public:
    ThreadPool(int aThreadCount)
    {
        myWorking = true;
        for (int i = 0; i < aThreadCount; i++)
        {
            myThreads.emplace_back(threadFunctionStatic, i);
        }
    }

    ~ThreadPool()
    {
        cout << "In threadPool destructor" << endl;
        instance().stop();
    }

    shared_ptr<Job> addJob(std::function<void()> aJobFunction)
    {
        std::lock_guard<std::mutex> lk(myJobsMutex);
        myJobs.emplace_back(new Job(aJobFunction));
        myJobPresentCondition.notify_one();
        return myJobs.back();
    }

    static ThreadPool& instance()
    {
        static ThreadPool instance(4);

        return instance;
    }

    void stop()
    {
        for (auto& t: myThreads)
        {
            myWorking = false;
            myJobPresentCondition.notify_all();
            t.join();
        }
    }
private:
    static void threadFunctionStatic(int aThreadId)
    {
        instance().threadFunction(aThreadId);
    }

    void threadFunction(int aThreadId)
    {
        while (true)
        {
            std::unique_lock<std::mutex> lk(myJobsMutex);
            myJobPresentCondition.wait(lk, [&]{return !myJobs.empty() || !myWorking;});
            if ( !myWorking )
            {
                cout << "Stoping thread " << aThreadId << endl;
                return;
            }
            shared_ptr<Job> currentJob = myJobs.front();
            myJobs.pop_front();
            lk.unlock();

            cout << "Thread " << aThreadId << ", starting task" << endl;
            currentJob->execute();
            cout << "Thread " << aThreadId << " finished task" << endl;
        }
    }

    vector<thread> myThreads;
    list<shared_ptr<Job> > myJobs;
    condition_variable myJobPresentCondition;
    mutex myJobsMutex;
    bool myWorking;
};

void sleep(int aSeconds)
{
    cout << "Sleep for a " << aSeconds << " seconds" << endl;
    this_thread::sleep_for(chrono::seconds(aSeconds));
    cout << "Task " << aSeconds << " is waking up" << endl;
}

class A
{
public:
    A(int aDataSize)
    {
        myData.resize(aDataSize, 0);
    }

    void fillData(int aValue, int aJobCount = 10)
    {
        int chunckSize = myData.size() / aJobCount;
        int chunckBegin = 0;
        int chunckEnd;

        std::list<shared_ptr<Job> > jobs;

        while (chunckBegin < myData.size())
        {
            chunckEnd = chunckBegin + chunckSize;
            if (chunckEnd > myData.size())
            {
                chunckEnd = myData.size();
            }
            jobs.push_back(ThreadPool::instance().addJob(bind(&A::fillDataChunck, this, chunckBegin, chunckEnd, aValue)));
            chunckBegin = chunckEnd;
        }

        for (auto j: jobs)
        {
            j->waitForCompletion();
        }
    }

    void fillDataChunck(int aBeginIndex, int aEndIndex, int aValue)
    {
        cout << "Filling from " << aBeginIndex << " to " << aEndIndex << endl;

        for (int i = aBeginIndex; i < aEndIndex; i++)
        {
            myData[i] = aValue;
        }
    }

    void printData();
private:
    vector<int> myData;
};

int main()
{
    std::list<shared_ptr<Job> > jobs;

    /*for (int i = 0; i < 10; i++)
    {
        jobs.push_back(ThreadPool::instance().addJob(std::bind(sleep, i)));
    }
    for (auto j: jobs)
    {
        j->waitForCompletion();
    }
    ThreadPool::instance().stop();
    */

    A a(100);
    a.fillData(42);
    //ThreadPool::instance().stop();

    cout << "Hello world!" << endl;

    return 0;
}
