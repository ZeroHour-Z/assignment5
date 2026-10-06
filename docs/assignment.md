# 作业要求

用录好的 MID-360 数据建立三维地图，整理后导出二维地图，并比较不同参数的效果。建图可以使用已有算法，地图处理部分需要自己实现。

## 1. 用雷达和 IMU 建立三维地图

接入 FAST-LIO、Point-LIO 或其他适用算法，把不同时刻的点云拼成一张三维地图。同时输出位置和朝向（位姿）、运动轨迹，并支持将地图保存为 PCD 点云文件、重新读取。

你需要把录制的数据接到算法中，重点处理以下问题：

- **时间对齐**：使用消息中的采集时间，检查雷达与 IMU 的时间是否对应。回放时的 `--clock` 和节点的 `use_sim_time` 只是让程序使用回放时钟，不会自动消除两种传感器的时间偏差。
- **位置关系**：弄清雷达和 IMU 的安装位置、朝向以及坐标轴方向，这些关系称为外参。两个消息的 `frame_id` 名称相同，不代表传感器装在同一个位置；还需要核对 IMU 的单位。
- **运动造成的点云变形**：一帧中的点并非同时采集，雷达移动时需要利用逐点时间进行修正，称为去畸变。CustomMsg 的 `offset_time` 单位是纳秒，转换数据时不要丢掉；PointCloud2 不一定有逐点时间，缺少时说明限制。

程序还应处理启动时的数据准备、数据中断，以及回放跳回较早时间的情况。重新建图时清除上次的地图和轨迹。

## 2. 整理地图，生成二维地图

编写独立的地图处理模块，完成：

- 去掉无效点和明显偏离周围物体的杂点。
- 将空间划成小格，每格保留代表点，减少点数。这叫体素降采样。
- 区分地面和障碍物，生成类似俯视图的二维网格地图，使用 ROS 的 `OccupancyGrid` 消息发布。
- 保存整理后的 PCD，以及二维地图的 PGM 图片和 YAML 配置。重新读取后，位置、方向、范围和每格代表的实际大小应保持一致。

可以调用 PCL 等库，但需要自己连接这些处理步骤，设计参数并完成保存功能。模板传来的是整张地图，别把每次收到的地图重复叠加。

## 提交

提交 github 链接到 2719850558@qq.com，README 写清怎么运行、怎么保存地图，完成了什么部分。

不要交编译完的内容上来，不要交rosbag上来。

## 参考

- [Livox 驱动与消息接口](https://github.com/Livox-SDK/livox_ros_driver2)
- [FAST-LIO](https://github.com/hku-mars/FAST_LIO) / [Point-LIO](https://github.com/hku-mars/Point-LIO)：自行确认所用实现支持 ROS 2 Humble。
- [OccupancyGrid](https://github.com/ros2/common_interfaces/blob/humble/nav_msgs/msg/OccupancyGrid.msg) / [Nav2 地图读写](https://github.com/ros-navigation/navigation2/blob/humble/nav2_map_server/src/map_io.cpp)
