

import time
import random
import numpy as np
import warnings


#max bend= 113.73752658643255 min_bend=3.0000150000000003
path = 'index.html'
data=np.loadtxt('augmented_data.csv',delimiter=',')
x1=data[:,0]
y1=data[:,1]
x2=data[:,2]
y2=data[:,3]
l=len(x1)

def write_to_file(msg,path):
    row=''
    row += str(msg)
    # row += '\n'
    
    #append to file: 'a'
    #overwrite : 'w'
    with open(path,'w') as f:
        f.write(row)




def stream_record(callback,fs):
    period=1.0/fs
    i=0
    while True:
        now=time.time()
        signal=[[x1[i],y1[i]],[x2[i],y2[i]]]
        callback(signal,fs)
        i+=1
        if i==l:
            i=0
        elapsed=time.time()-now
        try:
            time.sleep(period-elapsed)
        except:
            warnings.warn('System cannot handle such high frame rate, lower the desired frequency or simplify your callback fucntion')
            continue

x1_array=[]
y1_array=[]
x2_array=[]
y2_array=[]  
def callback(signal,fs):
    global x1_array,y1_array,x2_array,y2_array
    x1,y1=signal[0][0],signal[0][1]
    x2,y2=signal[1][0],signal[1][1]
    x1_array.append(x1)
    y1_array.append(y1)
    x2_array.append(x2)
    y2_array.append(y2)

    if len(x1_array)==fs: #Freq=1Hz
        x1_=np.average(x1_array)
        y1_=np.average(y1_array)
        x2_=np.average(x2_array)
        y2_=np.average(y2_array)
        x1_array=[]
        y1_array=[]
        x2_array=[]
        y2_array=[]
        
        msg=[[x1_,y1_],[x2_,y2_]]
        print(msg)
        write_to_file(msg,path)



def stream_random():
    while True:
        x1=random.randint(-5,5)
        y1=random.randint(-5,5)
        x2=random.randint(-5,5)
        y2=random.randint(-5,5)

        signal=[[x1,y1],[x2,y2]]
        print(signal)
        
        write_to_file(signal,path)
        

        time.sleep(1)

stream_record(callback,200)
# stream_random()
