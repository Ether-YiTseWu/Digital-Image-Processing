# -*- coding: utf-8 -*-
"""
Created on Thu Dec 26 13:07:31 2019

@author: Administrator
"""

import numpy as np
import tkinter as tk
#from skimage import io
from keras.preprocessing import image
import tensorflow as tf
from tkinter import filedialog
#import matplotlib.pyplot as plt
from tensorflow.compat.v1 import ConfigProto
from tensorflow.compat.v1 import InteractiveSession

config = ConfigProto()
config.gpu_options.allow_growth = True
session = InteractiveSession(config=config)

model = tf.contrib.keras.models.load_model('CatDogClassifier.h5')

def judge():
    img = image.load_img(CSV_path, target_size=(64, 64))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = x.astype('float32') /255
    #--- Use model to predict
    y_pred = model.predict(x)[:,1]
    score = float(y_pred)
    
    if score > 0.5:
        animal = 'Cat'
    elif score <= 0.5:
        animal = 'Dog'
    
    show_text.insert('insert',"This animal is ")
    show_text.insert('insert', animal) 
    
    #imgRGB = io.imread(CSV_path)
    #plt.figure(figsize=(15,6)) 
    #io.imshow(imgRGB)
    
window = tk.Tk()
window.title('Logistic Regression Classifier')
window.geometry('300x150')

#------------------------------------------------------------------------#

def Pathfinding():
    global CSV_path 
    CSV_path = filedialog.askopenfilename()
    show_text.insert('insert', "File Path：" + CSV_path)
    show_text.insert('insert', "\n\n")
    return CSV_path

button_selectCSV = tk.Button(window, text = "選擇檔案", command = Pathfinding)
button_selectCSV.pack()

#------------------------------------------------------------------------#
# 以下為 Show Text
show_text = tk.Text(window, height = 5, width = 32)
show_text.pack()
#------------------------------------------------------------------------#
# Part of Function and Button
def deleteData():   
    show_text.delete("1.0",tk.END)

button_train = tk.Button(window, text = "Classify", command = judge)
button_train.pack()

button_clear = tk.Button(window, text = "Clear", command = deleteData)
button_clear.pack()

window.mainloop()