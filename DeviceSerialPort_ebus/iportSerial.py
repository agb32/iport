#!/usr/bin/python
"""Sends a serial command via darc..."""

import sys
import string
import time
import numpy
import darc

def sendCmd(cmd,prefix="",cam=0):
    """Sends a cameralink serial command through an iport device, using the darc interface.  Packet format is:
    4 bytes 0
    4 bytes for number of characters following
    The characters
    Padding up to 4 bytes"""
    print "%s (cam %d)"%(cmd,cam)
    d=darc.Control(prefix)
    if cmd[-2:]!="\r\n":
        cmd=cmd+"\r\n"
    l=len(cmd)
    a=numpy.zeros((4+(l+3)//4,),numpy.uint32)
    a[0]=cam
    a[1]=0x40058000 #the address
    a[2]=0
    a[3]=l
    a[3]=a[3].byteswap()
    cmd+="\0"*((4-l%4)%4)
    a[4:]=numpy.fromstring(cmd,dtype=numpy.uint32)
    for i in a:
        print hex(i)
    d.Set("aravisMem",a.view(numpy.int32))

    #Now, I think the message has to and with 0x093e02 (byteswapped), and has to be preceeded by 0x09  (and maybe some extra zeros as well).
    #So, work backwards from the end of the message...
    #Msg always ends with \n, so, the last 4 bytes can be assigned.
    #a[-1]=0x023e090a
    #cmd=cmd[:-1]#strip off the "\n" (0x0a).
    #b=numpy.fromstring(cmd,dtype=numpy.uint8)
    #pad=(4-(len(cmd)+1)%4)%4
    #padarr=numpy.fromstring("\0"*pad+chr(9),dtype=numpy.uint8)
    #c=numpy.concatenate([padarr,b]).view(numpy.uint32)
    #a[-1-c.size:-1]=c
    #for i in a:
    #    print hex(i)
    #d.Set("aravisMem",a.view(numpy.int32))


txt="""Selection of available commands:
cooling 20
fps 5         00 00 00 00 00 00 00 07 66 70 73 20 35 0d 0a 00
fps 50        00 00 00 00 00 00 00 08 66 70 73 20 35 30 0d 0a
fps 500       00 00 00 00 00 00 00 09 66 70 73 20 35 30 30 0d 0a 35 37 02
fps 1000
gain 1        00 00 00 00 00 00 00 08 67 61 69 6e 20 31 0d 0a
gain 10
gain 100
gain 1000
test on       00 00 00 00 00 00 00 09 74 65 73 74 20 6f 6e 0d 0a 45 9c 03
test off      00 00 00 00 00 00 00 0a 74 65 73 74 20 6f 66 66 0d 0a 14 04
led on
led off
cooling reset   00 00 00 00 00 00 00 0f 63 6f 6f 6c 69 6e 67 20 72 65 73 65 74 0d 0a 00
cooling -60
cooling 1    00 00 00 00 00 00 00 0b 63 6f 6f 6c 69 6e 67 20 31 0d 0a 03
cooling 20   00 00 00 00 00 00 00 0c 63 6f 6f 6c 69 6e 67 20 32 20 0d 0a
cooling on   00 00 00 00 00 00 00 0c 63 6f 6f 6c 69 6e 67 20 6f 6e 0d 0a
cooling off (don't use unless previously done a cooling 20 and waited)
             00 00 00 00 00 00 00 0d 63 6f 6f 6c 69 6e 67 20 6f 66 66 0d 0a 00 00 00
bias on
bias off
flat on
flat off
protection reset    - must be used at startup to enable gain.
shutter on
shutter off
shutter internal
shutter external
shutter single
shutter burst
shutter sweep 0  (or 1 or 2)  - 0 is off, 1 is continuous, 2 is continuous started by an external trigger.
shutter pulse #ns  (set duration of shutter opening in ns - with an increment of 9.21 ns)
shutter blanking #ns (duration of blanking between 2 pulses in ns - increments of 9.21ns)
shutter position #ns (delay between frame trigger and first pulse - in 9.21ns increments).
shutter step #ns (delay in ns added to each frame shutter starting position when on sweep mode).  
shutter end #ns (time limit in ns that will reset the sweep)
shutter count #n Number of sequential pulses to fire in burst mode.

synchro on   switch to externally triggered mode (frame rate).  Low value 
             triggers a frame transfer and readout (so if it remains low, 
             will operate at 1.5kHz).
synchro off  switch back to internal triggering.

--prefix=WHATEVER
--cam=DARC CAM NUMBER


Or:

setup LaserFreq ShutterOpenTime(us) ShutterDelay(us - 666 to avoid readout+extra optionally to delay for LGS height) CameraFrameRate
"""

def prepareShutter(laserfreq,exptime,delay,frate,on=1,prefix="",cam=0):
    """Sets up parameters for laser freq, 
    with exposure time exptime (in us) 
    and camera frame rate frate, 
    and delay from start of frame transfer of delay (in us).
    """
    period=(1./laserfreq)*1e9
    frameperiod=(1./frate)*1e9
    sendCmd("shutter off",prefix,cam)
    time.sleep(0.05)
    sendCmd("shutter internal",prefix,cam)
    time.sleep(0.05)
    sendCmd("shutter burst",prefix,cam)
    time.sleep(0.05)
    sendCmd("shutter pulse %d"%int(exptime*1000),prefix,cam)
    time.sleep(0.05)
    bl=int(period-int(exptime*1000))
    sendCmd("shutter blanking %d"%bl,prefix,cam)
    time.sleep(0.05)
    sendCmd("shutter position %d"%int(delay*1000),prefix,cam)
    time.sleep(0.05)
    n=int(frameperiod-int(delay*1000))//(bl+int(exptime*1000))
    sendCmd("shutter count %d"%n,prefix,cam)
    time.sleep(0.05)
    if on:
        sendCmd("shutter on",prefix,cam)

if __name__=="__main__":
    prefix=""
    cam=0
    if len(sys.argv)>1:
        cmdlist=list(sys.argv[1:])
        newlist=[]
        for cmd in cmdlist:
            if cmd.startswith("--prefix="):
                #cmdlist.remove(cmd)
                prefix=cmd[9:]
            elif cmd.startswith("--cam="):
                #cmdlist.remove(cmd)
                cam=int(cmd[6:])
            else:
                newlist.append(cmd)
        cmdlist=newlist
        if cmdlist[0]=="setup":
            if len(cmdlist)!=5:
                print "Usage: %s setup LaserFreq ShutterOpenTime(us) ShutterDelay(us - 666 to avoid readout) CameraFrameRate"%sys.argv[0]
            else:
                laserfreq=float(cmdlist[1])
                shuttertime=float(cmdlist[2])
                delay=float(cmdlist[3])
                framerate=float(cmdlist[4])
                prepareShutter(laserfreq,shuttertime,delay,framerate,prefix=prefix,cam=cam)
        else:
            cmd=string.join(cmdlist," ")
            sendCmd(cmd,prefix,cam)
    else:
        print txt
