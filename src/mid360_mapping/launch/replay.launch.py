"""Start the scaffold and replay only the configured lidar/IMU topics."""
import math
from pathlib import Path
import yaml
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, IncludeLaunchDescription, OpaqueFunction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def setup(context):
    bag = Path(LaunchConfiguration('bag_path').perform(context)).expanduser().resolve()
    params = Path(LaunchConfiguration('params_file').perform(context)).expanduser().resolve()
    rate = float(LaunchConfiguration('rate').perform(context))
    if not bag.is_dir() or not (bag / 'metadata.yaml').is_file():
        raise RuntimeError('bag_path must be a ROS 2 bag directory containing metadata.yaml')
    if not math.isfinite(rate) or rate <= 0:
        raise RuntimeError('rate must be finite and positive')
    with params.open(encoding='utf-8') as stream:
        settings = yaml.safe_load(stream)['mapping_node']['ros__parameters']
    topics = [settings['lidar_topic'], settings['imu_topic']]
    if not all(isinstance(t, str) and t.startswith('/') and t != '/clock' for t in topics):
        raise RuntimeError('lidar_topic and imu_topic must be absolute names other than /clock')
    share = Path(get_package_share_directory('mid360_mapping'))
    command = ['ros2', 'bag', 'play', str(bag), '--clock', '60', '--rate', str(rate)]
    paused = LaunchConfiguration('start_paused').perform(context) == 'true'
    if paused:
        command.append('--start-paused')
    else:
        # Give the mapper a short startup window; use paused playback for explicit readiness.
        command += ['--delay', '3']
    command += ['--topics', *topics]
    return [
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(str(share / 'launch/mapping.launch.py')),
            launch_arguments={'params_file': str(params),
                              'rviz': LaunchConfiguration('rviz').perform(context),
                              'map_params_file': LaunchConfiguration('map_params_file').perform(context)}.items()),
        ExecuteProcess(cmd=command, output='screen', emulate_tty=True),
    ]


def generate_launch_description():
    share = Path(get_package_share_directory('mid360_mapping'))
    return LaunchDescription([
        DeclareLaunchArgument('bag_path', description='Absolute ROS 2 bag directory'),
        DeclareLaunchArgument('params_file', default_value=str(share / 'config/mid360_recorded.yaml')),
        DeclareLaunchArgument('map_params_file', default_value=str(share / 'config/map_processing.yaml')),
        DeclareLaunchArgument('rate', default_value='1.0'),
        DeclareLaunchArgument('start_paused', default_value='true', choices=['true', 'false']),
        DeclareLaunchArgument('rviz', default_value='true', choices=['true', 'false']),
        OpaqueFunction(function=setup),
    ])
