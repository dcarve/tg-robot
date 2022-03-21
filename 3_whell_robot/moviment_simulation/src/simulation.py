import numpy as np
import cv2 as cv
from draw_robot import Robot
from draw_track import Track
from external_functions import ScreenSize
from external_functions import TransformationMatrix
import math


class Simulation():

    def __init__(self):

        self.ss = ScreenSize()
        self.ss.get_screen_settings()

        self.rb = Robot(self.ss.proportion/2)

        self.image = np.zeros((self.ss.image_size[1],self.ss.image_size[0],3), dtype = "uint8")

    def calc_moviment(self, linear_speed, direction_speed, angular_speed):

        self.x_speed = int(linear_speed * math.cos(math.radians(direction_speed)))
        self.y_speed = int(linear_speed * math.sin(math.radians(direction_speed)))
        self.angular_speed = angular_speed
        
        #wheel_speeds_1, wheel_speeds_2, wheel_speeds_3 = TransformationMatrix().inverse_matriz(self.x_speed,
        #                                                                                        self.y_speed,
        #                                                                                        self.angular_speed)
        #return wheel_speeds_1, wheel_speeds_2, wheel_speeds_3

    def move_robot(self, add_angle):

        self.image  = Track().generate_track(self.ss.image_size, (0,0))
        
        self.image = self.rb.generate_robot_image(image = self.image,
                                                 center_object = self.ss.center_object,
                                                 angle_object = add_angle)



    def generate_simulation(self):

        self.calc_moviment(2,50,1)

        cv.namedWindow("animation") 

        i=0

        while True:
            i += 1

            next_angle = self.angular_speed*i*2
            
            self.move_robot(next_angle)

            cv.imshow('animation', self.image)
            if cv.waitKey(1) == ord('q'):
                break
            
        cv.destroyAllWindows()