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

PROCESSING_TIME = 1
TIME_OUT_IN_SECONDS = 5
BROADCAST_TIME_IN_SECONDS = 5

def init():
    wiringpi.wiringPiSetup()
    serial = wiringpi.serialOpen(DEVICE_PATH, BAUDRATE)
    
    if serial > -1:
        print('Connection with module established')
        
        print('Updating connect ability...')
        wiringpi.serialPrintf(serial, (AT_CONT + '1'))
        time.sleep(PROCESSING_TIME)
        
        print('Updating module name...')
        wiringpi.serialPrintf(serial, AT_NAME)
        time.sleep(PROCESSING_TIME)
        
    else:
        print('Couldn\'t establish connection with module =(')
        
    return serial

def broadcast(serial, data):
    if serial > -1:
        
        print('Putting module in broadcast mode...')
        wiringpi.serialPrintf(serial, AT_ROLE_S)
        time.sleep(PROCESSING_TIME)
                
        print('Start broadcast...')
        broadcast = chr(STX) + data + chr(ETX)
        wiringpi.serialPrintf(serial, (AT_AVDA_BASIS + broadcast))
        time.sleep(PROCESSING_TIME)
        
        print('Broadcasting: \'' + broadcast + '\'')
        time.sleep(BROADCAST_TIME_IN_SECONDS)
        
        print('End broadcast')
    else:
        print('Couldn\'t establish connection with module =(')

def listen(serial):
    receivedData = ''
    
    if serial > -1:
        
        print('Putting module in listen mode...')
        wiringpi.serialPrintf(serial, AT_ROLE_M)
        time.sleep(PROCESSING_TIME)
             
        print('Disconnect previous slave...')    
        wiringpi.serialPrintf(serial, AT_CLEAR)
        time.sleep(PROCESSING_TIME)
        
        print('Start listen...')
        hasTransmissionArrived = False
        startTime = time.time()
        
        while True:
                if wiringpi.serialDataAvail(serial):
                        b = wiringpi.serialGetchar(serial)
                
                        if b == STX:
                                hasTransmissionArrived = True
                                print('Transmission has arrived')
                        elif b == ETX:
                                #Transmission has ended
                                print('Data received: ' + receivedData)
                                print('End listen')
                                return receivedData
                        elif hasTransmissionArrived:
                                receivedData += chr(b)
                elif time.time() > startTime + TIME_OUT_IN_SECONDS:
                        print('Time-out listen')
                        break
    else:
        print('Couldn\'t establish connection with module =(')
        
    return -1
