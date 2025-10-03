#pragma once

#include "ros2serial/serial_communication.hpp"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/float32.hpp>
#include "ros2serial/visibility_control.h"

using json = nlohmann::json;
using std::placeholders::_1;
using namespace std::chrono_literals;


namespace ros2serial
{

class RosSerialBridge : public rclcpp::Node
{
public:
    explicit RosSerialBridge(const rclcpp::NodeOptions& options); 

    virtual ~RosSerialBridge();
private:
    // parameters
    std::string serial_port_;
    int baudrate_;
    
    // subscriver /cmd_vel
    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr cmd_vel);

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;

    // publisher /encoder
    void timer_callback();

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr encoder_left_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr encoder_right_pub_;

    std::shared_ptr<SerialCommunication> serial_com_;
};

}  // namespace ros2serial
