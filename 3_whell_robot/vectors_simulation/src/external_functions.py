import ctypes
import math
import numpy as np

class ScreenSize:

    def __init__(self):
        self.defaul_size_screen = 1920

    def __calc_proportion__(self, x_screen_size):
        return round(x_screen_size/self.defaul_size_screen, 2)

    def get_screen_settings(self):

        screen_size_x = ctypes.windll.user32.GetSystemMetrics(0)
        screen_size_y = ctypes.windll.user32.GetSystemMetrics(1)

        self.image_size = (screen_size_x , screen_size_y)
        self.proportion = self.__calc_proportion__(screen_size_x)
        self.center_object = (int(screen_size_x/2) , int(screen_size_y/2))



class TransformationMatrix:

    def inverse_matriz(self, linear_speed_x, linear_speed_y, algular_speed) -> list:
        entry_params = np.array([linear_speed_x, 
                                linear_speed_y, 
                                algular_speed])

        matriz = np.array([np.array([0, 2/3, 1/3]),
                np.array([1/math.sqrt(3), -1/3, 1/3]),
                np.array([-1/math.sqrt(3), -1/3, 1/3])])

        wheel_speeds = (matriz * entry_params).sum(axis=1)

        return wheel_speeds
