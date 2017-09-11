#include <iostream>
#include <fstream>
#include <string>
#include <boost/coroutine/all.hpp>

using coro_t = boost::coroutines::coroutine<std::string>;

void cat(coro_t::push_type& yield, char *filename)
{
    std::ifstream input(filename);
    while (true)
    {
        std::string line;
        if (!getline(input, line))
        {
            break;
        }
        yield(line);
    }
}


int main(int argc, char *argv[])
{
    for (auto &line : coro_t::pull_type([argv](coro_t::push_type &yield)
                                        {
                                            cat(yield, argv[1]);
                                        }))
    {
        std::cout << line << std::endl;
    }
    
    return 0;
}
