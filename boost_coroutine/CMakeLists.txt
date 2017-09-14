add_executable(generator generator.cpp)
target_link_libraries(generator ${Boost_SYSTEM_LIBRARY} boost_coroutine)

add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server ${Boost_SYSTEM_LIBRARY} boost_coroutine)
