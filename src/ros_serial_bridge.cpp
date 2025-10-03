#include "ros2serial/ros_serial_bridge.hpp"
#include <rclcpp_components/register_node_macro.hpp>

namespace ros2serial
{


RosSerialBridge::RosSerialBridge(const rclcpp::NodeOptions& options)
: Node("ros_serial_bridge_node", options)
{
    // パラメータ設定
    this->declare_parameter<std::string>("serial_port", "/dev/ttyACM0");
    this->declare_parameter<int>("baudrate", 115200);

    serial_port_ = this->get_parameter("serial_port").as_string();
    baudrate_ = this->get_parameter("baudrate").as_int();

    // serial communication start
    serial_com_ = std::make_shared<SerialCommunication>(serial_port_, baudrate_);

    // subscription /cmd_vel
    cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>("/cmd_vel",
        10, std::bind(&RosSerialBridge::cmd_vel_callback, this, _1));
    
    // publisher /encoder
    encoder_left_pub_ = this->create_publisher<std_msgs::msg::Float32>("/encoder/left", 10);
    encoder_right_pub_ = this->create_publisher<std_msgs::msg::Float32>("/encoder/right", 10);

    timer_ = this->create_wall_timer(
        10ms, bind(&RosSerialBridge::timer_callback, this));

}

RosSerialBridge::~RosSerialBridge()
{
}

void RosSerialBridge::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr cmd_vel)
{
    // initialize json object
    json send_data = serial_com_->send_data;
    // 小数点以下3桁に丸める
    send_data["linear_x"] = std::round(cmd_vel->linear.x * 1000.0) / 1000.0;
    send_data["angular_z"] = std::round(cmd_vel->angular.z * 1000.0) / 1000.0;

    std::string send_serial = send_data.dump() + "\n";
    serial_com_->sendSerialData(send_serial);
    std::cout << "sended:" << send_serial << std::endl;
}


void RosSerialBridge::timer_callback()
{
    // json deserialize proccess
    // initialize json object
    json receive_data = serial_com_->receive_data;
    std::string init_data = receive_data.dump() + "\n";
    std::string recv_data = serial_com_->receiveSerialData(init_data);
    if (recv_data.empty()) {
        // 未受信時は発行せずに抜ける（前回値を保持したい場合はクラス状態に保存して使う）
        return;
    }
    json decoded_data = serial_com_->decodeSerialData(recv_data, init_data);

    auto encoder_left_msg = std_msgs::msg::Float32();
    auto encoder_right_msg = std_msgs::msg::Float32();

    encoder_left_msg.data = decoded_data["encoder_left"];
    encoder_right_msg.data = decoded_data["encoder_right"];

    encoder_left_pub_->publish(encoder_left_msg);
    encoder_right_pub_->publish(encoder_right_msg);
}

}// namespace ros2serial

RCLCPP_COMPONENTS_REGISTER_NODE(ros2serial::RosSerialBridge)