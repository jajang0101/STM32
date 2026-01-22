import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry, Path
from geometry_msgs.msg import PoseStamped, TransformStamped
from tf2_ros import TransformBroadcaster
from sensor_msgs.msg import Imu
import math

def euler_to_quaternion(yaw):
    return [0.0, 0.0, math.sin(yaw / 2.0), math.cos(yaw / 2.0)]

class PathTracker(Node):
    def __init__(self):
        super().__init__('path_tracker')
        self.odom_pub = self.create_publisher(Odometry, '/odom', 10)
        self.path_pub = self.create_publisher(Path, '/robot_path', 10)
        self.tf_broadcaster = TransformBroadcaster(self) # Initialize TF
        
        self.path = Path()
        self.path.header.frame_id = 'odom'
        
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0 
        
        self.file = open('sensor_data.txt', 'r')
        self.timer = self.create_timer(0.1, self.update_path)

    def update_path(self):
        line = self.file.readline()
        if not line: return

        # 1. Parse your data
        # Format: Accel_X Accel_Y Gyro_Z WheelCount
        parts = line.split()
        gyro_z = float(parts[2])
        wheel_count = float(parts[3])

        # 2. Kinematics Integration
        # Assume 0.1s time step (matching your timer)
        dt = 0.1 
        v = wheel_count * 0.05 # Velocity (meters per count)
        vth = gyro_z           # Angular velocity (radians/s)

        self.th += vth * dt
        self.x += (v * dt) * math.cos(self.th)
        self.y += (v * dt) * math.sin(self.th)

        q = euler_to_quaternion(self.th)

        # 4. BROADCAST TF (Allows RViz to move the robot model)
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'base_link'
        t.transform.translation.x = self.x
        t.transform.translation.y = self.y
        t.transform.rotation.z = q[2]
        t.transform.rotation.w = q[3]
        self.tf_broadcaster.sendTransform(t)

        # 5. PUBLISH ODOMETRY MESSAGE
        odom = Odometry()
        odom.header = t.header
        odom.child_frame_id = 'base_link'
        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        odom.pose.pose.orientation.z = q[2]
        odom.pose.pose.orientation.w = q[3]
        # Optionally set velocities
        odom.twist.twist.linear.x = v
        odom.twist.twist.angular.z = vth
        self.odom_pub.publish(odom)

        imu_msg = Imu()
        imu_msg.header = t.header # same header as odom
        # parts[0] is Accel X, parts[1] is Accel Y
        imu_msg.linear_acceleration.x = float(parts[0]) * 9.81 # Convert Gs to m/s^2
        imu_msg.linear_acceleration.y = float(parts[1]) * 9.81
        imu_msg.angular_velocity.z = float(parts[2]) # Gyro Z

        self.imu_pub.publish(imu_msg)

        # 6. UPDATE PATH VISUALIZATION
        pose = PoseStamped()
        pose.header = t.header
        pose.pose = odom.pose.pose
        self.path.poses.append(pose)
        self.path_pub.publish(self.path)