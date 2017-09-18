add_executable(main main.cpp)
target_link_libraries(main ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_executable(main2 main2.cpp)
target_link_libraries(main2 ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
