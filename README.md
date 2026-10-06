# RoboMaster assignment 5 Nav

使用录好的雷达和 IMU 数据（rosbag），**建立三维地图、整理地图并导出二维地图**，最后比较不同参数的效果。具体见 [作业要求](docs/assignment.md)。

模板已经搭好两个节点和启动文件，你需要补全标有 TODO 的部分，让它们真正完成建图、地图处理和保存。开发环境为 **Ubuntu 22.04 / ROS 2 Humble**。

## 数据

将完整数据目录放在 `data/rosbag2_2026_05_28-17_11_13/`，包含 `metadata.yaml` 和 `.db3` 文件

数据时长约 104 秒。点云记录周围物体的位置，IMU 记录角速度和加速度

| 话题 | 类型 |
|---|---|
| `/livox/lidar_192_168_1_140` | `livox_ros_driver2/msg/CustomMsg` |
| `/livox/imu_192_168_1_140` | `sensor_msgs/msg/Imu` |

## 依赖

### 1. Livox-SDK2

按 [SDK 官方说明](https://github.com/Livox-SDK/Livox-SDK2#2-installation) 编译安装。它是下一步编译完整 Livox 驱动包所需的底层库。

### 2. livox_ros_driver2

这份 bag 使用 `livox_ros_driver2/msg/CustomMsg`。安装这个包后，ROS 才能识别并回放该消息

使用独立的 `~/ws_livox` 工作空间，因为官方构建脚本会清理所在工作空间的构建产物。下面先准备 ROS 2 的依赖清单，再安装依赖、执行 [Humble 构建脚本](https://github.com/Livox-SDK/livox_ros_driver2#23-build-the-livox-ros-driver-2)。

```bash
mkdir -p ~/ws_livox/src
cd ~/ws_livox/src
git clone https://github.com/Livox-SDK/livox_ros_driver2.git
cd livox_ros_driver2
cp package_ROS2.xml package.xml
source /opt/ros/humble/setup.bash
rosdep install --from-paths . --ignore-src -r -y --rosdistro humble
./build.sh humble
source ~/ws_livox/install/setup.bash

# 能显示消息字段，说明 ROS 已找到 CustomMsg
ros2 interface show livox_ros_driver2/msg/CustomMsg
```

**建图算法需要另外接入。** Livox 驱动本身不会建图，模板也未包含 FAST-LIO 或 Point-LIO。选择支持 Humble 的实现，按它的说明安装额外依赖，并将新增依赖写入相应功能包的 `package.xml` 和构建配置。PCL、Eigen 等库是否需要，由你的实现决定。

每次打开新终端，在本仓库根目录依次加载以下环境，再启动节点或回放数据：

```bash
source /opt/ros/humble/setup.bash
source ~/ws_livox/install/setup.bash
source install/setup.bash
```

使用 zsh 时将上述 `setup.bash` 换成 `setup.zsh`

## 建议

| 文件 | 需要完成的内容 |
|---|---|
| [mapping_node.cpp](src/mid360_mapping/src/mapping_node.cpp) | 把雷达和 IMU 数据接入建图算法，输出地图与运动轨迹 |
| [map_processor_node.cpp](src/mid360_mapping/src/map_processor_node.cpp) | 去掉杂点、减少点数，生成并保存二维地图 |

将代码推送到你的 Fork, 然后提交仓库链接到 2719850558@qq.com，格式为：第三次作业-班级-姓名（第三次作业-自动化2305-周湛昊）

完成后更新本 README，提交链接，具体见 [作业要求](docs/assignment.md)。

## 在这里解释你的项目

例如：

如何编译：

运行方式：

[截图]()