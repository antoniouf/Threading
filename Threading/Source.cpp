#include "ThreadSafeQueue.h"
#include <mutex>
#include <thread>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////////

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

void TestThreading()
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
}

//////////////////////////////////////////////////////////////////////////////

volatile DWORD Start;
volatile int Global = 0;

double TestSequential(long long NumSteps)
{
    long long i;
    Global++;
    double x = 0.0, Pi = 0.0, Sum = 0.0, Step = 0.0;

    Step = 1.0 / (double)NumSteps;

    for (i = 1; i <= NumSteps; i++) {
        x = (i - 0.5) * Step;
        Sum = Sum + 4.0 / (1.0 + x * x);
    }

    Pi = Step * Sum;
    return Pi;
}

double TestParallel(long long NumSteps) {
    long long i;
    Global++;
    double x = 0.0, Pi = 0.0, Sum = 0.0, Step = 0.0;

    Step = 1.0 / (double)NumSteps;

#pragma omp parallel for reduction(+:Sum) private(x)
    for (i = 1; i <= NumSteps; i++) {
        x = (i - 0.5) * Step;
        Sum = Sum + 4.0 / (1.0 + x * x);
    }

    Pi = Step * Sum;
    return Pi;
}

void TestOpenMP(int argc, char* argv[])
{
    double d;
    long long n = 1e10;

    if (argc > 1)
    {
        n = atoi(argv[1]);
    }

    Start = GetTickCount64();
    d = TestSequential(n);
    const DWORD TimeSingleThread = GetTickCount64() - Start;
    printf_s("For %d steps, pi = %.15f, single-thread: %lu milliseconds\n",
        n, d, TimeSingleThread);

    Start = GetTickCount64();
    d = TestParallel(n);
    const DWORD TimeMultiThread = GetTickCount64() - Start;
    printf_s("For %d steps, pi = %.15f, parallel: %lu milliseconds\n",
        n, d, TimeMultiThread);

    const double SpeedUp = static_cast<double>(TimeSingleThread) / TimeMultiThread;
    std::cout << "The speedup using multithreading with OpemMP: " <<
        std::setprecision(5) << SpeedUp  << "\n";
}

//////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // Simple test for OpenMP interface
    // This statement will run on each thread. If there
    // are 8 threads this will execute 8 times in total. 
#pragma omp parallel
    {
        std::cout << "Running on parallel threads..\n";
    }

    TestThreading();
    TestOpenMP(argc, argv);

    return 0;
}