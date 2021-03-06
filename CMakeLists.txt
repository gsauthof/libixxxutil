cmake_minimum_required(VERSION 2.8.8)
project(ixxxutil CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(LIB_SRC ixxx/util.cc ixxx/socket.cc ixxx/pthread_util.cc )

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
  target_link_libraries(ut
    ixxx_static
    ${Boost_LIBRARIES}
    Threads::Threads
  )
  set_property(TARGET ut PROPERTY INCLUDE_DIRECTORIES
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libixxx
  )

  # for executing it from a quickfix environment
  add_custom_target(check COMMAND ut)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

endif()

add_library(ixxxutil SHARED
  ${LIB_SRC}
)
target_link_libraries(ixxxutil
    ixxx
)
set_property(TARGET ixxxutil PROPERTY INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/libixxx
)
add_library(ixxxutil_static STATIC
  ${LIB_SRC}
)
set_property(TARGET ixxxutil_static PROPERTY INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/libixxx
)
# under windows shared/static libraries have the same extension ...
if(${UNIX})
  set_target_properties(ixxxutil_static PROPERTIES OUTPUT_NAME ixxxutil)
endif()
