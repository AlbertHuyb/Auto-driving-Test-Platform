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

global_vel = Twist()
lock = threading.Lock()

speed=0
turn=0

def keyboardLoop():
    rate = rospy.Rate(rospy.get_param('~hz', 1))

    # 速度变量
    walk_vel_ = rospy.get_param('walk_vel', 0.02)
    yaw_rate_ = rospy.get_param('yaw_rate', 0.02)

    max_tv = walk_vel_
    max_rv = yaw_rate_

    #speed=0
    #turn=0

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
            ch = sys.stdin.read(1)
            # print "already read sth"
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

            print ""
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

        lock.acquire()
        try:
            global_vel.linear.x=speed*max_tv
            global_vel.angular.z=turn*max_rv
            print "speed: ",speed
            print "turn: ",turn
        finally:
            lock.release()

        rate.sleep()
        # 停止机器人
        # stop_robot()


def pub_vel():
    rate = rospy.Rate(2)
    while not rospy.is_shutdown():
        global global_vel
        lock.acquire()
        pub.publish(global_vel)
        global_vel.linear.x=0
        global_vel.angular.z=0
        lock.release()
        rospy.sleep(3)






if __name__ == '__main__':

    # keyboardLoop()
    rospy.init_node('moveaswill')
    global_vel.linear.x=0
    global_vel.linear.y=0
    global_vel.linear.z=0

    global_vel.angular.x=0
    global_vel.angular.y=0
    global_vel.angular.z=0

    t1=threading.Thread(target=pub_vel)
    t2=threading.Thread(target=keyboardLoop)
    t1.start()
    t2.start()
