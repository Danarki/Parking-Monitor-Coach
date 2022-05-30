#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  module.py
#  
#  Copyright 2022  <pi@raspberrypi>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
# 
import time
import wiringpi

DEVICE_PATH = '/dev/serial0'
BAUDRATE = 9600

BROADCAST_TIME_IN_SECONDS = 5
    
STX = 0x02 #tart of Text
ETX = 0x03 #End of Text

MODULE_NAME = 'Gateway-1'

AT_NAME = 'AT+NAME=' + MODULE_NAME
AT_CONT = 'AT+CONT='

AT_ROLE_M = 'AT+ROLE=M'
AT_CLEAR = 'AT+CLEAR'

AT_ROLE_S = 'AT+ROLE=S'
AT_AVDA_BASIS = 'AT+AVDA='

STATUS_NAME = 'OKsetNAME:' + MODULE_NAME

TIME_OUT_IN_SECONDS = 2

def __waitForOK__(serial):
        if serial > -1:
                validResponse = False
                responseData = ''
                
                while True:
                        if wiringpi.serialDataAvail(serial):
                                c = chr(wiringpi.serialGetchar(serial))
                                
                                if c == 'O':
                                        responseData += c
                                elif c == 'K':
                                        #Response has ended
                                        validResponse = True
                                        responseData += c
                                        break
                                elif c == chr(STX) or c == chr(ETX):
                                        #missed the OK status
                                        responseData = 'UNKNOWN'
                                        break
                                        
                print('Wait for Status: ' + responseData)
                
                if validResponse:
                        return True
                else:
                        time.sleep(TIME_OUT_IN_SECONDS)
        else:
                print('Couldn\'t establish connection with module =(')
        
        return False
        
def __waitForX__(serial, x):
        if serial > -1:
                validResponse = False
                responseData = ''
                
                while True:
                        if wiringpi.serialDataAvail(serial):
                                c = chr(wiringpi.serialGetchar(serial))
                                responseData += c
                                 
                                if responseData == x:
                                        #x response received
                                        validResponse = True
                                        break;
                                elif c == chr(STX) or c == chr(ETX):
                                        #missed the OK status
                                        responseData = 'UNKNOWN'
                                        break
                                
                                
                print('Wait for \'' + x + '\' Status: ' + responseData)
                
                if validResponse:
                        return True
                else:
                        time.sleep(TIME_OUT_IN_SECONDS)
        else:
                print('Couldn\'t establish connection with module =(')
        
        return False        
        
def __enableModule__(serial):
        if serial > -1:
                while True:
                        print('Enabling module...')
                        wiringpi.serialPrintf(serial, (AT_CONT + '1'))
                        if __waitForOK__(serial):
                                time.sleep(1)
                                break
        
                print('Module ready')
        else:
                print('Couldn\'t establish connection with module =(')
        
def __disableModule__(serial):
        if serial > -1:
                while True:
                        print('Disabling module...')
                        wiringpi.serialPrintf(serial, (AT_CONT + '0'))
                        if __waitForOK__(serial):
                                time.sleep(1)
                                break
        
                print('Module disabled')
        else:
                print('Couldn\'t establish connection with module =(')

def init():
    wiringpi.wiringPiSetup()
    serial = wiringpi.serialOpen(DEVICE_PATH, BAUDRATE)
    
    if serial > -1:
        print('Connection with module established')
        
        #__disableModule__(serial)
        __enableModule__(serial)
        
        while True:
                print('Updating module name...')
                wiringpi.serialPrintf(serial, AT_NAME)
                if __waitForX__(serial, STATUS_NAME):
                        time.sleep(1)
                        break
        
    else:
        print('Couldn\'t establish connection with module =(')
        
    return serial

def broadcast(serial, data):
    if serial > -1:
        
        while True:
                print('Putting module in broadcast mode...')
                wiringpi.serialPrintf(serial, AT_ROLE_S)
                if __waitForOK__(serial):
                        time.sleep(1)
                        break
                        
        broadcast = chr(STX) + data + chr(ETX)
        #__enableModule__(serial)
        while True:
                print('Start broadcast...')
                wiringpi.serialPrintf(serial, (AT_AVDA_BASIS + broadcast))
                if __waitForOK__(serial):
                        time.sleep(1)
                        break
        
        print('Broadcasting: \'' + broadcast + '\'')
        time.sleep(BROADCAST_TIME_IN_SECONDS)
        
        print('End broadcast')
        #__disableModule__(serial)
    else:
        print('Couldn\'t establish connection with module =(')

def listen(serial):
    receivedData = ''
    
    if serial > -1:
        
        while True:
                print('Putting module in listen mode...')
                wiringpi.serialPrintf(serial, AT_ROLE_M)
                if __waitForOK__(serial):
                        time.sleep(1)
                        break
                        
        while True:
                print('Disconnect previous slave...')
                wiringpi.serialPrintf(serial, AT_CLEAR)
                if __waitForOK__(serial):
                        time.sleep(1)
                        break
        
        #__enableModule__(serial)
        print('Start listen...')
        b = ''
        hasTransmissionArrived = False
        
        while True:
            if wiringpi.serialDataAvail(serial):
                b = wiringpi.serialGetchar(serial)
                #print('In: ' + chr(b))
                
                if b == STX:
                        hasTransmissionArrived = True
                        print('Transmission has arrived')
                elif b == ETX:
                        #Transmission has ended
                        break
                elif hasTransmissionArrived:
                        receivedData += chr(b)
                        
        print('Data received: ' + receivedData)
        print('End listen')
        #__disableModule__(serial)
    else:
        print('Couldn\'t establish connection with module =(')
        
    return receivedData
