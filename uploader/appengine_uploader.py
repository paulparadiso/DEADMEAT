import serial
import httplib
import time
import sys

class HeartReader(object):
    
    def __init__(self,port):
        self._ser = serial.Serial(port,9600)
        self._getHeart = "G5\r"
        
    def readHeart(self):
        if(self._ser != ""):
            self._ser.write(self._getHeart)
            r = ""
            p = ""
            while(r != "\r"):
                r = self._ser.read()
                p += r
            return self.heartParse(p)

    def heartParse(self,p):
        q = p.split(" ")
        r = []
        if(len(q) > 6):
            for i in range(2,7):
                r.append(int(q[i]))
                #print r
            self.bubbleSort(r)
        return r[2]

    def bubbleSort(self,b):
        madeSwitches = 0
        for i in range(0,4):
            if(b[i] > b[i+1]):
                madeSwitches = 1
                tmp = b[i]
                b[i] = b[i+1]
                b[i+1] = tmp
        if(madeSwitches == 1):
            self.bubbleSort(b)

class Uploader(object):

    def putHR(self,data):
        conn = httplib.HTTPConnection('sl-link.appspot.com:80')
        hr = "/set_data?hr=" + data
        #print "putting rate = " + data
        conn.request('GET', hr)
        #resp = conn.getresponse()
        #if resp.status != 200:
        #    print "That shit didn't work"
        #resp.read()
        conn.close()

    def getDIR(self):
        conn = httplib.HTTPConnection('sl-link.appspot.com:80')
        dirURL = "/get_data?dir"
        conn.request('GET', dirURL)
        resp = conn.getresponse()
        r = resp.read()
        conn.close()
        return r

class Timer():
    
    def __init__(self, t):
        self._delay = t;
        self._last = 0;

    def go(self):
        now = time.time()
        if now - self._last > self._delay:
            self._last = now
            return 1
        else:
            return 0

class LCDScreen():
    
    def __init__(self,port):
        self._ser = serial.Serial(port,9600)
        
    def write(msg):
        self._ser.write(msg)
        

if __name__ == "__main__":
    u = Uploader()
    lcd = LCDScreen("/dev/ttyUSB0")
    ck_serial = True
    for arg in sys.argv:
        if arg == "dead":
            print "Running in Dead Mode."
            ck_serial = False
    if ck_serial == True:
        hr_timer = Timer(30.0)
    dir_timer = Timer(0.1)
    if ck_serial == True:
        hr = HeartReader("/dev/tty.usbserial-A6007z5N")
    direction = ""
    while(1):
        if ck_serial == True:
            if(hr_timer.go() == 1):
                result = hr.readHeart()
                u.putHR(str(result))
        if(dir_timer.go() == 1):
            r = u.getDIR()
            if(r != direction):
                direction = r
                print r
                lcd.write(r)
            
