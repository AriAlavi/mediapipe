#videoscript.py by Gokul Deep, John Kirchner, and Vahe Barseghyan
#Dependencies:
#pip install opencv-python
#pip install numpy
import os
import sys
import numpy as np
import cv2
import time
from GDrive import *
from datetime import datetime
from cv_threading import *

AVI_DATABASE = "14HLz4WqhRCRfFfNHSOrnOfUWewpZe2oP"
drive = authenticate()
avi_database = GoogleDriveDatabase(drive, AVI_DATABASE)


def takeVideo(character):
    
    while(True):
        filebase = character + "_" + datetime.now().strftime('%m-%d-%Y_%H_%M_%S')
        filename = filebase + ".avi"
        frames_per_second = 30.0
        capture_duration = 3  # the duration of the video captured
        total_frames = (capture_duration * frames_per_second)
        frame_speed = (int(1000/frames_per_second))
        my_res = "480p"  # can change to a certain resolution from STD_DIMENSIONS
        cam_width,cam_height = get_dim(my_res)
        print(cam_width,cam_height)
        capture = webcam(cam_width,cam_height).start()
        #print(capture.get(cv2.CAP_PROP_FPS),"is the FPS capture")
        #print(capture.get(3),"is the width")
        #print(capture.get(4),"is the height")
        four_cc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
        

        out = cv2.VideoWriter(filename, four_cc, frames_per_second, (640,480))
        TIMER = int(5)
        font = cv2.FONT_HERSHEY_PLAIN 
        quality_control = 'N'
        
        while(True):
            #print("IN first while loop") 
            frame = capture.read() 
            cv2.putText(frame, "Press s to start",  
                                (50, 250), font, 
                                3, (0, 0, 255), 
                                4, cv2.LINE_AA)
            if frame is not None:
                cv2.imshow('RECORDING', frame) 
                cv2.waitKey(1)
            k = cv2.waitKey(125) 
        
            if k == ord('s'):
                prev = time.time() 
                # the timer loop
                while TIMER >= 0: 
                    frame = capture.read()             
                    cv2.putText(frame, str(TIMER),  
                                (50, 250), font, 
                                3, (0, 0, 255), 
                                4, cv2.LINE_AA)
                    if frame is not None: 
                        cv2.imshow('RECORDING', frame) 
                    cv2.waitKey(125) 
                    cur = time.time() # current time 
        
                    if cur-prev >= 1: 
                        prev = cur 
                        TIMER = TIMER-1
                break
            elif k == 27: 
                break

        frmtrk = kronos().start()
        # recording loop
        while (frmtrk.frames<total_frames):
            tmp_time = time.perf_counter()-frmtrk.startvar
            record_frame = capture.read()
            show_frame = record_frame
            if record_frame is not None:
                
                out.write(record_frame)
                frmtrk.addFrame()
                arrow = (0.1*capture.stream.get(3)) + ((frmtrk.frames/total_frames)*0.8*capture.stream.get(3))
                cv2.putText(show_frame, str('|'),  
                        (int(capture.stream.get(3)*0.1), 250), font, 
                        7, (0, 0, 255), 
                        4, cv2.LINE_AA)

                cv2.putText(show_frame, str('|'),  
                        (int(capture.stream.get(3)*0.9), 250), font, 
                        7, (0, 0, 255), 
                        4, cv2.LINE_AA)  
            
            
                cv2.putText(show_frame, str('->'),  
                        (int(arrow), 250), font, 
                        3, (0, 0, 255), 
                        4, cv2.LINE_AA)

                cv2.imshow('RECORDING',show_frame)
                cv2.waitKey(int(1000/frames_per_second))
                

            
            
        frmtrk.end()
        print("Number of frames:",frmtrk.frames)
        print("Time:",frmtrk.elapsed)
        # displaying the saved video
        capture_display = webcam(cam_width,cam_height,filename,int(1000/frames_per_second)).start()
        display_frame = 1
        frame_list = []
        while not capture_display.stopped:
            #for i in range(int(total_frames)):
                #frame = capture_display.read()
                #frame_list.append(frame)
            #for i in range(int(total_frames)):
                #frame = frame_list.pop(0)
                frame = capture_display.read()
                cv2.putText(frame, "DISPLAYING THE VIDEO",  
                        (50, 50), font, 
                        1, (0, 0, 255), 
                        4, cv2.LINE_AA)
                if frame is not None:
                    cv2.imshow('RECORDING', frame)
                if frame is None:
                    break
                k = cv2.waitKey(1)
            
        capture_display.stop()
        while (capture.stopped != True):
            
            frame = capture.read()
            end_time = time.monotonic()
            cv2.putText(frame, "Was that video good enough?",  
                                (50, 50), font, 
                                1, (0, 0, 255), 
                                4, cv2.LINE_AA)
            cv2.putText(frame, "Y = YES, N = NO, R = REPLAY",  
                                (50, 100), font, 
                                1, (0, 0, 255), 
                                4, cv2.LINE_AA)
            if frame is not None:                                
                cv2.imshow('RECORDING', frame)
            k = cv2.waitKey(1) 
            if k == ord('n'): 
                quality_control = 'N'
                break
            if k == ord('y'):
                quality_control = 'Y'
                break

        out.release()

        if quality_control == "Y" or quality_control == "y":
            UploadVideoPath(filename,character)
            capture.stop()
            capture_display.stop()
            
            os.remove(filename)
    
        else:
            capture.stop()
            capture_display.stop()
            out.release()
            os.remove(filename)
            print("deleted {}".format(filename))
            continue
    return filename

def UploadVideoPath(input_directory, character):
    avi_database.upload(input_directory,character,"avi")

def change_res(capture, width, height):
    
    capture.stream.set(4, height)

def get_dim(res):
    # standard dimension sizes
    STD_DIMENSIONS = {
        "480p": (640, 480),
        "720p": (1280, 720),
        "1080p": (1920, 1080)
    }
    width, height = STD_DIMENSIONS['1080p']  # default is 1080p but can change my_res to get different resolutions
    if res in STD_DIMENSIONS:
        width, height = STD_DIMENSIONS[res]
    return width, height

if __name__ == "__main__":
    while(True):
        while(True):
            character = input("Which letter would you like to record? ")
            if(avi_database.checkFolder(character)):
                break
            else:
                print("{} is not a valid character. Pick from list: \n{}".format(character, tuple(avi_database.folders.keys())))
                
        takeVideo(character)

