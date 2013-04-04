cmake_minimum_required(VERSION 2.8)

add_library(test_publisher_component manual/test_publisher_component.cpp)
target_link_libraries(test_publisher_component darc_component)

add_library(test_subscriber_component manual/test_subscriber_component.cpp)
target_link_libraries(test_subscriber_component darc_component)

#catkin_add_gtest(darc_component_gtest_pubsub test/publisher_gtest.cpp)
#target_link_libraries(darc_component_publisher_gtest
#  ${GTEST_BOTH_LIBRARIES}
#  darc_component)
