import numpy as np
import cv2 as cv
from draw_robot import Robot
from draw_plan_2d import Plan2D
from external_functions import ScreenSize
import math
import time


class Simulation():

    def __init__(self):

        self.ss = ScreenSize()
        self.ss.get_screen_settings()
        self.rb = Robot(self.ss.proportion)
        self.plan2d = Plan2D()

    def calc_moviment(self, linear_speed, direction_speed, angular_speed):

        self.x_speed = linear_speed * math.cos(math.radians(direction_speed))
        self.y_speed = linear_speed * math.sin(math.radians(direction_speed))
        self.angular_speed = angular_speed


    def move_plan_2d(self, next_x_position, next_y_position, next_angle):
        return self.plan2d.generate_plan_2d(self.ss.image_size, (-next_x_position, -next_y_position, -next_angle))

    def move_robot(self, image, add_angle):
        return self.rb.generate_robot_image(image = image,
                                                center_object = self.ss.center_object,
                                                angle_object = add_angle)


    def generate_simulation(self):

        self.calc_moviment(10,45,0)

        cv.namedWindow("animation") 

        while True:


            next_angle = self.angular_speed
            next_x_position = self.x_speed
            next_y_position = self.y_speed
                        
            image  = self.move_plan_2d(next_x_position, next_y_position, next_angle)
            image  = self.move_robot(image, next_angle)

            cv.imshow('animation', image)
            if cv.waitKey(1) == ord('q'):
                break

        cv.destroyAllWindows()