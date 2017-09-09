#include "dynamic_thread_pool.hpp"
#include <iostream>

void fun()
{
    std::cout << "Hello, World!" << std::endl;    
}

int main(int argc, char *argv[])
{
    DynamicThreadPool pool(std::thread::hardware_concurrency());
    pool.Add(fun);
    
    return 0;
}
