from CppPythonSocket.server import Server

if __name__ == "__main__":
    server = Server("127.0.0.1", 5003)
    while True:
        data = server.receive()
        if data:
            print("[Client]:", data.concentrationScore)
            print("[Client]:", data.individualPeakFrequency)
