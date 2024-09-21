from CppPythonSocket.server import Server

import struct


class Data:
    struct_format: str = 'Ifffff'

    def __init__(self, field_flags, fatigue_score, gravity_score, concentration_score, accumulated_fatigue, individual_peak_frequency):
        self.field_flags = field_flags
        self.fatigue_score = fatigue_score
        self.gravity_score = gravity_score
        self.concentration_score = concentration_score
        self.accumulated_fatigue = accumulated_fatigue
        self.individual_peak_frequency = individual_peak_frequency

    def get_fatigue_score(self):
        if self.field_flags & 0x01:
            return self.fatigue_score
        return None

    def get_gravity_score(self):
        if self.field_flags & 0x02:
            return self.gravity_score
        return None

    def get_concentration_score(self):
        if self.field_flags & 0x04:
            return self.concentration_score
        return None

    def get_accumulated_fatigue(self):
        if self.field_flags & 0x08:
            return self.accumulated_fatigue
        return None

    def get_individual_peak_frequency(self):
        if self.field_flags & 0x16:
            return self.individual_peak_frequency
        return None


def handler(conn):
    STRUCT_SIZE = struct.calcsize(Data.struct_format)
    try:
        while True:
            print("Receiving data")
            data = conn.recv(STRUCT_SIZE)
            if not data:
                print("Соединение потеряно. Ожидание переподключения клиента...")
                break

            data = Data(*struct.unpack(Data.struct_format, data))

            # print("Sending response")
            # conn.sendall(b"OK")

            if data.get_fatigue_score() is not None:
                print("fatigue_score:", data.get_fatigue_score())

            if data.get_gravity_score() is not None:
                print("gravity_score:", data.get_gravity_score())

            if data.get_concentration_score() is not None:
                print("concentration_score:", data.get_concentration_score())

            if data.get_accumulated_fatigue() is not None:
                print("accumulated_fatigue:", data.get_accumulated_fatigue())

            if data.get_individual_peak_frequency() is not None:
                print("individual_peak_frequency:", data.get_individual_peak_frequency())

    except ConnectionResetError:
        print("Соединение разорвано. Ожидание переподключения клиента...")
    finally:
        conn.close()


if __name__ == "__main__":
    server = Server("127.0.0.1", 5004)
    server.start(handler)

