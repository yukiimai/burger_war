cmake_minimum_required(VERSION 2.8.3)
project(enemy_position)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  gazebo_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  geometry_msgs
  cv_bridge
  )

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp gazebo_msgs tf2 tf2_ros tf2_geometry_msgs geometry_msgs
)

find_package( OpenCV REQUIRED )


include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  src/${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
 ${OpenCV_LIBRARIES}
  ptr_msgs_generate_messages_cpp
)

install(DIRECTORY
  include
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(
  TARGETS
    ${PROJECT_NAME}
  RUNTIME DESTINATION
    ${CATKIN_PACKAGE_BIN_DESTINATION}
)

