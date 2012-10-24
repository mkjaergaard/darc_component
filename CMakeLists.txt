cmake_minimum_required(VERSION 2.8)
project(darc_component)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES darc_component
)

find_package(catkin REQUIRED COMPONENTS beam darc_pubsub darc_common)
find_package(Boost REQUIRED COMPONENTS system)


include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

add_library(darc_component
  src/owner.cpp
  src/primitive.cpp
  src/component.cpp
  src/component_manager.cpp
  src/thread_manager.cpp
)

target_link_libraries(darc_component ${catkin_LIBRARIES} -pthread)

add_library(my_component1
  test/my_component1.cpp
)

target_link_libraries(my_component1 darc_component)


add_executable(my_node1
  test/node1.cpp
)

target_link_libraries(my_node1 darc_component -lzmq)

# Add test cpp file
#catkin_add_gtest(darc_common_gtest_buffer test/buffer_gtest.cpp)
#target_link_libraries(darc_common_gtest_buffer darc_common ${GTEST_BOTH_LIBRARIES})
