"""Start the mapping scaffold independently of rosbag playback."""
from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    share = Path(get_package_share_directory('mid360_mapping'))
    return LaunchDescription([
        DeclareLaunchArgument('params_file', default_value=str(share / 'config/mid360_recorded.yaml')),
        DeclareLaunchArgument('map_params_file', default_value=str(share / 'config/map_processing.yaml')),
        DeclareLaunchArgument('rviz', default_value='true', choices=['true', 'false']),
        Node(package='mid360_mapping', executable='mapping_node', name='mapping_node',
             output='screen', parameters=[LaunchConfiguration('params_file'), {'use_sim_time': True}]),
        Node(package='mid360_mapping', executable='map_processor_node', name='map_processor_node',
             output='screen', parameters=[LaunchConfiguration('map_params_file'), {'use_sim_time': True}]),
        Node(package='rviz2', executable='rviz2', output='screen',
             arguments=['-d', str(share / 'rviz/mapping.rviz')],
             parameters=[{'use_sim_time': True}], condition=IfCondition(LaunchConfiguration('rviz'))),
    ])
