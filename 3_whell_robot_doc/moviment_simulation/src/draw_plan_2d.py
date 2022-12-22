import numpy as np
import cv2 as cv
import math

class Plan2D:

    def __init__(self):
        self.position = [0,0]
        self.angle = 0
        

    def generate_plan_2d(self, image_size, position_angle_offset_change):

        img = cv.imread("files_images/grid.png")

        self.angle = self.angle + position_angle_offset_change[2]

        modulo = math.sqrt(position_angle_offset_change[0]**2 +  position_angle_offset_change[1]**2)
        angle_temp = math.atan(position_angle_offset_change[1]/position_angle_offset_change[0]) + self.angle
        
        position_offset_change = [modulo * math.cos(angle_temp), modulo * math.sin(angle_temp)]    


        self.position = [self.position[0] - position_offset_change[0], 
                         self.position[1] + position_offset_change[1]]

            
        x = int(self.position[0] + img.shape[1]/2)
        y = int(self.position[1] + img.shape[0]/2)


        image = img[y:y+image_size[1], 
                    x:x+image_size[0]]
            

        return image