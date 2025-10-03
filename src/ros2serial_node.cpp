#include <rclcpp/rclcpp.hpp>

#include <ros2serial/ros_serial_bridge.hpp>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::executors::SingleThreadedExecutor exec;
    const auto ros2serial = std::make_shared<ros2serial::RosSerialBridge>(rclcpp::NodeOptions());
    exec.add_node(ros2serial);
    exec.spin();
    rclcpp::shutdown();
}