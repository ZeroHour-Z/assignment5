#include <exception>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "mid360_mapping/mapping_node.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  int result = 0;
  try {
    rclcpp::spin(std::make_shared<mid360_mapping::MappingNode>());
  } catch (const std::exception & error) {
    RCLCPP_ERROR(rclcpp::get_logger("mapping_node"), "%s", error.what());
    result = 1;
  }
  rclcpp::shutdown();
  return result;
}
