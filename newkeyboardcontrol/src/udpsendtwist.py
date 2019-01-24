#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist

import time  # provides time functions
import socket  # provides socket functions
import sys  # system exception handling


#####################
# Global settings


#####################
# Local routines
target=("ip_address",4040)



def udpinit(mk1_addr,  # IP Address of MK1 Radio
             mk1_port=4040,  # Port of MK1 Radio
            ):
    print "Test source to MK1 on %s:%d" % (mk1_addr, mk1_port)

    txsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # create socket
    txsock.bind(('', 50000))  # bind to whatever port we can get our hands on
    txsock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    global target
    target = (mk1_addr, mk1_port)

    return txsock


def callback(data,txsock):
    # print "enter call back"
    pbuf = "linearx "+ str(data.linear.x)
    txsock.sendto(pbuf,target)
    # print "sendto linearx:",data.linear.x
    # print "txsock: ",txsock
    pbuf = "angularz "+ str(data.angular.z)
    txsock.sendto(pbuf,target)



def listener():
    rospy.init_node('udp_send_twist', anonymous=True)
    mkx_ip=rospy.get_param("~rsu_ip","192.168.1.101")
    mkx_port=rospy.get_param("~rsu_port",4040)

    txsock=udpinit(mkx_ip,mkx_port)

    rospy.Subscriber("/sub1/cmd_vel", Twist, callback,txsock)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':

    listener()
