cmake_minimum_required(VERSION 2.8)
project(darc_component)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES darc_component
  CATKIN_DEPENDS darc_common iris darc_ns darc_pubsub darc_procedure
)

find_package(catkin REQUIRED COMPONENTS iris darc_pubsub darc_procedure darc_common darc_ns)
find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_library(darc_component
  src/owner.cpp
  src/primitive.cpp
  src/component.cpp
  src/component_manager.cpp
  src/thread_manager.cpp
  src/registry.cpp
)
target_link_libraries(darc_component ${catkin_LIBRARIES} -pthread -ldl ${Boost_LIBRARIES})

find_package(Boost REQUIRED COMPONENTS program_options)
add_executable(darc_component_manager
  src/component_manager_executable.cpp
)
target_link_libraries(darc_component_manager darc_component ${Boost_LIBRARIES})
install(TARGETS darc_component_manager
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#add_subdirectory(pybindings)

# Gtest
catkin_add_gtest(darc_component_publisher_gtest test/publisher_gtest.cpp)
target_link_libraries(darc_component_publisher_gtest
  ${GTEST_BOTH_LIBRARIES}
  darc_component
  -pthread)

add_library(my_ping_component
  speed_test/my_ping_component.cpp)
target_link_libraries(my_ping_component darc_component)

add_library(my_pong_component
  speed_test/my_pong_component.cpp)
target_link_libraries(my_pong_component darc_component)

add_executable(ping_pong_peer
  speed_test/ping_pong_node.cpp)
target_link_libraries(ping_pong_peer darc_component)

