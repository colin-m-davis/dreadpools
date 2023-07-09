# Dreadpools
A threadpool library

## Interface
`ThreadPool::ThreadPool(const unsigned int count_threads)`

Create a new threadpool with the specified number of threads. Tries to select a reasonable number of threads if the caller does not pass an argument.

`ThreadPool::start()`

Activates the worker threads. The behavior of a program that calls `submit` before starting a threadpool is undefined.

`ThreadPool::join()`

Waits for the workers to complete all tasks in the queue before joining all the threads.

`ThreadPool::submit(Callable &&f, Args &&...args)`

Submits a new task `f` to the threadpool to be called with `args`. Returns a `std::future` to the result of calling `f` with `args`.
NOTE: When submitting a function template to a threadpool, you need to specify the template arguments or wrap the function in a lambda to avoid compiler errors, since there are issues with using C++17's CTAD here.

