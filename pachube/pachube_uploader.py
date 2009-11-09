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

class PachubeUploader(object):

    def __init__(self, feed, key):
        self._url = feed
        self._key = key

    def putCSV(self,data):
        conn = httplib.HTTPConnection('www.pachube.com:80')
        feedURL = '/api/' + self._url + '.csv'
        conn.request('PUT', feedURL, data, {'X-PachubeApiKey':self._key})
        resp = conn.getresponse()
        if resp.status != 200:
            print "That shit didn't work"
        resp.read()
        conn.close()

    def getCSV(self):
        conn = httplib.HTTPConnection('www.pachube.com:80')
        dirURL = '/api/' + '3075' + '.csv'
        conn.request('GET', dirURL,"",{'X-PachubeApiKey':self._key})
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
        

if __name__ == "__main__":
    pu = PachubeUploader("3033","c6fd7b812e1288f99288ea26aa4f680b1bfe34a473cafd4ca78c8a1ae5993286")
    ck_serial = True
    for arg in sys.argv:
        if arg == "dead":
            print "Running in Dead Mode."
            ck_serial = False
    if ck_serial == True:
        hr_timer = Timer(30.0)
    dir_timer = Timer(7.0)
    if ck_serial == True:
        hr = HeartReader("/dev/tty.usbserial-A6007z5N")
    direction = ""
    while(1):
        if ck_serial == True:
            if(hr_timer.go() == 1):
                result = hr.readHeart()
                pu.putCSV(str(result))
        if(dir_timer.go() == 1):
            r = pu.getCSV()
            if(r != direction):
                direction = r
                print r
            
