#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  server.py
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
import requests

DATA_PACKAGE_START = 'T0V1'
DATA_PARKING_SPACE_ONE_DIGIT= '00'
DATA_PARKING_SPACE_TWO_DIGITS = '0'
DATA_PACKAGE_MIDDLE = 'D1'

LINK_STATUS = 'https://parking.ganaarjenieuwewebsite.nl/api/status?'
LINK_RESERVATIONS = 'https://parking.ganaarjenieuwewebsite.nl/api/reservations?'

MAX_PARKING_SPACES = 5

LENGTH_BROADCAST = 12
LENGTH_DATA_PACKAGE = LENGTH_BROADCAST - 2

def send(dataPackage):
    if len(dataPackage) == LENGTH_DATA_PACKAGE:
        #sort the data from the data package
        #timeToLive = dataPackage[1:-8]
        #gatewayID = dataPackage[3:-6]
        vakID = dataPackage[4:-3]
        richting = int(dataPackage[8:-1])
        waarde = chr(int(dataPackage[9:]) + 1 + ord('0'))
        
        
        #send the payload to the server
        if richting > 0:
            print('Kan geen reservaties sturen naar de server!')
        else:
            print('Sensor status versturen naar server...')
            r = requests.post(
            LINK_STATUS + 'parking_space_id=' + vakID + '&status=' + waarde)
            print(r.url)
        
            status()
            print('Sensor status verstuurd')
    else:
        print 'Invalid data package!'
    
def status():
    r = requests.get(LINK_STATUS)
    print(r.text)
    
def receive():
    #ask the server for current reservations
    r = requests.get(LINK_RESERVATIONS)
    reservations = r.json()

    dataPackages = [] #data that is going to be broadcasted
    
    #loop through all the parking spaces to compare there reservation status
    for parkingSpaceID in range(MAX_PARKING_SPACES):
        #build and add a data package
        parkingSpaceIDChar = chr(parkingSpaceID + 1 + ord('0'))
        #print(parkingSpaceIDChar)
        if parkingSpaceIDChar in reservations:
            dataPackages.append(
            DATA_PACKAGE_START + DATA_PARKING_SPACE_ONE_DIGIT + parkingSpaceIDChar + DATA_PACKAGE_MIDDLE + '1'
            )
        else:
            dataPackages.append(
            DATA_PACKAGE_START + DATA_PARKING_SPACE_ONE_DIGIT + parkingSpaceIDChar + DATA_PACKAGE_MIDDLE + '0'
            )
    
    #send back the data packages ready for broadcasting
    print('Data packages ready for broadcast:')
    print(dataPackages)
    return dataPackages
    
    
