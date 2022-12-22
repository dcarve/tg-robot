import ctypes
import math
import numpy as np

class ScreenSize:

    def __init__(self):
        self.offset_redution_size = 0.2
        default_screen_size  = 1920
        self.defaul_image_size = int(default_screen_size-default_screen_size*self.offset_redution_size)

    def __calc_proportion__(self, x_screen_size):
        return round(x_screen_size/self.defaul_image_size, 2)

    def calc_y_image_size(self, x_image_size):
        return int(9*x_image_size/16)

    def get_screen_settings(self):

        x_image_size = int((ctypes.windll.user32.GetSystemMetrics(0)-ctypes.windll.user32.GetSystemMetrics(0)*self.offset_redution_size))
        y_image_size = self.calc_y_image_size(x_image_size)

        self.image_size = (x_image_size , y_image_size)
        self.proportion = self.__calc_proportion__(x_image_size)
        self.center_object = (int(x_image_size/2) , int(y_image_size/2))



class TransformationMatrix:

    def __init__(self, center_offset):
        self.center_offset = center_offset

    def inverse_matriz(self, linear_speed_x, linear_speed_y, algular_speed) -> list:

        l = self.center_offset

        entry_params = np.array([linear_speed_x, 
                                linear_speed_y, 
                                algular_speed])

        matriz = np.array([np.array([0, 2/3, l/3]),
                np.array([1/math.sqrt(3), -1/3, l/3]),
                np.array([-1/math.sqrt(3), -1/3, l/3])])

        wheel_speeds = (matriz * entry_params).sum(axis=1)

        return wheel_speeds
