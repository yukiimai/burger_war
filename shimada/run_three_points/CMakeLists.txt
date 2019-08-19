cmake_minimum_required(VERSION 2.8.3)
project(run_three_points)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  nav_msgs
)

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES run_three_points
  CATKIN_DEPENDS roscpp rospy std_msgs nav_msgs
#  DEPENDS system_lib
)
include_directories( include
  ${catkin_INCLUDE_DIRS}
)

add_executable(run_three_points src/run_three_points.cpp)
target_link_libraries(run_three_points ${catkin_LIBRARIES})
add_dependencies(run_three_points ${catkin_EXPORTED_TARGETS})
#add_library(${PROJECT_NAME})

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)

target_link_libraries(run_three_points
  ${catkin_LIBRARIES}
  ${RPI_LIBRARIES} 
)

install(TARGETS run_three_points
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
