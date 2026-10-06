#pragma once

#include <cstdint>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/generic_subscription.hpp>
#include <rclcpp/serialized_message.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/path.hpp>
#include <std_srvs/srv/trigger.hpp>

namespace mid360_mapping
{
class MappingNode : public rclcpp::Node
{
public:
  MappingNode();

private:
  void on_cloud(sensor_msgs::msg::PointCloud2::ConstSharedPtr message);
  void on_custom_cloud(std::shared_ptr<rclcpp::SerializedMessage> message);
  void on_imu(sensor_msgs::msg::Imu::ConstSharedPtr message);

  std::uint64_t cloud_count_{0};
  std::uint64_t imu_count_{0};
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr cloud_sub_;
  rclcpp::GenericSubscription::SharedPtr custom_sub_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr map_pub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr registered_pub_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr save_service_;
  rclcpp::TimerBase::SharedPtr status_timer_;
};
}  // namespace mid360_mapping
