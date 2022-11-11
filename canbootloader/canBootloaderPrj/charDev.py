import sys
import time
import os
from abc import ABCMeta, abstractmethod

class CCharDev(object):
    __metaclass__ = ABCMeta

    _dataQue = bytearray(b'')
    _latestCharDevVersion = 0x10
    @abstractmethod
    def open(self):
        pass

    @abstractmethod
    def close(self):
        pass

    @abstractmethod
    def write(self, charflow):
        pass
    
    @abstractmethod
    def read(self, len):
        pass

    @abstractmethod
    def ioctl(self, cmd, arg):
        pass

    def clearReadBuf(self):
        self._dataQue.clear()
    
import socket
class CUdpCharDev(CCharDev):
    def __init__(self, primeAddress = ('192.168.192.4',5003), seconAddress = ('192.168.192.4',19204)):
        #primeAddress: ip+port for command jump to bootloader
        self._primeAddress = primeAddress
        #address: ip+port for IAP operation
        self._seconAddress = seconAddress
        #address: abstructed ip+port 
        self._address = seconAddress
        self._so = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print('init target IP:%s, port:%d'%(self._address[0], self._address[1]))
        self._so.settimeout(1)
        self._latestCharDevVersion = 0x13
    
    def open(self):
        pass
    
    def close(self):
        self._so.close()

    def write(self, data):
        self._so.sendto(data, self._address)

    def read(self, bufflen):
        while(True):
            if(len(self._dataQue) >= bufflen):
                ret = self._dataQue[0:bufflen]
                self._dataQue[0:bufflen] = b''
                return ret
            else:
                try:
                    self._dataQue += self._so.recvfrom(1024)[0]
                except socket.timeout:
                    return b''

    def ioctl(self, cmd, arg = 0):
        if(cmd == "usePrimeAddress"):
            self._address = self._primeAddress
        elif(cmd == "useSeconAddress"):
            self._address = self._seconAddress
        elif (cmd == "readTimeout"):
            self._so.settimeout(arg)
        elif (cmd == "getReadTimeout"):
            arg = self._so.gettimeout()
        elif (cmd == "clearReadBuf"):
            self._dataQue.clear()
        elif(cmd == "latestVersion"):
            arg[0] = self._latestCharDevVersion
        else:
            print("unknow param!")
            os.system("pause")
            quit()

