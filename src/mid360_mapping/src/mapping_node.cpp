#include "mid360_mapping/mapping_node.hpp"
#include <chrono>
#include <stdexcept>
#include <string>
#include <rcl_interfaces/msg/parameter_descriptor.hpp>

namespace mid360_mapping
{
MappingNode::MappingNode() : Node("mapping_node")
{
  rcl_interfaces::msg::ParameterDescriptor startup;
  startup.read_only = true;
  startup.description = "Startup setting; restart the node after changing configuration";
  const auto input_type = declare_parameter<std::string>("input_type", "pointcloud2", startup);
  const auto lidar_topic = declare_parameter<std::string>("lidar_topic", "/livox/lidar", startup);
  const auto imu_topic = declare_parameter<std::string>("imu_topic", "/livox/imu", startup);
  declare_parameter<std::string>("map_frame", "map", startup);
  declare_parameter<std::string>("body_frame", "body", startup);
  declare_parameter<std::string>("map_output_path", "", startup);
  if (lidar_topic.empty() || imu_topic.empty()) {
    throw std::invalid_argument("lidar_topic and imu_topic must not be empty");
  }

  if (input_type == "pointcloud2") {
    cloud_sub_ = create_subscription<sensor_msgs::msg::PointCloud2>(
      lidar_topic, rclcpp::SensorDataQoS().keep_last(10),
      [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) {on_cloud(msg);});
  } else if (input_type == "livox_custom") {
    // A generic subscription avoids a compile-time Livox SDK dependency in the scaffold.
    // The livox_ros_driver2 message type support must still be installed and sourced.
    custom_sub_ = create_generic_subscription(
      lidar_topic, "livox_ros_driver2/msg/CustomMsg", rclcpp::SensorDataQoS().keep_last(10),
      [this](std::shared_ptr<rclcpp::SerializedMessage> msg) {on_custom_cloud(msg);});
  } else {
    throw std::invalid_argument("input_type must be pointcloud2 or livox_custom");
  }
  imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
    imu_topic, rclcpp::SensorDataQoS().keep_last(1000),
    [this](sensor_msgs::msg::Imu::ConstSharedPtr msg) {on_imu(msg);});

  // These are output contracts, not an implemented mapping pipeline.
  map_pub_ = create_publisher<sensor_msgs::msg::PointCloud2>("mapping/map", rclcpp::SensorDataQoS());
  registered_pub_ = create_publisher<sensor_msgs::msg::PointCloud2>(
    "mapping/cloud_registered", rclcpp::SensorDataQoS());
  odom_pub_ = create_publisher<nav_msgs::msg::Odometry>("mapping/odom", 10);
  path_pub_ = create_publisher<nav_msgs::msg::Path>("mapping/path", 10);
  save_service_ = create_service<std_srvs::srv::Trigger>(
    "mapping/save_map",
    [](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
      std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
      // TODO: Save the accumulated map to map_output_path and report the actual result.
      response->success = false;
      response->message = "Scaffold only: map storage is not implemented";
    });
  status_timer_ = create_wall_timer(std::chrono::seconds(5), [this]() {
    RCLCPP_INFO(
      get_logger(), "Scaffold only: clouds=%llu imu=%llu; no map or pose is produced",
      static_cast<unsigned long long>(cloud_count_),
      static_cast<unsigned long long>(imu_count_));
  });
  RCLCPP_WARN(get_logger(), "Implement the mapping TODOs before using this as a mapper");
}

void MappingNode::on_cloud(sensor_msgs::msg::PointCloud2::ConstSharedPtr message)
{
  ++cloud_count_;
  (void)message;
  // TODO: Read actual point fields and units, including per-point acquisition times.
  // TODO: Connect to your chosen mapping backend; preserve sensor timestamps and frames.
}

void MappingNode::on_custom_cloud(std::shared_ptr<rclcpp::SerializedMessage> message)
{
  ++cloud_count_;
  (void)message;
  // TODO: Replace serialized-message reception with a typed Livox input adapter.
  // Declare livox_ros_driver2 in package.xml and CMake when using its C++ message type.
  // TODO: Preserve timebase, offset_time, reflectivity, tag and line as needed by your backend.
}

void MappingNode::on_imu(sensor_msgs::msg::Imu::ConstSharedPtr message)
{
  ++imu_count_;
  (void)message;
  // TODO: Check the recorded units/frame and handle the IMU data in your mapping backend.
  // TODO: Publish estimated poses, transforms, registered clouds and an accumulated map.
  // Counts alone are not synchronization, deskewing or mapping.
}
}  // namespace mid360_mapping
