import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry, Path
from geometry_msgs.msg import PoseStamped, TransformStamped
from tf2_ros import TransformBroadcaster
from sensor_msgs.msg import Imu
import math

def euler_to_quaternion(yaw):
    return [0.0, 0.0, math.sin(yaw / 2.0), math.cos(yaw / 2.0)]

class PathVisualizer(Node):
    def __init__(self):
        super().__init__('path_tracker')
        self.path_pub = self.create_publisher(Path, '/car_path', 10)
        self.tf_broadcaster = TransformBroadcaster(self)
        
        # Path object to store history
        self.path = Path()
        self.path.header.frame_id = 'odom'
        
        # Robot State (Starting at 0,0,0)
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0 
        self.last_wheel_count = 0.0

        self.file = open('capture.txt', 'r')
        self.timer = self.create_timer(0.1, self.update_path) # 10Hz update

    def update_path(self):
        line = self.file.readline()
        if not line:
            return

        try:
            parts = line.split()
            gyro_z_raw = float(parts[2])
            current_wheel_count = float(parts[3])

            delta_dist = (current_wheel_count - self.last_wheel_count) * 0.8
            self.last_wheel_count = current_wheel_count

            dt = 0.1 
            gyro_z_rad = (gyro_z_raw * (math.pi / 180.0)) #Convert to rad/s
            self.th += gyro_z_rad * dt
            self.x += delta_dist * math.cos(self.th)
            self.y += delta_dist * math.sin(self.th)

            now = self.get_clock().now().to_msg()
            
            #Broadcast TF (Moves the robot "base_link" in RViz)
            t = TransformStamped()
            t.header.stamp = now
            t.header.frame_id = 'odom'
            t.child_frame_id = 'base_link'
            t.transform.translation.x = self.x
            t.transform.translation.y = self.y
            t.transform.rotation.z = math.sin(self.th / 2.0)
            t.transform.rotation.w = math.cos(self.th / 2.0)
            self.tf_broadcaster.sendTransform(t)

            #Update path line
            pose = PoseStamped()
            pose.header.stamp = now
            pose.header.frame_id = 'odom'
            pose.pose.position.x = self.x
            pose.pose.position.y = self.y
            self.path.poses.append(pose)
            self.path_pub.publish(self.path)

        except (ValueError, IndexError):
            pass

def main():
    rclpy.init()
    node = PathVisualizer()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()