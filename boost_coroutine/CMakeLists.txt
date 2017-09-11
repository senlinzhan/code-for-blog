add_executable(generator generator.cpp)
target_link_libraries(generator ${Boost_SYSTEM_LIBRARY} boost_coroutine)
