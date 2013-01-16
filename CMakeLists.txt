cmake_minimum_required(VERSION 2.8)
project(darc_component)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES darc_component
  CATKIN_DEPENDS darc_common iris darc_ns darc_pubsub darc_procedure
)

find_package(catkin REQUIRED COMPONENTS iris darc_pubsub darc_procedure darc_common darc_ns)
find_package(Boost REQUIRED COMPONENTS system)


include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

add_library(darc_component
  src/owner.cpp
  src/primitive.cpp
  src/component.cpp
  src/component_manager.cpp
  src/thread_manager.cpp
  src/registry.cpp
)

target_link_libraries(darc_component ${catkin_LIBRARIES} -pthread)

add_executable(autoload_peer src/autostart_peer.cpp)
target_link_libraries(autoload_peer darc_component)

add_library(my_component1
  test/my_component1.cpp
)

target_link_libraries(my_component1 darc_component)


add_executable(my_ping_pong_node
  test/ping_pong_node.cpp
)
target_link_libraries(my_ping_pong_node darc_component -lzmq)

add_executable(my_ping_node
  test/ping_node.cpp
)
target_link_libraries(my_ping_node darc_component -lzmq)

add_executable(my_pong_node
  test/pong_node.cpp
)
target_link_libraries(my_pong_node darc_component -lzmq)

add_executable(my_client_node
  test/client_node.cpp
)
target_link_libraries(my_client_node darc_component -lzmq)

add_executable(my_server_node
  test/server_node.cpp
)
target_link_libraries(my_server_node darc_component -lzmq)

# old
add_executable(my_node1
  test/node1.cpp
)
target_link_libraries(my_node1 darc_component -lzmq)

add_executable(my_node2
  test/node2.cpp
)
target_link_libraries(my_node2 darc_component -lzmq)

# Add test cpp file
#catkin_add_gtest(darc_common_gtest_buffer test/buffer_gtest.cpp)
#target_link_libraries(darc_common_gtest_buffer darc_common ${GTEST_BOTH_LIBRARIES})
