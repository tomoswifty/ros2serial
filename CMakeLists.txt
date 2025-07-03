cmake_minimum_required(VERSION 3.8)
project(ros2serial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake_auto REQUIRED)

ament_auto_find_build_dependencies()

# ament_auto_add_library(pubsub src/pubsub.cpp)
# rclcpp_components_register_nodes(pubsub "ros2serial::PubSub")
# ament_auto_add_library(serial src/serial_com.cpp)
# rclcpp_components_register_nodes(pubsub "ros2serial::PubSub")

# ament_auto_add_executable(pubsub_node
#  src/ros2serial_node.cpp
#  src/pubsub.cpp
# )

ament_auto_add_library(ros2serial src/ros_serial_bridge.cpp)
ament_auto_add_library(serial_com src/serial_communication.cpp)
rclcpp_components_register_nodes(ros2serial "ros2serial::RosSerialBridge")

ament_auto_add_executable(ros2serial_node
 src/ros2serial_node.cpp
 src/serial_communication.cpp
 src/ros_serial_bridge.cpp
)



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package(USE_SCOPED_HEADER_INSTALL_DIR)