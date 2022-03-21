import cv2 as cv
from draw_robot import Robot
from draw_speed_vectors import Vectors
from external_functions import ScreenSize


class Simulation():

    def __init__(self):

        self.ss = ScreenSize()
        self.ss.get_screen_settings()

        self.rb = Robot(self.ss.proportion)
        
        self.vt = Vectors(robot_class = self.rb, center_object = self.ss.center_object)


        self.image = self.rb.generate_image_project(image_size = self.ss.image_size,
                                                center_object = self.ss.center_object)


    def action(self, event,x,y,flags,params):
    
        if event == cv.EVENT_MOUSEMOVE:

            self.image = self.rb.generate_image_project(image_size = self.ss.image_size,
                                                center_object = self.ss.center_object )

            self.image = self.vt.draw_speed_vectors(self.image, 
                                                    x_mouse=x, 
                                                    y_mouse=y)


    def generate_simulation(self):

        cv.namedWindow("Window") 
        cv.setMouseCallback("Window",self.action)

        while(True):

            cv.imshow("Window",self.image)
            if cv.waitKey(20) & 0xFF == ord('q'):
                break

        cv.destroyAllWindows()