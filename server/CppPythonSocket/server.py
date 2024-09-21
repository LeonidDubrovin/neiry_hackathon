import numpy as np
import time
import socket


class Server:
    def __init__(self, ip, port):
        self.__size_message_length = 1024  # Buffer size for the length

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((ip, port))
        self.server_socket.listen(True)
        print("Сервер запущен, ожидаем подключение...")

    def start(self, handler):
        while True:
            try:
                conn, addr = self.server_socket.accept()
                print(f"Подключено: {addr}")
                handler(conn)
            except socket.error:
                print("Ошибка подключения, попытка повторного подключения...")
                time.sleep(5)

    def __del__(self):
        self.server_socket.close()


    # def __receive_value(self, conn, buf_lentgh, decode=True):
    #     STRUCT_FORMAT = 'ifI'
    #     STRUCT_SIZE = struct.calcsize(STRUCT_FORMAT)
    #     data = b""
    #     while len(data) < STRUCT_SIZE:
    #         packet = conn.recv(STRUCT_SIZE - len(data))
    #         if not packet:
    #             return None
    #         data += packet
    #
    #     data = Data(*struct.unpack('ifI', data))
    #     print(f'Получено от клиента: int: {data.intValue}, float: {data.floatValue}, строка: {data.stringLen}')
    #     return data

    # def clear_buffer(self):
    #     try:
    #         while self.conn.recv(1024):
    #             pass
    #     except:
    #         pass
    #     return
