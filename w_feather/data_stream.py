

import time
import random
import numpy as np
import warnings


#max bend= 113.73752658643255 min_bend=3.0000150000000003
path = 'index.html'
data=np.loadtxt('filtered_data',delimiter=',')
x=data[:,0]
y=data[:,1]

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
        signal=[x[i],y[i]]
        callback(signal,fs)
        i+=1
        elapsed=time.time()-now
        try:
            time.sleep(period-elapsed)
        except:
            warnings.warn('System cannot handle such high frame rate, lower the desired frequency or simplify your callback fucntion')
            continue

x_array=[]
y_array=[]    
def callback(signal,fs):
    global x_array,y_array
    x,y=signal[0],signal[1]
    x_array.append(x)
    y_array.append(y)
    if len(x_array)==fs: #Freq=1Hz
        x_=np.average(x_array)
        y_=np.average(y_array)
        x_array=[]
        y_array=[]
        
        msg=[[x,y]]
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
