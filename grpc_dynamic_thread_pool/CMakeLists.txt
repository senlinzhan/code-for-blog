set(SRCS
    main.cpp
    dynamic_thread_pool.cpp
)

add_executable(grpc_dynamic_thread_pool ${SRCS})
target_link_libraries(grpc_dynamic_thread_pool ${CMAKE_THREAD_LIBS_INIT})
