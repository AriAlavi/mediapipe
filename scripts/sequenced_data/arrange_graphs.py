import os
from data_util import data_tools as dt
import cv2
import numpy as np
import glob
from PIL import Image,ImageFilter
import shutil



json_path = os.path.join(os.getcwd(),'SigNN Formed Mediapipe 3D Video Json copy/ALL/dynamic_lstm_training_data.json')
image_folder_path = os.path.join(os.getcwd(),'image_data')
dir_to_fifheen = os.path.join(os.getcwd(),'image_data','fiftheens')

x_data, y_data = dt.load_json(json_path)
x_flat, y_flat = dt.flatten(x_data,y_data)

print(x_flat[0])


count = 0
while(count <= len(x_flat)//15):
  hand_range = list(range(count*15,count*15+15))
  for i in hand_range: # generate the images
    dt.generate_images(x_flat,i,image_folder_path)
  os.chdir(dir_to_fifheen)

  for j in range(15): # generate the 15-graphs
      HAND = []
      TOP = np.zeros(shape = [200,640,3],dtype=np.uint8)
      TOP.fill(255)
      for i in hand_range:
          im = cv2.imread(os.path.join(image_folder_path,'hand'+str(i),'fig'+str(j)+'.png'))
          print(im)
          HAND.append(im)
          # assert len(HAND) == 15; 'not 15 hands'
      im_v = cv2.vconcat([cv2.hconcat([TOP for _ in range(5)]),
                          cv2.hconcat([HAND[k] for k in range(0,5)]),
                          cv2.hconcat([HAND[k] for k in range(5,10)]),
                          cv2.hconcat([HAND[k] for k in range(10,15)])])
      os.chdir(dir_to_fifheen)
      cv2.imwrite('f'+str(j)+'.png', im_v) 
  os.chdir('..')

  # remove the folder with hand imgs
  hands = fiftheen = glob.glob('hand*')
  for folder in hands:
    shutil.rmtree(folder)
    
  os.chdir(dir_to_fifheen)
  print(os.getcwd())
  images = [] 

  for m in range(15): # generate the gifs   
      exec('a'+str(m)+'=Image.open("f'+str(m)+'.png")')
      images.append(eval('a'+str(m)))
  images[0].save('anim_'+str(count)+'.gif',
                  save_all=True,
                  append_images=images[1:],
                  duration=150,
                  loop = 0)

  ft = fiftheen = glob.glob('f*.png')
  for filename in ft:
    os.remove(filename)
  count += 1
  



























































# def click_and_crop(event, x, y, flags, param):
# 	# grab references to the global variables
# 	global refPt, cropping
# 	# if the left mouse button was clicked, record the starting
# 	# (x, y) coordinates and indicate that cropping is being
# 	# performed
# 	if event == cv2.EVENT_LBUTTONDOWN:
# 		refPt = [(x, y)]
# 		cropping = True
# 	# check to see if the left mouse button was released
# 	elif event == cv2.EVENT_LBUTTONUP:
# 		# record the ending (x, y) coordinates and indicate that
# 		# the cropping operation is finished
# 		refPt.append((x, y))
# 		cropping = False
# 		# draw a rectangle around the region of interest
# 		cv2.rectangle(image, refPt[0], refPt[1], (0, 255, 0), 2)
# 		cv2.imshow("image", image)

# for el in imfilelist:
#         print el
#         image = cv2.imread(el, cv2.CV_LOAD_IMAGE_COLOR)
#         cv2.imshow('Image', image) #Show the image
#         cv2.waitKey(1000)

# fiftheen = glob.glob(os.path.join(hand_dir,'fiftheens','*.png'))
# n = 0
# while(n < 25):
#     img = cv2.imread(fiftheen[n])
#     cv2.imshow('DISPLAY',img)
#     n += 1
#     if n == 23:
#         n = 0
#     cv2.waitKey(10)