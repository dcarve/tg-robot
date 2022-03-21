import numpy as np
import cv2 as cv

class Track:

    def generate_track(self, image_size, position):




        def get_grid():

            img = cv.imread("files_images/grid.png")
            
            y = position[1] + int(img.shape[0]/2)
            x = position[0] + int(img.shape[1]/2)

            image = img[y:y+image_size[1], 
                        x:x+image_size[0]]
                
            return image


        
        

        return get_grid()