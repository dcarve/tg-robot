import numpy as np
import cv2 as cv
from draw_robot import Robot
from draw_speed_vectors import Vectors
from external_functions import ScreenSize



class Simulation():

    def __init__(self):
        """    
        not yet 
        """
        self.ss = ScreenSize()
        self.ss.get_screen_settings()

        self.rb = Robot(self.ss.proportion/2)

        self.rb.calc_wheels_angle(0)

        self.image = np.zeros((self.ss.image_size[1],self.ss.image_size[0],3), dtype = "uint8")
        
        self.image = self.rb.generate_image_project(image = self.image,
                                                center_object = self.ss.center_object)


    def action(self, event,x,y,flags,params):
    
       
        self.image = self.rb.generate_image_project(image = self.image,
                                            center_object = self.ss.center_object )


    def generate_simulation(self):

        cv.namedWindow("Window")  #, cv.WND_PROP_FULLSCREEN)
        #cv.setWindowProperty("Window",cv.WND_PROP_FULLSCREEN,cv.WINDOW_FULLSCREEN)
        cv.setMouseCallback("Window",self.action)

        while(True):

            cv.imshow("Window",self.image)
            if cv.waitKey(20) & 0xFF == ord('q'):
                break

        cv.destroyAllWindows()