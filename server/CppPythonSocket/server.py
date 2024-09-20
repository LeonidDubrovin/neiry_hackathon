import numpy as np
import time
import socket
import struct

import sys


class Data:
    fatigueScore: float
    gravityScore: float
    concentrationScore: float
    accumulatedFatigue: float
    individualPeakFrequency: float

    def __init__(self, fatigueScore, gravityScore, concentrationScore, accumulatedFatigue, individualPeakFrequency):
        self.fatigueScore = fatigueScore
        self.gravityScore = gravityScore
        self.concentrationScore = concentrationScore
        self.accumulatedFatigue = accumulatedFatigue
        self.individualPeakFrequency = individualPeakFrequency


class Server:
    def __init__(self, ip, port):
        self.__size_message_length = 1024

        # Start and connect to client
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind((ip, port))

        self.socket.listen(True)
        print("[Server]: Waiting for connection...")
        self.conn, addr = self.socket.accept()
        print("[Server]: Connected")

    def __del__(self):
        self.socket.close()

    def send(self, message):
        message_size = str(len(message)).ljust(self.__size_message_length).encode()
        self.conn.sendall(message_size)  # Send length of msg (in known size, 16)
        self.conn.sendall(message.encode())  # Send message

    def receive(self, decode=True):
        message = self.__receive_value(self.conn, self.__size_message_length)
        if message is not None:  # If None received, no new message to read
            return message
        return None

    def __receive_value(self, conn, buf_lentgh, decode=True):
        STRUCT_FORMAT = 'fffff'
        STRUCT_SIZE = struct.calcsize(STRUCT_FORMAT)
        data = b""
        while len(data) < STRUCT_SIZE:
            packet = conn.recv(STRUCT_SIZE - len(data))
            if not packet:
                return None
            data += packet

        data = Data(*struct.unpack('ifI', data))
        print(f'Получено от клиента: int: {data.intValue}, float: {data.floatValue}, строка: {data.stringLen}')
        return data

    def clear_buffer(self):
        try:
            while self.conn.recv(1024):
                pass
        except:
            pass
        return
