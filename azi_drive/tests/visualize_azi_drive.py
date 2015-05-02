#!/usr/bin/python
## Math
from __future__ import division
import numpy as np
## Display
import pygame
import time
## Ros
import rospy
from tf import transformations as tf_trans
## AZI
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import Azi_Drive
## Ros Msgs
from std_msgs.msg import Header, Float64
from geometry_msgs.msg import Point, PointStamped, Quaternion
from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig

SCREEN_DIM = (750, 750)
ORIGIN = np.array([SCREEN_DIM[0] / 2.0, SCREEN_DIM[1] / 2.0])


def print_in(f):
    print("Defining " + f.func_name)
    def print_on_entry(*args, **kwargs):
        print("Executing " + f.func_name + " with arguments " + str(args) + " and kwargs " + str(kwargs))
        result = f(*args, **kwargs)
        print("Returning " + str(result))
        return(result)
    return(print_on_entry)


def round_point((x, y)):
    '''Round and change point to centered coordinate system'''
    return map(int, ((30 * y) + ORIGIN[0], -(30 * x) + ORIGIN[1]))


def unround_point((x, y)):
    '''Change center-origin coordinates to pygame coordinates'''
    return ((y - ORIGIN[0]) / 30.0, (-x + ORIGIN[1]) / 30.0)


def norm_angle_diff(ang_1, ang_2):
    '''norm_angle_diff(ang_1, ang_2)
    -> Normalized angle difference, constrained to [-pi, pi]'''
    return (ang_1 - ang_2 + np.pi) % (2 * np.pi) - np.pi


def saneify_angle(boat_angle):
    '''Make boat-style servo angles into something more...sane'''
    real_angle = (boat_angle - np.pi) / 2
    return real_angle


class Azi_Drive_Visual(object):
    def __init__(self):
        '''This intends to simulate the physical servo angles without fake offsets'''
        rospy.init_node('azi_drive_visualization')

        rospy.Subscriber('thruster_config', thrusterNewtons, 
            self._thruster_cb, queue_size=4)
        rospy.Subscriber('dynamixel/dynamixel_full_config', DynamixelFullConfig, 
            self._dynamixel_cb, queue_size=4)
        rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size=1)

        # Thruster 2 is on the right, thruster 3 is on the left
        self.thruster_forces = {2: 0, 3: 0}
        self.thruster_angles = {2: 0.0, 3: 0.0}
        self.thruster_positions = {
            # Taken from simulation
            3: (-0.7239, -0.3048),
            2: (-0.7239, 0.3048),
        }

    def _thruster_cb(self, msg):
        self.thruster_forces[msg.id] = msg.thrust

    def _wrench_cb(self, msg):
        pass

    def _dynamixel_cb(self, msg):
        self.thruster_angles[msg.id] = msg.goal_position

    def ray(self, display, start, angle, length, scale=0.1):
        # pygame.draw.line(display, color, start_pos, end_pos, width=1): return Rect

        unit_v = np.array([np.sin(angle), np.cos(angle)])
        _start = round_point(start)
        _end = round_point(start + (unit_v * length * scale))


        pygame.draw.circle(display, (255, 200, 80), _start, 5)

        pygame.draw.line(display, (255, 0, 0), 
            _start,
            _end,
            3
        )

        Text_Box.draw(display, 
            pos=_end,
            color=(255, 0, 0), 
            text="Force: {}\nReal Angle: {}\n".format(
                round(length, 4),
                round(angle, 4)
            ),
        )


    def vector(self, display, start, direction, scale=0.1):
        pygame.draw.circle(display, (200, 255, 20), round_point(start), 5)
        pygame.draw.line(display, (0, 255, 0),
            round_point(start),
            round_point(np.array(start) + (np.array(direction) * scale)),
        )


    def draw(self, display):
        '''Draw the whole arm'''
        # Update positions given current angles
        for _id, position in self.thruster_positions.items():
            self.ray(display, 
                start=position, 
                angle=saneify_angle(self.thruster_angles[_id]) - (np.pi / 2), 
                length=self.thruster_forces[_id],
                scale=0.1
            )        
        
        net = Azi_Drive.net_force(
            alpha=[saneify_angle(self.thruster_angles[3]), saneify_angle(self.thruster_angles[2])],
            u=[self.thruster_forces[3], self.thruster_forces[2]],
        )
        # Swapped x and y
        self.vector(display, start=(0, 0), direction=(net[0], net[1]), scale=0.1)

        Text_Box.draw(display, 
            pos=(475, 200),
            color=(60, 200, 30), 
            text="Fx: {}\nFy: {}\nTorque: {}\n".format(
                round(net[0], 4), 
                round(net[1], 4),
                round(net[2], 4),
            ),
        )



class Text_Box(object):
    '''Text_Box()
    You never have to initialize this! Just call Text_Box.draw(display, pos, color, text)
    It draws the same way a pygame primitive would.
    '''
    pygame.font.init()
    font = pygame.font.SysFont("monospace", 15)


    @classmethod
    def draw(self, display, pos=(0, 0), color=(255, 255, 255), text="Empty!"):
        ''' draw(display, pos=(0, 0), color=(255, 255, 255), text="Empty!"):

        pos: In pygame coordinates
        color: [0, 255]
        text: Can by multiline, of arbitrary length

        To change text during operation, use the "set_text method"
        Ex:
            >>> tb = Text_Box()
            >>> tb.draw(display, text='hello')
        or in a draw loop,
            >>> tb.draw(display, pos, color, text)
        '''
        lines = text.splitlines()
        width = height = 0
        for l in lines:
            width = max(width, self.font.size(l)[0])
            height += self.font.get_linesize()

        height = 0
        for l in lines:
            t = self.font.render(l, 0, color)
            display.blit(
                t, 
                (pos[0], pos[1] + height)
            )
            height += self.font.get_linesize()


def main():
    '''In principle, we can support an arbitrary number of arms in simulation'''
    draws = [
        Azi_Drive_Visual()
    ]

    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption("Boat azi_drive_visualization")

    clock = pygame.time.Clock()

    while not rospy.is_shutdown():
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return

            if event.type == pygame.KEYDOWN:
                if (event.key == pygame.K_ESCAPE) or (event.key == pygame.K_q):
                    return

        t = time.time()
        for draw in draws:
            draw.draw(display)
        
        pygame.display.update()
        clock.tick(20)
        display.fill((0, 0, 0))


if __name__ == '__main__':
    main()