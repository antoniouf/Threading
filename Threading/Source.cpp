#include <ThreadSafeQueue.h>
#include <ObjectLevelLockable.h>
#include <mutex>
#include <thread>
#include <iostream>

int counter = 0;
std::mutex Mtx;

void AcquireLockOnMutex()
{ // Acquire lock on mutex, sleep for 1s and release lock
    Mtx.lock();
    std::cout << "Obtained lock on mutex and going to sleep..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Waking up from sleep..." << std::endl;
    Mtx.unlock();
}

void IncreaseCnt()
{ // acquire lock and increase counter
    std::lock_guard<std::mutex> Lock(Mtx);
    std::cout << "Obtained lock on mutex and increasing counter..." << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        ++counter;
        std::cout << counter << std::endl;
    }
}

int main()
{
    constexpr int NumOfThreads = 10;
    std::thread InterruptingThread;
    std::thread Threads[NumOfThreads];
    for (int i = 0; i < NumOfThreads; ++i)
    {
        if (i == 5)
        { // Block other threads while sleeping
            InterruptingThread = std::thread(AcquireLockOnMutex);
        }

        Threads[i] = std::thread(IncreaseCnt);
    }

    for (int i = 0; i < NumOfThreads; ++i)
    {
        Threads[i].join();
    }

    InterruptingThread.join();
    return 0;
}