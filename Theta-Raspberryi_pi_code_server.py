#!/usr/bin/env python

import asyncore
import socket
import logging
import smbus
import time
import sys

usleep = lambda x: time.sleep(x / 1000000.0)
bus = smbus.SMBus(1)
address = 0xA


class Server(asyncore.dispatcher):
    def __init__(self, address):
        asyncore.dispatcher.__init__(self)
        self.logger = logging.getLogger('Server')
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind(address)
        self.address = self.socket.getsockname()
        self.logger.debug('binding to %s', self.address)
        self.listen(5)

    def handle_accept(self):
        # Called when a client connects to our socket
        client_info = self.accept()
        if client_info is not None:
            self.logger.debug('handle_accept() -> %s', client_info[1])
            ClientHandler(client_info[0], client_info[1])


def go_direction(dir):
    # values = []
    # for i in range (2):
    # bus.write_byte(address, i)
    # print("Senzor value : ",i , bus.read_byte(address))
    # a = bus.read_byte(address)
    # values.insert(i,a)
    # usleep(29500)
    # print(values)
    # front = values[0]
    if (dir == 0):
        print("\n stoop \n")
        #bus.write_byte(0x9, 0x0)
        #bus.write_byte(0x8, 0x0)
    if (dir == 1):
        print("\n go_frooont \n")
        #bus.write_byte(0x9, 0x1)
        #bus.write_byte(0x8, 0x1)
    if (dir == 2):
        print("\n go_back \n")
        #bus.write_byte(0x8, 0x2)
        #bus.write_byte(0x9, 0x2)

    if (dir == 3):
        print("\n turn_right \n")
        #bus.write_byte(0x8, 0x3)
        #bus.write_byte(0x9, 0x3)
    if (dir == 4):
        print("\n turn_left \n")
        #bus.write_byte(0x8, 0x4)
        #bus.write_byte(0x9, 0x4)


class ClientHandler(asyncore.dispatcher):

    def __init__(self, sock, address):
        asyncore.dispatcher.__init__(self, sock)
        self.logger = logging.getLogger('Client ' + str(address))
        self.data_to_write = []

    def writable(self):
        return bool(self.data_to_write)

    def handle_write(self):
        data = self.data_to_write.pop()
        sent = self.send(data[:8192])

        if sent < len(data):
            remaining = data[sent:]
            self.data.to_write.append(remaining)
        self.logger.debug(' handle_write() -> (%d) "%s"', sent, data[:sent].rstrip())

    def handle_read(self):
        direction = 0
        data = self.recv(8192)
        self.logger.debug(' handle_read() -> (%d) "%s"', len(data), data.rstrip())
        self.data_to_write.insert(0, data)
        if ("controll_front" in data.decode("utf-8")):
            go_direction(1)
        elif ("controll_back" in data.decode("utf-8")):
            go_direction(2)
        elif ("controll_stay" in data.decode("utf-8")):
            go_direction(0)
        elif ("controll_right" in data.decode("utf-8")):
            go_direction(3)
        elif ("controll_left" in data.decode("utf-8")):
            go_direction(4)

    def handle_close(self):
        self.logger.debug('handle_close()')
        self.close()


def main():
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(name)s:[%(levelname)s]: %(message)s')

    HOST = '192.168.0.154'
    PORT = 23
    s = Server((HOST, PORT))
    asyncore.loop()


if __name__ == '__main__':
    main()

