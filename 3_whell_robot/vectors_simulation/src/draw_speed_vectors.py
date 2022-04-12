import cv2 as cv
import math
from external_functions import TransformationMatrix
from draw_robot import Robot


class Vectors:

    def __init__(self, robot_class, center_object):
        self.thickness_vectors = 2
        self.vector_wheel_1_color = (255,115, 5)
        self.vector_wheel_2_color = (78, 255, 54)
        self.vector_wheel_3_color = (7, 7, 195)
        self.rb = robot_class
        self.center_object = center_object


    def __generate_vector_wheel__(self, image, angle, wheel_speed, wheel_position, color_vector):

        angular_speed_diretion_radians = math.radians(angle) - math.radians(90)

        w_x = int(wheel_position[0] + wheel_speed*math.cos(angular_speed_diretion_radians))
        w_y = int(wheel_position[1] - wheel_speed*math.sin(angular_speed_diretion_radians))


        image = cv.arrowedLine(img = image, 
                                pt1 = (wheel_position[0], wheel_position[1]), 
                                pt2 = (w_x, w_y),
                                color = color_vector, 
                                thickness = self.thickness_vectors, 
                                tipLength=0.05)

        return image



    def draw_speed_vectors(self, image, x_mouse, y_mouse):

        image = cv.arrowedLine(img = image, 
                            pt1 = self.center_object, 
                            pt2 = (x_mouse, y_mouse),
                            color = (255, 255, 255), 
                            thickness = self.thickness_vectors, 
                            tipLength=0.05)


        tm = TransformationMatrix(self.rb.center_offset)

        wheel_speeds_1, wheel_speeds_2, wheel_speeds_3 = tm.inverse_matriz(x_mouse-self.center_object[0],
                                                                                                y_mouse-self.center_object[1],
                                                                                                0)

        image = self.__generate_vector_wheel__(image, 
                                            angle = self.rb.angular_position_wheel_1, 
                                            wheel_speed = wheel_speeds_1, 
                                            wheel_position = self.rb.__calc_center_position_wheel__(self.rb.angular_position_wheel_1, self.center_object),
                                            color_vector = self.vector_wheel_1_color)
                                            
        image = self.__generate_vector_wheel__(image, 
                                            angle = self.rb.angular_position_wheel_2, 
                                            wheel_speed = wheel_speeds_2, 
                                            wheel_position = self.rb.__calc_center_position_wheel__(self.rb.angular_position_wheel_2, self.center_object),
                                            color_vector = self.vector_wheel_2_color)
                                            
        image = self.__generate_vector_wheel__(image, 
                                            angle = self.rb.angular_position_wheel_3, 
                                            wheel_speed = wheel_speeds_3, 
                                            wheel_position = self.rb.__calc_center_position_wheel__(self.rb.angular_position_wheel_3, self.center_object),
                                            color_vector = self.vector_wheel_3_color)
                                            

        return image