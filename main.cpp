#include <thread_pool.h>
#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std::chrono;

class Stopwatch
{
public:
    Stopwatch() : isStopped(false)
    {
    }

    void start()
    {
        startTime = high_resolution_clock::now();
    }

    int stop()
    {
        if (!isStopped)
        {
            auto duration = high_resolution_clock::now() - startTime;
            return duration_cast<milliseconds>(duration).count();
        }

        return 0;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    bool isStopped;
};

void foo()
{
    std::this_thread::sleep_for(100ms);
}

int main()
{

    //post lamda function
    Stopwatch stopwatch;

    {
        ThreadPool threadPool(5);
        stopwatch.start();
        for (int i = 0; i < 20; ++i)
            threadPool.post([]()
                            { std::this_thread::sleep_for(100ms); });
    }

    std::cout << "5 thread, 20 tasks: " << stopwatch.stop() << " ms" << std::endl;

    //post function void foo() {std::this_thread::sleep_for(100ms);}
    {
        ThreadPool threadPool(5);
        stopwatch.start();
        for (int i = 0; i < 20; ++i)
            threadPool.post(foo);
    }

    std::cout << "5 thread, 20 tasks: " << stopwatch.stop() << " ms" << std::endl;

}