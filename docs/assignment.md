# 作业要求

用录好的 rosbag 建立三维地图，整理后导出二维地图，并比较不同参数的效果。建图可以使用已有算法，地图处理部分需要自己实现。

## 1. 用雷达和 IMU 建立三维地图

- 接入 FAST-LIO、Point-LIO 或其他适用算法，把不同时刻的点云拼成一张三维地图。同时输出位置和朝向（位姿）、运动轨迹，并支持将地图保存为 pcd 点云文件

## 2. 整理地图，生成二维地图

编写独立的地图处理模块，完成：

- 去掉无效点和明显偏离周围物体的杂点
- 将空间划成小格，每格保留代表点，减少点数，进行体素降采样
- 生成类似俯视图的二维网格地图，使用 ROS 的 `OccupancyGrid` 消息发布
- 保存二维地图的 pgm 图片和 yaml 配置

## 3. 显示结果

- 将三维点云地图和二维地图在 rviz 中显示出来，位置、方向、范围和每格代表的实际大小应保持一致。

可以调用 PCL 等库，但需要自己连接这些处理步骤，设计参数并完成保存功能。模板传来的是整张地图，别把每次收到的地图重复叠加。

## 提交要求

**三维地图和二维地图文件统一保存到 [maps](maps) 文件夹中**。三维地图应该是 pcd 文件，二维地图应该是一个 pgm 文件和一个对应的 yaml 配置文件

README 中写清怎么运行、怎么保存地图，完成了什么内容，贴一张 rviz 截图（展示你的三维地图和二维地图的对齐效果）

提交 github 链接到 2719850558@qq.com

不要交编译完的内容上来，不要交 rosbag 上来。

## 参考

- [Livox 驱动与消息接口](https://github.com/Livox-SDK/livox_ros_driver2)
- [FAST-LIO](https://github.com/hku-mars/FAST_LIO) / [Point-LIO](https://github.com/hku-mars/Point-LIO)：自行确认所用实现支持 ROS 2 Humble。
- [OccupancyGrid](https://github.com/ros2/common_interfaces/blob/humble/nav_msgs/msg/OccupancyGrid.msg) / [Nav2 地图读写](https://github.com/ros-navigation/navigation2/blob/humble/nav2_map_server/src/map_io.cpp)
