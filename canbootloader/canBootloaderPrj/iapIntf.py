import sys
import os
import time
import charDev

print('start')

class CIapDev(object):

    ACK = b'\x79'
    NACK = b'\x1F'

    byteReset = bytearray(b'\x09\x00\x00\x00\xff\xff\xff\xff')
    byteJump2BL = bytearray(b'\x07\x00\x00\x00\xff\xff\xff\xff')
    byteBoot2BL = bytearray(b'\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f')

    byteWriteMemCmd = bytearray(b'\x31\xCE')
    byteReadMemCmd = bytearray(b'\x11\xEE')
    byteGoCmd = bytearray(b'\x21\xDE')
    byteGetFirmwareVersion = bytearray(b'\x01\xFE')

    byteBootParam_BL = bytearray(b'\x66\x66\x2b\x2b')
    byteBootParam_APP = bytearray(b'\xaa\xaa\x55\x55')

    def __init__(self, charDevice, flashMap):
        self._charDev = charDevice
        self._addrBootLoader = flashMap[0]
        self._addrBootParam = flashMap[1]
        self._addrApp = flashMap[2]
        self._charDev.open()

    @staticmethod
    def getXor(val):
        xor = 0
        for byte in val:
            xor ^= byte
        return xor
    
    @staticmethod
    def getBytesFromUint32(val):
        val0 = val & 0xFF
        val1 = (val & 0xFF00) >> 8
        val2 = (val & 0xFF0000) >> 16
        val3 = (val & 0xFF000000) >> 24
        return bytearray([val3,val2,val1,val0])

    @staticmethod
    def isAllBytesFF(val):
        for byte in val:
            if byte != 255:
                return False
        return True

    def confirm_ack(self):
        stmback = self._charDev.read(1)
        if stmback.__len__() == 0 or stmback[0] != CIapDev.ACK[0]:
            print('not ack:'+ str(stmback))
            self._charDev.clearReadBuf()
            return False
        else:
            return True

    def sendto_stm32(self, val):
        self._charDev.ioctl("clearReadBuf")
        self._charDev.write(val)

    def resetBoard(self):
        self._charDev.ioctl("usePrimeAddress")
        while(True):
            self.sendto_stm32(CIapDev.byteReset)
            stmback = self._charDev.read(4)
            if(stmback.__len__() == 0):
                print("timeout")
            elif(stmback[0] == CIapDev.NACK[0]):
                print("in booloader")
                break
            elif(stmback[0] == 0x09):
                print('reset ok')
                break
            else:
                print("error: get code ", stmback)
            sys.stdout.flush()

    def jumpToAddress(self, address = 0):
        if(0 == address):
            address = self._addrApp
        print('jump to address: 0x%X'%address)
        while(True):
            self.sendto_stm32(CIapDev.byteGoCmd)
            print('send go cmd')
            if(self.confirm_ack() != True):
                continue
            cmd = CIapDev.getBytesFromUint32(address)
            cmd.append(self.getXor(cmd))
            time.sleep(0.01)
            self.sendto_stm32(cmd)
            if(self.confirm_ack() != True):
                continue
            else:
                break
            print('finished')
            sys.stdout.flush()
    
    def jumpToApp(self):
        self.jumpToAddress(self._addrApp)


    def jumpToBootloader(self):
        print('jump to bootloader')
        sys.stdout.flush()
        self._charDev.ioctl("usePrimeAddress")
        while True:
            self.sendto_stm32(CIapDev.byteJump2BL)
            stmback = self._charDev.read(1)
            if(stmback == b''):
                print('timeout')
                break
                # continue
            elif(stmback == CIapDev.NACK):
                print('already in bootloader')
                self._charDev.ioctl("useSeconAddress")
                break
            elif(stmback == b'\x07'):
                print('from application')
                #0.2 second delay is important!
                time.sleep(1)
                self._charDev.ioctl("useSeconAddress")
                break
            else:
                print('get byte', stmback)
                continue
            print('resend jump command')
            sys.stdout.flush()
    
    def resetToBootloader(self):
        print('reset to bootloader')
        isInApp = True
        self.sendto_stm32(CIapDev.byteReset)
        stmback = self._charDev.read(1)
        if(stmback.__len__() > 0 and stmback[0] == CIapDev.NACK[0]):
            print("already in bootloader")
            isInApp = False
        elif (stmback.__len__() > 0 and stmback[0] == CIapDev.byteReset[0]):
            self._charDev.ioctl("clearReadBuf")
            print("jump from app")
        else:
            print("don't know where it is, get", stmback)
            os.system('pause')
            quit()

        prevTimeout = 1
        self._charDev.ioctl('getReadTimeout', prevTimeout)
        self._charDev.ioctl('readTimeout', 0.001)
        while(isInApp):
            self._charDev.write(CIapDev.byteBoot2BL)
            time.sleep(0.02)
            stmback = self._charDev.read(1)
            if(stmback.__len__() > 0 and CIapDev.NACK[0] == stmback[0]):
                print('jump success')
                time.sleep(0.1)
                self._charDev.ioctl("readTimeout", 1)
                break
            else:
                # print('error: get', stmback)
                pass
        
        self._charDev.ioctl("clearReadBuf")
        self._charDev.ioctl('readTimeout', prevTimeout)
            

    def writeBootParam(self, val:'bootpram enum', address = 0):
        if(val != CIapDev.byteBootParam_BL 
        and val != CIapDev.byteBootParam_APP):
            print('boot param error:', val)
            return
        if(0 == address):
            address = self._addrBootParam

        print('write bootparam')
        while(True):
            time.sleep(0.01)
            self.sendto_stm32(CIapDev.byteWriteMemCmd)
            if(self.confirm_ack() != True):
                continue

            cmd = CIapDev.getBytesFromUint32(address)
            cmd.append(CIapDev.getXor(cmd))
            self.sendto_stm32(cmd)
            if(self.confirm_ack() != True):
                continue

            cmd = bytearray(b'\x03') + val
            cmd.append(CIapDev.getXor(cmd))
            self.sendto_stm32(cmd)
            if(self.confirm_ack() != True):
                print('write failed, retry')
                continue
            else:
                break

    def getBootLoaderVersion(self):
        self._charDev.ioctl('useSeconAddress')
        print('read firmware version with second address')
        while True:
            self.sendto_stm32(CIapDev.byteGetFirmwareVersion)
            stmback = self._charDev.read(5)
            if(stmback == b''):
                print('read timeout, maybe port unmatch, switch to primeAddress')
                self._charDev.ioctl('usePrimeAddress')
                continue
            elif(stmback[0] != CIapDev.ACK[0]):
                print('not ack', stmback)
                continue
            else:
                break
        latestVersion = [0x00]
        self._charDev.ioctl("latestVersion", latestVersion)
        if(latestVersion[0] != stmback[1]):
            print("Current firmware version V%d.%d, the latest is V%d.%d"
                %( stmback[1] >> 4,  stmback[1] & 0xF, latestVersion[0] >> 4, latestVersion[0] & 0xF,))
            print("Firmware version mismatch, do you want to continue? y/n")
            sys.stdout.flush()
            if('y' != input()):
                quit()
        return 0x00

    def loadBin(self, filename, address = 0):
        if(0 == address):
            address = self._addrApp
        SEND_DATA_LEN = 256
        tail = filename[-4:]
        if tail != ".bin":
            print("the extension is not .bin")
            return False
        f = open(filename,'rb')
        data1 = f.read()
        # 不足4的倍数的补齐
        if len(data1) % 4 != 0:
            pack_reset = (4 - len(data1) % 4)
            temp = []
            for i in range(0, len(data1)):
                temp.append(data1[i])
            for i in range(0, pack_reset):
                temp.append(0x00)
            data = bytes(temp)
        i = 0
        length = len(data)
        while i < length:
            nowDownloadAddress = address + i
            print("write address 0x%X" % nowDownloadAddress)
            sys.stdout.flush()
            j = i + SEND_DATA_LEN
            if j > length:
                j = length
            slip = data[i:j]
            slipLen = j - i
            slipArray = bytearray(slip)
           
            slipArray.insert(0,slipLen - 1)
            slipArray.append(CIapDev.getXor(slipArray))
            # send head
            self.sendto_stm32(CIapDev.byteWriteMemCmd)
            if self.confirm_ack() != True:
                continue
            #send address
            byteAddress = CIapDev.getBytesFromUint32(nowDownloadAddress)
            byteAddress.append(CIapDev.getXor(byteAddress))
            self.sendto_stm32(byteAddress)
            if self.confirm_ack() != True:
                continue
            #send data
            self.sendto_stm32(slipArray)
            if self.confirm_ack() != True:
                continue
            i = j

    def readBin(self, filename, address = 0):
        # get flasher file, read only, binary
        if(0 == address):
            address = self._addrApp
        READ_DATA_LEN = 256
        f = open(filename,'wb')
        i = 0
        loopCondition = True
        length = 80000
        while i < length:
            nowReadAddress = address + i
            print("read address 0x%X" % nowReadAddress)
            sys.stdout.flush()
            j = i + READ_DATA_LEN
            if j > length:
                j = length
            slipLen = j - i

            # send head
            self.sendto_stm32(CIapDev.byteReadMemCmd)
            if self.confirm_ack() != True:
                continue

            #send address
            byteAddress = CIapDev.getBytesFromUint32(nowReadAddress)
            byteAddress.append(CIapDev.getXor(byteAddress))
            self.sendto_stm32(byteAddress)
            if self.confirm_ack() != True:
                continue

            #send datalength
            double_datalen = bytearray([slipLen-1, slipLen-1])
            self.sendto_stm32(double_datalen)
            if self.confirm_ack() != True:
                continue

            #read data
            stmback = self._charDev.read(256)
            checkbyte = self._charDev.read(1)
            if self.getXor(bytearray(stmback)) == checkbyte[0]:
                if self.isAllBytesFF(stmback):
                    print("all byte 0xFF, read finished")
                    f.close()
                    break
                else:
                    f.write(stmback)
            else:
                print("check sum failed")
                print(stmback)
                print("calc = 0x%X" %self.getXor(bytearray(stmback)))
                print("get = 0x%X" % checkbyte[0])
                continue

            i = j

