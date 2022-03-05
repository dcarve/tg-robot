import ctypes
import math


class whell_1:
    """
        angular_position_whell: degrees
        angular_position_whell: degrees
        radius: not used yet
    """
    angular_position_whell = math.radians(0)
    angular_speed_diretion = angular_position_whell - math.radians(90)
    radius = 1
    color_speed_direction_vector = (255,0, 255)

class whell_2:
    """
        angular_position_whell: degrees
        angular_position_whell: degrees
        radius: not used yet

    """
    angular_position_whell = math.radians(120)
    angular_speed_diretion = angular_position_whell - math.radians(90)
    radius = 1
    color_speed_direction_vector = (0, 255, 255)


class whell_3:
    """
        angular_position_whell: degrees
        angular_position_whell: degrees
        radius: not used yet
    """

    angular_position_whell = math.radians(240)
    angular_speed_diretion = angular_position_whell - math.radians(90)
    radius = 1
    color_speed_direction_vector = (0, 0, 255)


class Positions:

    center_offset = 150
    thickness_vectors = 2

    image_size_x = ctypes.windll.user32.GetSystemMetrics(0)-100
    image_size_y = ctypes.windll.user32.GetSystemMetrics(1)-100

    image_center_x = int(image_size_x/2)
    image_center_y = int(image_size_y/2)

    wheel_1_global_position = [int(image_center_x + center_offset*math.cos(whell_1.angular_position_whell)),
                              int(image_center_y - center_offset*math.sin(whell_1.angular_position_whell))]
                              
    wheel_2_global_position = [int(image_center_x + center_offset*math.cos(whell_2.angular_position_whell)),
                              int(image_center_y - center_offset*math.sin(whell_2.angular_position_whell))]
                              
    wheel_3_global_position = [int(image_center_x + center_offset*math.cos(whell_3.angular_position_whell)),
                              int(image_center_y - center_offset*math.sin(whell_3.angular_position_whell))]