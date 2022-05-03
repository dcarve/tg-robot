import math
import numpy as np
import cv2 as cv


class Robot:

    def __init__(self,proportion=3.0):

        self.center_offset =   int( 75 *  proportion)
        self.wheel_diameter =  int( 40 *  proportion)
        self.wheel_width =     int( 20 *  proportion)
        self.angular_position_wheel_1 = 0
        self.angular_position_wheel_2 = 120
        self.angular_position_wheel_3 = 240



    def __calc_center_position_wheel__(self, angle, center_object):
        angle = math.radians(angle)
                    
        x_position = int(center_object[0] + self.center_offset*math.cos(angle))
        y_position = int(center_object[1] - self.center_offset*math.sin(angle))

        return (x_position, y_position)
        

    def __generate_wheel__(self, image, angle, center_object):
        rect = (self.__calc_center_position_wheel__(angle,center_object), 
                (self.wheel_width, self.wheel_diameter), 
                -angle)

        box = np.int0(cv.boxPoints(rect))
        return  cv.drawContours(image,[box],0,(93,81,98),-1)


    def __generate_robot_main_body__(self, image, center_object):

        return cv.circle(img = image, 
                                center = center_object, 
                                radius=self.center_offset, 
                                color=(156, 156, 156), 
                                thickness=-1)


    
    def generate_image_project(self, image_size, center_object) -> np.ndarray:
        image_size_x = image_size[0]
        image_size_y = image_size[1]

        image = np.zeros((image_size_y,image_size_x,3), dtype = "uint8")

        image = self.__generate_robot_main_body__(image, center_object)
        image = self.__generate_wheel__(image, self.angular_position_wheel_1, center_object)
        image = self.__generate_wheel__(image, self.angular_position_wheel_2, center_object)
        image = self.__generate_wheel__(image, self.angular_position_wheel_3, center_object)

        return image