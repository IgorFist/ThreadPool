# Thread pool
This repo is implementation of thread pool.

# Description

To place a task in thread pool you must use template member function **post**. A task is a callable object with any arguments without the returning value. For a passing argument as reference use **std::ref**.

```cpp
template <typename Fn, typename... Args>
void post(Fn &&fn, Args &&...args)
{
        std::lock_guard<std::mutex> lock(mutex);
        workQueue.emplace(std::async(std::launch::deferred,
                                     std::forward<Fn>(fn),
                                     std::forward<Args>(args)...));
}
```
***Example 1***

passing a lamda-function

```cpp
Stopwatch stopwatch;
ThreadPool threadPool(5);
stopwatch.start();
for (int i = 0; i < 20; ++i)
    threadPool.post([]()
    { 
        std::this_thread::sleep_for(100ms); 
    });

std::cout << "5 thread, 20 tasks: " << stopwatch.stop() << std::endl;
```
output

```
5 thread, 10 tasks: 401 ms
```

***Example 2***

passing a function 
```cpp
void foo()
{
    std::this_thread::sleep_for(100ms);
}
...
ThreadPool threadPool(5);
stopwatch.start();
for (int i = 0; i < 20; ++i)
    threadPool.post(foo);

std::cout << "5 thread, 20 tasks: " << stopwatch.stop() << " ms" << std::endl;
```
output
```
5 thread, 10 tasks: 401 ms
```
***Example 3***

