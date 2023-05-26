#! /usr/bin/python3
import serial
import rospy
from geometry_msgs.msg import Twist
from time import *


def map_to_pwm(vel):
    # convert to pwm value
    pwm = int(vel * 100)

    if pwm == 0:
        return pwm
    elif pwm < 0:
        if pwm < -PWMRANGE:
            return -PWMRANGE
        else:
            return min(pwm, -PWM_MIN)
    else:
        if pwm > PWMRANGE:
            return PWMRANGE
        else:
            return max(pwm, PWM_MIN)

def run_cmd_callback(data):
    rec_msg = data
    linear_vel = rec_msg.linear.x
    angular_vel = rec_msg.angular.z

    left_wheel_vel = linear_vel - (angular_vel * WHEEL_DISTANCE / 2)
    right_wheel_vel = linear_vel + (angular_vel * WHEEL_DISTANCE / 2)

    # Map wheel velocities to PWM values
    left_pwm = map_to_pwm(left_wheel_vel)
    right_pwm = map_to_pwm(right_wheel_vel)

    if left_pwm == -right_pwm:
        if left_pwm < 0:
            left_pwm = 0
            right_pwm += 30
        elif right_pwm < 0:
            right_pwm = 0
            left_pwm += 50
        else:
            print("front/back")
    
    rospy.loginfo(rospy.get_caller_id() + ", l_pwm = {}, r_pwm = {}".format(left_pwm, right_pwm))

    info = str(left_pwm) + ":" + str(right_pwm)

    arduino.write(info.encode())


def cmd_listner():
    rospy.Subscriber('/cmd_vel', Twist, run_cmd_callback)
    rospy.spin()

if __name__ == '__main__':
    
    rospy.init_node('cmd_listner', anonymous=True)

    # Get parameter from launch file
    PWMRANGE = rospy.get_param('~MAX_PWM')
    PWM_MIN = rospy.get_param('~MIN_PWM')
    WHEEL_DISTANCE = rospy.get_param('~WHEEL_SEP')
    # print(PWMRANGE, PWM_MIN, WHEEL_DISTANCE) # ok
    
    # wait until connecting to arduino
    connected = False
    while not connected:
        try:
            arduino = serial.Serial('/dev/ttyACM0', 9600)
            connected = True
        except serial.SerialException:
            rospy.loginfo("Arduino not found. Retrying.....")
            sleep(1)
    
    # start to listen from vel
    try:
        cmd_listner()
    except rospy.ROSInterruptException or KeyboardInterrupt:
        print("Keyboard Enterrupted!!")
        arduino.close()