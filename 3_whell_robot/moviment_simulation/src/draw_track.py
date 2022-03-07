import numpy as np
import cv2 as cv

class Track:

    def generate_track(self, image, image_size):

        
        def draw_line(image, image_size, axis, steps):
            
            if axis==0:
                gap = int(image_size[0]/steps)
                
                for i in range(steps):
                    image = cv.line(image, (i*gap, 0) , (i*gap, image_size[1]) , (255, 255, 255) , 1) 
                

            elif axis==1:
                gap = int(image_size[1]/steps)
                
                for i in range(steps):
                    image = cv.line(image, (0, i*gap) , (image_size[0], i*gap) , (255, 255, 255) , 1) 
                
            
            return image
                

        
        image = draw_line(image, image_size=image_size, axis=0, steps=10)
        image = draw_line(image, image_size=image_size, axis=1, steps=10)
    
        return image