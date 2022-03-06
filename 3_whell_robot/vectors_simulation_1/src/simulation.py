import numpy as np
import ctypes
import math
from params import Positions, whell_1, whell_2, whell_3
import params
import cv2 as cv


class Simulation(Positions):

    def __init__(self):
        pass

    def inverse_matriz(self, linear_speed_x, linear_speed_y, algular_speed) -> list:
        entry_params = np.array([linear_speed_x, 
                                linear_speed_y, 
                                algular_speed])

        matriz = np.array([np.array([0, 2/3, 1/3]),
                np.array([1/math.sqrt(3), -1/3, 1/3]),
                np.array([-1/math.sqrt(3), -1/3, 1/3])])

        wheel_speeds = (matriz * entry_params).sum(axis=1)

        return wheel_speeds



    def generate_image_project(self) -> np.ndarray:
        self.image = np.zeros((super().image_size_y,
                              super().image_size_x,
                              3))


        self.image = cv.circle(img = self.image, 
                                center = (super().image_center_x, super().image_center_y), 
                                radius=super().center_offset, 
                                color=(255, 255, 255), 
                                thickness=1)

        rect_dim_x = 20
        rect_dim_y = 40

        rect = ((super().wheel_1_global_position[0], super().wheel_1_global_position[1]), 
                (rect_dim_x, rect_dim_y), 
                -math.degrees(whell_1.angular_position_whell))

        box = np.int0(cv.boxPoints(rect))
        self.image = cv.drawContours(self.image,[box],0,(255,255,255),1)


        rect = ((super().wheel_2_global_position[0], super().wheel_2_global_position[1]), 
                (rect_dim_x, rect_dim_y), 
                -math.degrees(whell_2.angular_position_whell))

        box = np.int0(cv.boxPoints(rect))
        self.image = cv.drawContours(self.image,[box],0,(255,255,255),1)


        rect = ((super().wheel_3_global_position[0], super().wheel_3_global_position[1]), 
                (rect_dim_x, rect_dim_y), 
                -math.degrees(whell_3.angular_position_whell))

        box = np.int0(cv.boxPoints(rect))
        self.image = cv.drawContours(self.image,[box],0,(255,255,255),1)




    def draw_vectors(self, event,x,y,flags,params):
    
        if event == cv.EVENT_MOUSEMOVE:
            self.generate_image_project()

            cv.arrowedLine(img = self.image, 
                        pt1 = (super().image_center_x, super().image_center_y), 
                        pt2 = (x, y),
                        color = (255, 255, 255), 
                        thickness = super().thickness_vectors, 
                        tipLength=0.05)


            wheel_speeds_1, wheel_speeds_2, wheel_speeds_3 = self.inverse_matriz(x-super().image_center_x,
                                                                                y-super().image_center_y,
                                                                                0)

            w_x = int(super().wheel_1_global_position[0] + wheel_speeds_1*math.cos(whell_1.angular_speed_diretion))
            w_y = int(super().wheel_1_global_position[1] - wheel_speeds_1*math.sin(whell_1.angular_speed_diretion))
  
            cv.arrowedLine(img = self.image, 
                         pt1 = (super().wheel_1_global_position[0], super().wheel_1_global_position[1]), 
                         pt2 = (w_x, w_y),
                         color = whell_1.color_speed_direction_vector, 
                        thickness = super().thickness_vectors, 
                         tipLength=0.05)


            w_x = int(super().wheel_2_global_position[0] + wheel_speeds_2*math.cos(whell_2.angular_speed_diretion))
            w_y = int(super().wheel_2_global_position[1] - wheel_speeds_2*math.sin(whell_2.angular_speed_diretion))

            cv.arrowedLine(img = self.image, 
                         pt1 = (super().wheel_2_global_position[0], super().wheel_2_global_position[1]), 
                         pt2 = (w_x, w_y),
                         color = whell_2.color_speed_direction_vector, 
                        thickness = super().thickness_vectors, 
                         tipLength=0.05)


            w_x = int(super().wheel_3_global_position[0] + wheel_speeds_3*math.cos(whell_3.angular_speed_diretion))
            w_y = int(super().wheel_3_global_position[1] - wheel_speeds_3*math.sin(whell_3.angular_speed_diretion))

            cv.arrowedLine(img = self.image, 
                         pt1 = (super().wheel_3_global_position[0], super().wheel_3_global_position[1]), 
                         pt2 = (w_x, w_y),
                         color = whell_3.color_speed_direction_vector, 
                        thickness = super().thickness_vectors, 
                         tipLength=0.05)

    def generate_simulation(self):

        cv.namedWindow("Window")
        cv.setMouseCallback("Window",self.draw_vectors)

        while(True):

            cv.imshow("Window",self.image)
            if cv.waitKey(20) & 0xFF == ord('q'):
                break

        cv.destroyAllWindows()