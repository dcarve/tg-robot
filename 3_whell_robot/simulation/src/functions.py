import ctypes
import numpy as np


def get_screen_size() -> tuple:
    user32 = ctypes.windll.user32
    return (user32.GetSystemMetrics(0), user32.GetSystemMetrics(1))

def get_project_image(x, y) -> np.ndarray:
    return np.zeros((y-100,x-100,3))



