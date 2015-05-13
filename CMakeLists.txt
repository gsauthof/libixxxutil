cmake_minimum_required(VERSION 2.8.8)
project(ixxxutil CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_subdirectory(libixxx)

# guard from super-projects, i.e. when it is added as subdirectory
IF(${CMAKE_PROJECT_NAME} STREQUAL "ixxxutil")

  # only for the unittests
  find_package(Boost 1.53
    COMPONENTS
      system
      filesystem
      unit_test_framework
    REQUIRED)

  add_executable(ut
    ixxx/util/boost.cc
    ixxx/util.cc
    test/main.cc
    test/boost.cc
    test/util.cc
  )
  target_link_libraries(ut
    ixxx_static
    ${Boost_LIBRARIES}
  )
  set_property(TARGET ut PROPERTY INCLUDE_DIRECTORIES
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libixxx
  )

endif()

set(LIB_SRC ixxx/util.cc ixxx/util/boost.cc)
add_library(ixxxutil SHARED
  ${LIB_SRC}
)
add_library(ixxxutil_static STATIC
  ${LIB_SRC}
)
# under windows shared/static libraries have the same extension ...
if(${UNIX})
  set_target_properties(ixxxutil_static PROPERTIES OUTPUT_NAME ixxxutil)
endif()
