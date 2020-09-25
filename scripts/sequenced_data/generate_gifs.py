# import cv2
# import numpy as np
# import os

# hand_dir = os.path.join(os.getcwd(),'image_data')

# HAND = []

# for i in range(15):
#     im = cv2.imread(os.path.join(hand_dir,'hand'+str(i),'fig0.png'))
#     HAND.append(im)

# im = cv2.imread('/Users/vahebarseghyan/code/J_AND_Z_HOUSEKEEPING/lena.jpg')
# im_v = cv2.vconcat([
#                     cv2.hconcat([HAND[i] for i in range(0,5)]),
#                     cv2.hconcat([HAND[i] for i in range(5,10)]),
#                     cv2.hconcat([HAND[i] for i in range(10,15)])
#                     ])
# # im_v = cv2.hconcat([cv2.vconcat([im[1],im[2]]),cv2.vconcat([im[1],im[2]])])
# cv2.imwrite('ss.jpeg', im_v)

# # img = cv2.imread('ss.jpeg')
# # while(True):
#     # cv2.imshow("image", img)
