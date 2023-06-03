cmake_minimum_required(VERSION 3.7...3.26)
project(ixxxutil CXX)

set(LIB_SRC
    ixxx/util.cc
    ixxx/socket.cc
    ixxx/pthread_util.cc
)

add_library(ixxxutil        SHARED ${LIB_SRC})
add_library(ixxxutil_static STATIC ${LIB_SRC})

target_link_libraries(ixxxutil        PUBLIC ixxx)
target_link_libraries(ixxxutil_static PUBLIC ixxx_static)

target_include_directories(ixxxutil        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ixxxutil_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# under windows shared/static libraries have the same extension ...
if(UNIX)
    set_target_properties(ixxxutil_static PROPERTIES OUTPUT_NAME ixxxutil)
endif()


# guard from super-projects, i.e. when it is added as subdirectory
IF(${CMAKE_PROJECT_NAME} STREQUAL "ixxxutil")
    add_subdirectory(libixxx)

    # only for the unittests
    find_package(Boost 1.53
    COMPONENTS
        system
        filesystem
        unit_test_framework
    REQUIRED)

    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    add_executable(ut
        ${LIB_SRC}
        test/main.cc
        test/util.cc
        test/helper.cc
    )
    target_link_libraries(ut PUBLIC
        ixxx_static
        ${Boost_LIBRARIES}
        Threads::Threads
    )
    target_include_directories(ut PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # for executing it from a quickfix environment
    add_custom_target(check COMMAND ut)
endif()
