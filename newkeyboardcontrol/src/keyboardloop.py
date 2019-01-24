#!/usr/bin/env python
# -*- coding: utf-8 -*

import sys
import tty, termios
import rospy.timer
import threading
import time

import rospy
from geometry_msgs.msg import Twist

# 全局变量
pub = rospy.Publisher('/cmd_vel', Twist,queue_size=10)

ch = 'q'
global_vel = Twist()

speed=0
turn=0

def keyboardLoop():
    rate = rospy.Rate(rospy.get_param('~hz', 1))

    # 速度变量
    walk_vel_ = rospy.get_param('walk_vel', 0.02)
    yaw_rate_ = rospy.get_param('yaw_rate', 0.02)

    max_tv = walk_vel_
    max_rv = yaw_rate_

    # 显示提示信息
    print "Reading from keyboard"
    print "Use WASD keys to control the robot"
    print "Press Caps to move faster"
    print "Press Q to quit"

    # 读取按键循环
    while not rospy.is_shutdown():
        #键盘输入读取
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        # 不产生回显效果
        old_settings[3] = old_settings[3] & ~termios.ICANON & ~termios.ECHO
        try:
            # print "wait for keyboard"
            tty.setraw(fd)
            temp_ch = sys.stdin.read(1)
	    ch = temp_ch
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        if ch == 'w':
            max_tv = walk_vel_
            speed = 3
            turn = 0
        elif ch == 's':
            max_tv = walk_vel_
            speed = -3
            turn = 0
        elif ch == 'a':
            max_rv = yaw_rate_
            speed = 0
            turn = 3
        elif ch == 'd':
            max_rv = yaw_rate_
            speed = 0
            turn = -3

        elif ch == 'q':
            turn = 0
            speed = 0

        elif ch == 'W':
            speed = 5
            turn=0
        elif ch == 'S':
            speed = -5
            turn=0
        elif ch == 'A':
            turn = 5
            speed=0
        elif ch == 'D':
            turn = -5
            speed=0


        elif ch=='Q':
            exit()



        else:
            max_tv = walk_vel_
            max_rv = yaw_rate_

	global_vel.linear.x=speed*max_tv
	global_vel.angular.z=turn*max_rv
	print "speed: ",speed
	print "turn: ",turn

        rate.sleep()

	pub.publish(global_vel)
	lock.release()
        rospy.sleep(2)
        temp = global_vel
        global_vel.linear.x=0
        global_vel.angular.z=0
        pub.publish(global_vel)
        rospy.sleep(2)
        global_vel = temp



if __name__ == '__main__':

    rospy.init_node('moveaswill')
    global_vel.linear.x=0
    global_vel.linear.y=0
    global_vel.linear.z=0

    global_vel.angular.x=0
    global_vel.angular.y=0
    global_vel.angular.z=0

    keyboardLoop()
