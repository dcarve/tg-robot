import numpy as np
import ctypes
import math


class Simulation:

    def __init__(self, cv):
        self.cv = cv

    def inverse_matriz(self, linear_speed_x, linear_speed_y, algular_speed) -> list:
        entry_params = np.array([linear_speed_x, 
                                linear_speed_y, 
                                algular_speed])

        matriz = np.array([np.array([0, 2/3, 1/3]),
                np.array([1/math.sqrt(3), -1/3, 1/3]),
                np.array([-1/math.sqrt(3), -1/3, 1/3])])

        wheel_speeds = (matriz * entry_params).sum(axis=1)

        print(wheel_speeds)

        return wheel_speeds


    def create_image_size(self) -> list:
        user32 = ctypes.windll.user32
        self.x_length = user32.GetSystemMetrics(0)-100
        self.y_length = user32.GetSystemMetrics(1)-100

    def generate_image_project(self) -> np.ndarray:
        self.image = np.zeros((self.y_length,self.x_length,3))


    def draw_vectors(self, event,x,y,flags,params):
    
        if event == self.cv.EVENT_MOUSEMOVE:
            self.generate_image_project()

            self.cv.arrowedLine(img = self.image, 
                        pt1 = (int(self.x_length/2), int(self.y_length/2)), 
                        pt2 = (x, y),
                        color = (255, 255, 255), 
                        thickness = 3, 
                        tipLength=0.05)



            wheel_speeds_1, wheel_speeds_2, wheel_speeds_3 = self.inverse_matriz(x-int(self.x_length/2),y-int(self.y_length/2),0)

            w_x = int(wheel_speeds_1*math.cos(-3.1415/2)+(self.x_length/4))
            w_y = int((self.y_length/2) - wheel_speeds_1*math.sin(-3.1415/2))

            self.cv.arrowedLine(img = self.image, 
                         pt1 = (int(self.x_length/4), int(self.y_length/2)), 
                         pt2 = (w_x, w_y),
                         color = (255,0, 255), 
                         thickness = 2, 
                         tipLength=0.05)


            w_x = int(wheel_speeds_2*math.cos(3.1415/6)+(self.x_length/4))
            w_y = int((self.y_length/2) - wheel_speeds_2*math.sin(3.1415/6))

            self.cv.arrowedLine(img = self.image, 
                         pt1 = (int(self.x_length/4), int(self.y_length/2)), 
                         pt2 = (w_x, w_y),
                         color = (0, 255, 255), 
                         thickness = 2, 
                         tipLength=0.05)


            w_x = int(wheel_speeds_3*math.cos(5*3.1415/6)+(self.x_length/4))
            w_y = int((self.y_length/2) - wheel_speeds_3*math.sin(5*3.1415/6))

            self.cv.arrowedLine(img = self.image, 
                         pt1 = (int(self.x_length/4), int(self.y_length/2)), 
                         pt2 = (w_x, w_y),
                         color = (0, 0, 255), 
                         thickness = 2, 
                         tipLength=0.05)

    def generate_simulation(self):

        self.cv.namedWindow("Window")
        self.cv.setMouseCallback("Window",self.draw_vectors)

        while(True):

            self.cv.imshow("Window",self.image)
            if self.cv.waitKey(20) & 0xFF == ord('q'):
                break

        self.cv.destroyAllWindows()