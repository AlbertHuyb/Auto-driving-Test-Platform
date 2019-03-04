#!/usr/bin/env python
# -*- coding: utf-8 -*

import sys
import tty, termios
import rospy.timer
import rospy
from geometry_msgs.msg import Twist
import threading

# 全局变量
rospy.init_node('smartcar_teleop') # 初始化
pub = rospy.Publisher('sub1/cmd_vel', Twist, queue_size=10) # 发布主题
global_vel = Twist() # 速度
rate = rospy.Rate(2) # 更新频率2Hz，可以将频率调大以进行精细调节   
walk_vel_ = rospy.get_param('walk_vel', 0.02) # 前进速度
yaw_rate_ = rospy.get_param('yaw_rate', 0.02) # 转动速度
ch = '0' # 挡位字符
lock = threading.Lock() # 线程锁

# 键盘读入函数
def keyboard():
    global ch
    while not rospy.is_shutdown():
        # I DONT KNOW WHAT THEY MEAN，把读到的字符存到ch中
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        old_settings[3] = old_settings[3] & ~termios.ICANON & ~termios.ECHO
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

	
# 运动函数
def motion():
	global global_vel, rate, walk_vel_, yaw_rate_
	while not rospy.is_shutdown(): 
		if ch == 'o':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/rotation.txt', 'r')
		elif ch == 'w':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/w.txt', 'r')
		elif ch == 'a':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/a.txt', 'r')
		elif ch == 's':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/s.txt', 'r')
		elif ch == 'e':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/escape.txt', 'r')
		elif ch == 'd':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/d.txt', 'r')
		elif ch == '1':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/youth_ave.txt', 'r')
		elif ch == '2':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/youth_res.txt', 'r')
		elif ch == '3':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/youth_slu.txt', 'r')
		elif ch == 't':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/youth_side_ave.txt', 'r')
		elif ch == 'c':
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/cat_ave.txt', 'r')
		else:
			f = open('/home/ros/multifunction_test/src/dsrc_move/src/stop.txt', 'r')
		speed_list = f.readlines()
		for i in speed_list:
	    # 根据速度文件改变速度
			para = i.split(',')
			speed = float(para[0])
			turn = float(para[1])
	    # 运动速度和角速度
			global_vel.linear.x = speed * walk_vel_
			global_vel.angular.z = turn * yaw_rate_
			pub.publish(global_vel)
			rate.sleep()
		f.close()

if __name__ == '__main__':
    # 初始化
    global_vel.linear.x=0 
    global_vel.linear.y=0
    global_vel.linear.z=0
    global_vel.angular.x=0
    global_vel.angular.y=0
    global_vel.angular.z=0
    
    # 设置线程
    t1 = threading.Thread(target = keyboard)
    t2 = threading.Thread(target = motion)

    # 启动线程
    t1.start()
    t2.start()


    
