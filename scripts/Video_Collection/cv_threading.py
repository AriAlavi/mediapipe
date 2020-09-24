#cv_threading class by John Kirchner
import cv2
import time
from threading import Thread


class kronos:
    def __init__(self):
        self.startvar = None
        self.endvar = None
        self.elapsed = None
        self.frames = 0
    
    def start(self):
        self.startvar = time.perf_counter()
        return self
    
    def end(self):
        self.endvar = time.perf_counter()
        self.elapsed = (self.endvar - self.startvar)
        return self

    def addFrame(self):
        self.frames+=1
        return self

    def calcFPS(self):
        return self.frames/self.elapsed

class webcam:
    def __init__(self,width,height,src=0,wait=1):
        self.stream = cv2.VideoCapture(src)
        self.wait = wait
        self.stream.set(3, width)
        self.stream.set(4, height)
        (self.grabbed, self.framevar) = self.stream.read()
        self.stopped = False
    
    def start(self):
        Thread(target=self.update, args=()).start()
        return self

    def update(self):

        while(True):

            if self.stopped == True:
                return

            (self.grabbed, self.framevar) = self.stream.read()
            cv2.waitKey(self.wait)
    
    def read(self):
        return self.framevar

    def stop(self):
        self.stopped = True
        self.stream.release()

    def changeDim(self,width,height):
        self.stream.set(3, width)
        self.stream.set(4, height)
        

if __name__ == "__main__":
    fps = 30
    total_duration = 3
    vidsrc = webcam(640,480).start()
    frmtrk = kronos().start()
    while frmtrk.frames<fps*total_duration:
        framenow = vidsrc.read()
        if framenow is not None:
            cv2.imshow("Test",framenow)
            cv2.waitKey(int(1000/fps))
            cv2.putText(framenow, str('|'),  
                        (int(vidsrc.stream.get(3)*0.1), 250), cv2.FONT_HERSHEY_PLAIN, 
                        7, (0, 0, 255), 
                        4, cv2.LINE_AA)
            frmtrk.addFrame()
            
        
            
        
    frmtrk.end()
    vidsrc.stop()
    print("Elapsed time is:",frmtrk.elapsed,"secs")
    print("FPS is:",frmtrk.calcFPS())

