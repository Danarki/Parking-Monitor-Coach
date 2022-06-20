#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  gateway.py
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
import module
import server

MAX_CONNECTION_ATTEMPTS = 3

DIVIDER = '- - - - - - - - - -'

def main(args):
    serial = module.init()
    
    while True:
        print(DIVIDER)
        newReservations = server.receive()
        
        for newReservation in newReservations:
            
            sendBroadcastID = newReservation[4:-3]
            unreachableParkingSpaceCounter = 0 #the amount of attempts to reach a certain parking space
            
            while True:
                print(DIVIDER)
                
                #after too many failed attempts, the parking space is seen as unreachable
                if unreachableParkingSpaceCounter >= MAX_CONNECTION_ATTEMPTS: 
                    print('Parking space ' + sendBroadcastID + ' unreachable')
                    unreachableParkingSpaceCounter = 0
                    break
        
                print('(Attempt ' + str(unreachableParkingSpaceCounter + 1) + ') Trying to reach parking space ' + sendBroadcastID + '...')
                module.broadcast(serial, newReservation)
                
                receivedBroadcast = module.listen(serial)
                if receivedBroadcast == -1: #time-out listen
                    unreachableParkingSpaceCounter += 1
                else:
                    receivedBroadcastID = receivedBroadcast[4:-3]
            
                    if sendBroadcastID == receivedBroadcastID:
                        print('Parking space ' + sendBroadcastID + ' reached')
                        server.send(receivedBroadcast)
                        break;
                    else:
                        print('Received broadcast doesn\'t belong to parking space ' + sendBroadcastID)
                        unreachableParkingSpaceCounter += 1
                    
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
