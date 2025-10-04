# ros2serial
## install
```bash
git clone https://github.com/tomoswifty/ros2serial.git
cd ~/ros2_ws
colcon build --symlink-install --packages-select ros2serial
```

## How to use
```bash
ros2 run ros2serial ros2serial
# with teleop-twist-joy
## pare DualSense controler before launch
ros2 launch ros2serial ros2serial_teleop_joy_launch.xml 
```
The console now you can see sended data via serial.
If you want see the published topic from received serial.
```bash
# Verify the existence of following topics
ros2 topic list
## /encoder/left
## /encoder/right
ros2 topic echo /encoder/left
# open another terminal
ros2 topic echo /encoder/right
```

## set USB device with udev.rules
copy file to root directory
```bash
# ファイルをコピー
sudo cp ros2serial/99-arduino-uno-r4.rules /etc/udev/rules.d
# 設定の反映
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## set service for auto start launch
copy service file to root directory
```bash
# launch ファイルのシェルスクリプトに実行権限を付与
sudo chmod +x ros2serial/msd350-teleop-joy.sh
## このような出力があればok
ll ros2serial/msd350-teleop-joy.sh 
-rwxrwxr-x 1 msd700 msd700 202  7月  3 16:39 ros2serial/msd350-teleop-joy.sh*

# ファイルをコピー
sudo cp ros2serial/msd350-teleop-joy.service /etc/systemd/system
```

how to use service 
```bash
# リロード
sudo systemctl daemon-reload
# 有効化
sudo systemctl enable msd350-teleop-joy.service
# 開始 ###
sudo systemctl start msd350-teleop-joy.service
# 確認
sudo systemctl status msd350-teleop-joy.service
# 停止
sudo systemctl stop msd350-teleop-joy.service
# 無効化
sudo systemctl disable msd350-teleop-joy.service
```