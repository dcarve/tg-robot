from simulation import Simulation
import cv2 as cv


sm = Simulation(cv)

sm.create_image_size()
sm.generate_image_project()

sm.generate_simulation()
