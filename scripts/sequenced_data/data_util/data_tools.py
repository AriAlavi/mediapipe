from scipy import stats, mean
import numpy as np
import json
import os
import mpl_toolkits.mplot3d.axes3d as p3
from PIL import Image,ImageFilter
from matplotlib import animation, rc
import matplotlib.pyplot as plt
import sys

def load_json(path):
    with open(path) as f:
        data = json.load(f)
    x_data = [x for x in data.values()]
    y_data = [y for y in data.keys()]
    return (x_data,y_data)


def flatten(x_data,y_data, z_axes = True):
  x_flat, y_flat, x_frame, h = [],[],[],[]
  print('FINDING ZSCORS')
  print('...')
  for n,x in enumerate(x_data):
    for frame_seq in x:
      f = zscorize_lstm_list(frame_seq)
      x_frame = []
      for i in f:
        h = []
        xs = [i[j] for j in range(0,63,3)]
        ys = [i[j] for j in range(1,63,3)]
        if z_axes == True:
          zs = [i[j] for j in range(2,63,3)]
        for l in range(21):
          h.append(xs[l])
          h.append(ys[l])
          if z_axes == True:
            h.append(zs[l])
        x_frame.append(h)
      x_flat.append(x_frame)
      y_flat.append(n)
  return (x_flat, y_flat)

def zscorize_lstm_list(seq_list, z_axes = True):
  z_zscored_seq = []
  x_seq = stats.zscore([seq_list[i][j]  for i in range(len(seq_list)) for j in range(0,63,3)]) 
  y_seq = stats.zscore([seq_list[i][j]  for i in range(len(seq_list)) for j in range(1,63,3)])
  if z_axes == True:
    z_seq = stats.zscore([seq_list[i][j]  for i in range(len(seq_list)) for j in range(2,63,3)])
  step = 0
  for m in range(60): 
    single = []
    for k in range(21):
      step = m*21 + k
      single.append(x_seq[step]) #-x_mean)
      single.append(y_seq[step]) #-y_mean)
      if z_axes == True:
        single.append(z_seq[step]) #-z_mean)
    z_zscored_seq.append(single)
  return z_zscored_seq



def shuffle(x,y):
    assert len(x) == len(y); "Different size of input and label"
    idx = list(np.random.permutation(len(x)))
    sh_y = np.array([y[i] for i in idx])
    sh_x = np.array([x[i] for i in idx])
    return (sh_x, sh_y)

def split(x,y):
    cut = int(len(x)*0.75)
    x_train, y_train = x[0:cut], y[0:cut]
    x_test, y_test = x[cut:len(x)], y[cut:len(x)]
    return (x_train, y_train, x_test, y_test)

def generate_images(x,n, image_folder_path, gif = False): 
  new_dir = os.path.join(image_folder_path,'hand'+str(n))
  print("++++++++++++++++++")
  print(new_dir)
  if not os.path.exists(new_dir):
    os.mkdir(new_dir)
  os.chdir(new_dir)
  for k in range(len(x[n])):
    fig = plt.figure()
    ax = p3.Axes3D(fig)
    # ax = plt.axes(projection='3d')
    xs = [x[n][k][j] for j in range(0,63,3)]
    ys = [x[n][k][j] for j in range(1,63,3)]
    zs = [x[n][k][j] for j in range(2,63,3)]
    ax.set_xlim(-5,5)
    ax.set_ylim(-5,5)
    ax.set_zlim(-5,5)
    ax.text(3, 0, 5.5, "hand "+str(n), color='blue', fontsize = 13)
    ax.scatter(xs, ys, zs, 'ro')                
    for i in range(0,20,4): 
          plt.plot([xs[0],xs[i+1]],[ys[0],ys[i+1]],[zs[0],zs[i+1]],'k-')
          for j in range(0,3):
            plt.plot([xs[i+j+1],xs[i+2+j]],[ys[i+j+1],ys[i+2+j]],[zs[i+j+1],zs[i+2+j]],'k-') 
    plt.savefig("fig"+str(k)+".png") 
    plt.close()
    os.chdir(os.path.join(image_folder_path,'hand'+str(n)))
  os.chdir('..')

# def make_gif(path):
#   for m in range(2):
#     exec('a'+str(m)+'=Image.open('+path+'/'+str(m)+'.png")')
#     images.append(eval('a'+str(m)))
#   images[0].save('anim_'+str(n)+'.gif',
#                 save_all=True,
#                 append_images=images[1:],
#                 duration=100,
#                 loop = 0)











