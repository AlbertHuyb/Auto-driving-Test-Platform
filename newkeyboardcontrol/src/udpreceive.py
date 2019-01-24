#!/usr/bin/env python

import time  # provides time functions
import socket  # provides socket functions
import sys  # system exception handling

import rospy
from geometry_msgs.msg import Twist

#####################
# Global settings

puv_vel=rospy.Publisher("sub2/cmd_vel", Twist)
global_vel = Twist()



#####################
# Main routines

# -------------------------------------------
# Packet Sink
def TestPacketSink(mk1_port=500001  # Port on which to listen to MK1 Radio
                   ):
    """
    Test packet sink thread.

    This requires that the utility 'fwd_wsmp_forward_rx' is running on
    the target radio.

    Arguments:
    mk1_port - the port on which to listen for packets from the MK1 Radio
               e.g. 500001
    """

    print "sink running on port %d - Press CTRL-C to terminate\n" % mk1_port

    UpdatePeriod = 1.0  # process loop at least ever 1 second.

    MAX_PKT_LEN = 5000

    # set up the socket to receive packets
    rxsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # create socket
    rxsock.bind(('', mk1_port))  # bind to the MK1 using  port
    # Increase the UDP RX buffer space
    # an ABSOLUTE MUST to prevent packet loss due to process scheduling etc
    # 65536 seems to give good results - up to 500 pkt/sec 1470 byte pkts.
    rxsock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 655360)

    pkt_num = 0

    try:
        while 1:
            # Receive UDP packet
            rxpkt = rxsock.recvfrom(MAX_PKT_LEN)
            print rxpkt[0]
            data = rxpkt[0]
            if data[0:7]=="linearx":
                print data[8:]
                global_vel.linear.x=float(data[8:])
            elif data[0:8]=="angularz":
                global_vel.angular.z=float(data[9:])
                puv_vel.publish(global_vel)
            else:
                print "receiver wrong message"

    except KeyboardInterrupt:
        rxsock.close()
        print 'User CTRL-C, stopping packet sink'
    except:
        rxsock.close()
        print "Got exception:", sys.exc_info()[0]
        raise


# ------------------------------------------------------------------------------
# Main Program
# ------------------------------------------------------------------------------
if __name__ == '__main__':
    print ''
    print '*********************************************************************'
    print 'NOTE: fwdwsm should not be used for testing the throughput of an MKx.'
    print '      Instead the runtest_iperf_[r|t]x.sh scripts on the MKx itself'
    print '      should be used for throughput testing.'
    print '*********************************************************************'
    print ''

    rospy.init_node("receive_udp", anonymous=True)
    mk1_port=rospy.get_param("~receive_port",4040)
    TestPacketSink(mk1_port)
