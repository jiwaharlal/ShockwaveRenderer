#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <memory>
#include <vector>
#include <list>
#include <condition_variable>
#include <thread>

class Job
{
public:
                    Job(
                        const std::function<void()>& aFunction);

    void            waitForCompletion();
    void            execute();
private:
    bool                        myIsDone;
    std::function<void()>       myFunction;
    std::condition_variable     myCV;
    std::mutex                  myMutex;
};

class ThreadPool
{
public:
                            ThreadPool(
                                int aThreadCount);

                            ~ThreadPool();

    std::shared_ptr<Job>    addJob(
                                const std::function<void()>&      aJobFunction);
    static ThreadPool&      instance();
    void                    stop();
private:
    static void             threadFunctionStatic(
                                int aThreadId);
    void                    threadFunction(
                                int aThreadId);

    std::vector<std::thread>            myThreads;
    std::list<std::shared_ptr<Job> >    myJobs;
    std::condition_variable             myJobPresentCondition;
    std::mutex                          myJobsMutex;
    bool                                myWorking;
};

#endif // THREADPOOL_H
