import numpy as np
import ctypes


class Simulation:

    def __init__(self, cv):
        self.cv = cv


    def create_image_size(self) -> list:
        user32 = ctypes.windll.user32
        self.x_length = user32.GetSystemMetrics(0)-100
        self.y_length = user32.GetSystemMetrics(1)-100

    def generate_image_project(self) -> np.ndarray:
        self.image = np.zeros((self.y_length,self.x_length,3))


    def draw_vectors(self, event,x,y,flags,params):
    
        start_point = (int(self.x_length/2), int(self.y_length/2))
        end_point = (x, y)
        color = (255, 255, 255)
        thickness = 2

        if event == self.cv.EVENT_MOUSEMOVE:
            self.generate_image_project()

            self.cv.arrowedLine(self.image, 
                        start_point, 
                        end_point,
                        color, 
                        thickness, 
                        tipLength=0.05)

    def generate_simulation(self):

        self.cv.namedWindow("Window")
        self.cv.setMouseCallback("Window",self.draw_vectors)

        while(True):

            self.cv.imshow("Window",self.image)
            if self.cv.waitKey(20) & 0xFF == ord('q'):
                break

        self.cv.destroyAllWindows()