import ctypes
from ctypes import *
class CCanCharDev(CCharDev):
    class _VCI_CAN_OBJ(Structure):
        _fields_ = [("ID", c_uint32),
                    ("TimeStamp", c_uint32),
                    ("TimeFlag", c_byte),
                    ("SendType", c_byte),
                    ("RemoteFlag", c_byte),
                    ("ExternFlag", c_byte),
                    ("DataLen", c_byte),
                    ("Data", c_ubyte * 8),
                    ("Reserved", c_byte * 3)]

    _BUFF_LEN = 512
    _VCI_CAN_OBJ_ARRAY = _VCI_CAN_OBJ*_BUFF_LEN

    _DEV_TYPE = 4
    _DEV_IDX = 0
    _dll = ctypes.windll.LoadLibrary('ControlCAN.dll')
    def __init__(self, primeAddress = (0x5004, 0x5005), seconAddress = (0x5004, 0x5005), USB_CAN_CH = 0):
        #primeAddress: upstreamID + downstreamID for command jump to bootloader
        self._primeAddress = primeAddress
        #address: upstreamID + downstreamID for IAP operation
        self._seconAddress = seconAddress
        #address: abstructed upstreamID + downstreamID 
        self._upId = seconAddress[0]
        self._downId = seconAddress[1]

        self._DEV_CH = USB_CAN_CH
        self._rxbuff = CCanCharDev._VCI_CAN_OBJ_ARRAY()
        self._readTimeout = 1
        self._latestCharDevVersion = 0x11
        
    
    def open(self):
        canInitArrayType = c_byte*16
        canInitArray = canInitArrayType()
        canInitArray[0] = 0x08
        canInitArray[1] = 0x00
        canInitArray[2] = 0x00
        canInitArray[3] = 0x80

        canInitArray[4] = 0xff
        canInitArray[5] = 0xff
        canInitArray[6] = 0xff
        canInitArray[7] = 0xff

        canInitArray[8] = 0x00
        canInitArray[9] = 0x00
        canInitArray[10] = 0x00
        canInitArray[11] = 0x00

        canInitArray[12] = 0x00
        canInitArray[13] = 0x01
        canInitArray[14] = 0x1c
        canInitArray[15] = 0x00
        #open device
        if(CCanCharDev._dll.VCI_OpenDevice(CCanCharDev._DEV_TYPE, CCanCharDev._DEV_IDX, self._DEV_CH) == 1):
            print('UsbCanTool Open Success')
        else:
            print('UsbCanTool Open falid')
            print("傻X,是不是CAN卡又没关？")
            sys.stdout.flush()
            os.system('pause')
            quit()

        #init device
        if(CCanCharDev._dll.VCI_InitCAN(CCanCharDev._DEV_TYPE, CCanCharDev._DEV_IDX, self._DEV_CH, byref(canInitArray)) == 1):
            print('device init ok!')
        else:
            print('device init failed...')
            sys.stdout.flush()
            os.system('pause')
            quit()

        #start channel
        if(CCanCharDev._dll.VCI_StartCAN(CCanCharDev._DEV_TYPE, CCanCharDev._DEV_IDX, self._DEV_CH) == 1):
            print('Channel%d start ok!'%(self._DEV_CH + 1))
        else:
            print('Channel%d start failed...'%(self._DEV_CH + 1))
            sys.stdout.flush()
            os.system('pause')
            quit()
        
    def close(self):
        CCanCharDev._dll.VCI_CloseDevice(CCanCharDev._DEV_TYPE, 0)
        print('UsbCanTool Close')
        sys.stdout.flush()

    def read(self, bufflen):
        timecount = 0
        while(True):
            if(timecount > self._readTimeout * 100):
                print("read timeout")
                return b''
                
            if(len(self._dataQue) >= bufflen):
                ret = self._dataQue[0:bufflen]
                self._dataQue[0:bufflen] = b''
                return ret
            else:
                frame_num = CCanCharDev._dll.VCI_Receive(CCanCharDev._DEV_TYPE,  
                    CCanCharDev._DEV_IDX, 
                    self._DEV_CH, 
                    self._rxbuff,
                    CCanCharDev._BUFF_LEN, 0)
                if(0 == frame_num):
                    time.sleep(0.01)
                    timecount = timecount + 1
                    continue
                for i in range(0, frame_num):
                    if(self._rxbuff[i].ID != self._upId):
                        continue
                    for j in range(0, self._rxbuff[i].DataLen):
                        self._dataQue.append(self._rxbuff[i].Data[j])

    def _send_frame(self, frame):
        while True:
            ret = CCanCharDev._dll.VCI_Transmit(CCanCharDev._DEV_TYPE, 
                CCanCharDev._DEV_IDX, 
                self._DEV_CH, 
                frame,1)
            if ret == 1:
                break
            else:
                print('send error code:',ret)
        
    def write(self, charflow):
        count = 0
        send_frame = CCanCharDev._VCI_CAN_OBJ()
        send_frame.ID = self._downId
        send_frame.SendType = 1
        send_frame.RemoteFlag = 0
        send_frame.ExternFlag = 1
        send_frame.DataLen = 8
        for byte in charflow:
            send_frame.Data[count] = byte
            count = count + 1
            if (count == 8):
                count = 0
                send_frame.DataLen = 8
                self._send_frame(byref(send_frame))
        if(count > 0):
            send_frame.DataLen = count
            self._send_frame(byref(send_frame))
    
    def ioctl(self, cmd, arg = 0):
        if(cmd == "usePrimeAddress"):
            self._upId = self._primeAddress[0]
            self._downId = self._primeAddress[1]
        elif(cmd == "useSeconAddress"):
            self._upId = self._seconAddress[0]
            self._downId = self._seconAddress[1]
        elif (cmd == "readTimeout"):
            self._readTimeout = arg
        elif (cmd == "getReadTimeout"):
            arg = self._readTimeout
        elif (cmd == "clearReadBuf"):
            CCanCharDev._dll.VCI_ClearBuffer(CCanCharDev._DEV_TYPE, 
                CCanCharDev._DEV_IDX, 
                self._DEV_CH)
            self._dataQue.clear()
        elif(cmd == "latestVersion"):
            arg[0] = self._latestCharDevVersion
        else:
            print("unknow param!")
            os.system("pause")


        