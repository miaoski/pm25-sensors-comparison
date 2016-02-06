# Read from MCU
# Data format:
# A4 77 105 111|G3 62 92 115|SHT31 13.37 46.13|AM2302 14.10 38.20|AM2320 14.80 46.50|

import serial
import requests
import threading
from datetime import datetime

INTERVAL = 60
APIKEY = open('thingspeak.key').read().strip()
ser = serial.Serial("/dev/ttyS0", 57600)
FIELDS = []
FIELDS_LOCK = threading.Lock()


def avg(l):
    n = len(l)
    s = sum(map(float, l))
    return s/n


def timed_task():
    from itertools import imap
    submit_thread = threading.Timer(INTERVAL, timed_task)
    submit_thread.daemon = True
    submit_thread.start()
    if len(FIELDS) == 0:
        return
    FIELDS_LOCK.acquire()
    it = list(imap(None, *FIELDS))
    FIELDS_LOCK.release()
    fields = [avg(x) for x in it]
    url = 'https://api.thingspeak.com/update?api_key=%s&field1=%f&field2=%f&field3=%f&field4=%f&field5=%f&field6=%f&field7=%f&field8=%f' % tuple([APIKEY, ] + fields)
    print 'url =', url
    r = requests.get(url)
    if r.status_code != requests.codes.ok:
        print 'Failed to submit', r.status_code


def logme(s):
    t = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S   ') + s
    with open('compare.log', 'a') as f:
        f.write(t + '\n')
    print t


def main():
    l = ser.readline()
    try:
        (a4, g3, sht31, am2302, am2320, _) = l.split('|')
    except:
        return
    logme('|'.join((a4, g3, sht31, am2302, am2320)))
    FIELDS_LOCK.acquire()
    FIELDS.append((sht31.split()[1], sht31.split()[2], 
                   am2302.split()[1], am2302.split()[2],
                   am2320.split()[1], am2320.split()[2],
                   g3.split()[2], a4.split()[2]))
    FIELDS_LOCK.release()


if __name__ == '__main__':
    try:
        timed_task()
        while 1:
            main()
    except KeyboardInterrupt:
        ser.close()
        print "Exit."
