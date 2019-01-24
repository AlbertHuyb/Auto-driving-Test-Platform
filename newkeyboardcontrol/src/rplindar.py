#!/usr/bin/env python
import tf
import rospy
from nav_msgs.msg import Odometry


br1 = tf.TransformBroadcaster()

def callback(msg):
    x=msg.position.x
    y=msg.position.y

    br1.sendTransform((x,y, 0),
                     msg.orientation,
                     rospy.Time.now(),
                     "base",
                     "odometry")

    rospy.loginfo('x: {}, y:{}, orientation_z:{},' .format(x,y, msg.orientation.z))

def main():
    rospy.init_node('tf2gmappingodom')
    rospy.Subscriber("/odom",Odometry.pose,callback)
    rospy.spin()

if __name__ == '__main__':
    main()