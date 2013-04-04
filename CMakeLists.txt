cmake_minimum_required(VERSION 2.8)
project(darc_component)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES darc_component
  CATKIN_DEPENDS darc
)

find_package(catkin REQUIRED COMPONENTS darc)

include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

# library
add_library(darc_component
  src/owner.cpp
  src/primitive.cpp
  src/component.cpp
  src/component_manager.cpp
  src/thread_manager.cpp
  src/registry.cpp
)
target_link_libraries(darc_component ${catkin_LIBRARIES} -pthread -ldl)

# component_manager
find_package(Boost REQUIRED COMPONENTS program_options)
add_executable(darc_component_manager
  src/component_manager_executable.cpp
)
target_link_libraries(darc_component_manager darc_component ${Boost_LIBRARIES})

# Install
install(TARGETS darc_component_manager
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

# Test
add_subdirectory(test)

#add_subdirectory(pybindings)
