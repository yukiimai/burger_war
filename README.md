# midori game team burger_war
ロボットで戦車対戦をするゲームです。
大砲で撃つ代わりに、カメラでターゲットのARマーカーを読み取ります。<BR>

## 追加インストール
なし

## make
cd ~/catkin_ws
catkin_make

※install には対応していません！！！！！

## 実行
buger_war/launch/your_burger.launch
をオリジナルに書き換えています。
サンプルと同様にns入でinclude起動してください。

## 動作確認環境
- Ubuntu 16.04 
- Ros kinetic
2018年からkineticで開発しています｡

##gazeboで起動でerror

[ INFO] [1566222710.759609830]: Finished loading Gazebo ROS API Plugin.
[ INFO] [1566222710.760108475]: waitForService: Service [/gazebo/set_physics_properties] has not been advertised, waiting...
[ INFO] [1566222710.878206037]: Finished loading Gazebo ROS API Plugin.
[ INFO] [1566222710.878632161]: waitForService: Service [/gazebo/set_physics_properties] has not been advertised, waiting...
Killed
[gazebo_gui-2] process has died [pid 13701, exit code 137, cmd /opt/ros/kinetic/lib/gazebo_ros/gzclient __name:=gazebo_gui __log:=/home/first/.ros/log/6f1c7792-c285-11e9-bca1-fbb7c1c61faa/gazebo_gui-2.log].
log file: /home/first/.ros/log/6f1c7792-c285-11e9-bca1-fbb7c1c61faa/gazebo_gui-2*.log
Error [parser.cc:581] Unable to find uri[model://my_ground_plane]
Error [parser.cc:581] Unable to find uri[model://center_box]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://corner_box]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://corner_box]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://corner_box]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://corner_box]
Error [parser.cc:581] Unable to find uri[model://plate]
Error [parser.cc:581] Unable to find uri[model://plate]

と表示されて落ちる。メンバー４名全員のPCで確認

