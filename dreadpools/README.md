# Dreadpools
A threadpool library

## Interface
`ThreadPool::ThreadPool(const unsigned int count_threads)`
Create a new threadpool with the specified number of threads. Tries to select a reasonable number of threads if the caller does not pass an argument.

`ThreadPool::start()`
Activates the worker threads. The behavior of calling `submit` before starting a threadpool is undefined.

`ThreadPool::join()`
Waits for the workers to complete all tasks in the queue before joining all the threads.

`ThreadPool::submit(Callable &&f, Args &&...args)`
Submits a new task `f` to the threadpool to be called with `args`. Returns a `std::future` to the result of calling `f` with `args`.
NOTE: You may need to wrap a template function in a lambda, since there are issues with using C++17's CTAD here.

