#!/bin/bash
export ROS_DOMAIN_ID=24
source /opt/ros/humble/setup.bash
source /home/msd700/ros2_ws/install/setup.bash
# set launch file auto start 
ros2 launch ros2serial ros2serial_teleop_joy_launch.xml