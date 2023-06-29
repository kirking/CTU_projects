import socket
import time
from math import sqrt
from threading import Thread

SERVER_SYNTAX_ERROR = "301 SYNTAX ERROR\a\b"
SERVER_LOGIC_ERROR = "302 LOGIC ERROR\a\b"
SERVER_KEY_OUT_OF_RANGE_ERROR = "303 KEY OUT OF RANGE\a\b"
SERVER_LOGIN_FAILED = "300 LOGIN FAILED\a\b"
SERVER_OK = "200 OK\a\b"
SERVER_KEY_REQUEST = "107 KEY REQUEST\a\b"
SERVER_LOGOUT = "106 LOGOUT\a\b"
SERVER_PICK_UP = "105 GET MESSAGE\a\b"
SERVER_TURN_RIGHT = "104 TURN RIGHT\a\b"
SERVER_TURN_LEFT = "103 TURN LEFT\a\b"
SERVER_MOVE = "102 MOVE\a\b"
CLIENT_RECHARGING = "RECHARGING\a\b"
CLIENT_FULL_POWER = "FULL POWER\a\b"

LEFT = -1
RIGHT = 1
UP = 2
DOWN = -2
STUCK = 0
ERROR = -3


class InputBuffer:
    data = ""

    def add_data(self, n_data):
        self.data += n_data

    def get_cur_data(self):
        if self.data.find("\a\b") != -1:
            tmp_str = self.data[:self.data.find("\a\b") + 2]
            self.data = self.data.replace(tmp_str, "")
            return tmp_str

    def check_input(self):
        if self.data.find("\a\b") != -1:
            return True
        return False

    def force_get_data(self):
        return self.data

    def clean(self):
        self.data = ""

    def get_content(self):
        return self.data

    def is_empty(self):
        return len(self.data) == 0


def check_distance(cur_offset):

    res = LEFT

    dist = sqrt((cur_offset["x"] - 1)**2 + cur_offset["y"]**2)

    if sqrt((cur_offset["x"] + 1)**2 + cur_offset["y"]**2) < dist:
        dist = sqrt((cur_offset["x"] + 1)**2 + cur_offset["y"]**2)
        res = RIGHT

    if sqrt(cur_offset["x"]** 2 + (cur_offset["y"] + 1)** 2) < dist:
        dist = sqrt(cur_offset["x"]** 2 + (cur_offset["y"] + 1)** 2)
        res = UP

    if sqrt(cur_offset["x"] ** 2 + (cur_offset["y"] - 1) ** 2) < dist:
        res = DOWN

    return res


def move_accept(connection, buf):
    if not receive_timeout(connection, buf, 1, -1):
        return None
    received_data = buf.get_cur_data()

    if detect_charge(received_data):
        if not receive_timeout(connection, buf, 5):
            return None
        received_data = buf.get_cur_data()
        if received_data != CLIENT_FULL_POWER:
            connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))

        if not receive_timeout(connection, buf, 1, 12):
            return None
        received_data = buf.get_cur_data()

    if len(str(received_data)) > 12 or \
            str(received_data)[-2:][0] != '\a' or str(received_data)[-2:][1] != '\b':
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return None

    return received_data


def change_offset(received_data, cur_offset, connection):

    received_data = received_data[3:-2]

    if not received_data[:received_data.find(" ")].replace("-", "1").isdigit():
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    cur_offset['x'] = int(received_data[:received_data.find(" ")])

    if not received_data[received_data.find(" ") + 1:].replace("-", "1").isdigit():
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    cur_offset['y'] = int(received_data[received_data.find(" ") + 1:])

    # print(cur_offset)

    return True


def det_direction(connection, buf, cur_offset):

    res = LEFT

    prev_offset = {"x": cur_offset["x"], "y": cur_offset["y"]}

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))

    received_data = move_accept(connection, buf)

    if received_data is None:
        return ERROR

    if not change_offset(received_data, cur_offset, connection):
        return ERROR


    # print("IN DET", cur_offset , prev_offset)

    if prev_offset["x"] == cur_offset["x"] - 1:
        res = RIGHT

    elif prev_offset["y"] == cur_offset["y"] + 1:
        res = DOWN

    elif prev_offset["y"] == cur_offset["y"] - 1:
        res = UP

    elif prev_offset["x"] == cur_offset["x"] and prev_offset["y"] == cur_offset["y"]:
        res = STUCK

    return res


def pass_obstacle(connection, buf, cur_offset):

    connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1

    if not change_offset(received_data, cur_offset , connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1

    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
    received_data = move_accept(connection, buf)

    if received_data is None:
        return -1
    if not change_offset(received_data, cur_offset, connection):
        return -1

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return 0

    return 1


def move(connection, buf, direction, cur_offset, cur_dir):

    if cur_dir == STUCK:
        if pass_obstacle(connection, buf, cur_offset) == -1:
            return False
        return True

    elif cur_dir == -direction:
        for i in range(2):
            connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

    elif direction == LEFT:

        if cur_dir == UP:
            connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

        elif cur_dir == DOWN:
            connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

    elif direction == RIGHT:
        if cur_dir == UP:
            connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

        elif cur_dir == DOWN:
            connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

    elif direction == DOWN:
        if cur_dir == LEFT:
            connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

        elif cur_dir == RIGHT:
            connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

    elif direction == UP:
        if cur_dir == LEFT:
            connection.send(bytes(SERVER_TURN_RIGHT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

        elif cur_dir == RIGHT:
            connection.send(bytes(SERVER_TURN_LEFT, encoding="UTF-8"))
            received_data = move_accept(connection, buf)

            if received_data is None:
                return False

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))

    received_data = move_accept(connection, buf)

    if received_data is None:
        return False

    prev_offset = {"x": cur_offset["x"], "y": cur_offset["y"]}
    if not change_offset(received_data, cur_offset, connection):
        return False
    if prev_offset == cur_offset:
        if pass_obstacle(connection, buf, cur_offset) == -1:
            return False

    return True


def path_finding(connection, buf):
    cur_offset = {"x": -1,
                  "y": -1}

    connection.send(bytes(SERVER_MOVE, encoding="UTF-8"))

    received_data = move_accept(connection, buf)

    if received_data is None:
        return False

    if not change_offset(received_data, cur_offset, connection):
        return False

    if cur_offset["x"] == 0 and cur_offset["y"] == 0:
        return pickup(connection, buf)

    cur_dir = det_direction(connection, buf, cur_offset)

    # print("DET DIR:", cur_dir)

    prev_offset = {"x": cur_offset["x"], "y": cur_offset["y"]}

    if cur_dir == ERROR:
        return False

    elif cur_dir == STUCK:
        if not move(connection, buf, LEFT, cur_offset, cur_dir):
            return False

        if prev_offset["x"] == cur_offset["x"] - 2:
            cur_dir = RIGHT

        elif prev_offset["y"] == cur_offset["y"] - 2:
            cur_dir = UP

        elif prev_offset["y"] == cur_offset["y"] + 2:
            cur_dir = DOWN

        else:
            cur_dir = LEFT

    while not (cur_offset["x"] == 0 and cur_offset["y"] == 0):
        optimal_dir = check_distance(cur_offset)
        if not move(connection, buf, optimal_dir, cur_offset, cur_dir):
            return False

        cur_dir = optimal_dir

    return pickup(connection, buf)


def pickup(connection, buf):

    connection.send(bytes(SERVER_PICK_UP, encoding="UTF-8"))

    if not receive_timeout(connection, buf, 1, 100):
        return False

    received_data = buf.get_cur_data()
    if received_data == CLIENT_FULL_POWER:
        connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))
        connection.close()
    # print(str(received_data))

    if detect_charge(received_data):
        if not receive_timeout(connection, buf, 5):
            return False
        received_data = buf.get_cur_data()
        if received_data != CLIENT_FULL_POWER:
            connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))

        if not receive_timeout(connection, buf, 1, 100):
            return False
        received_data = buf.get_cur_data()

    if len(str(received_data)) > 100 or \
            str(received_data)[-2:][0] != '\a' or str(received_data)[-2:][1] != '\b':
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    # print(received_data)

    connection.send(bytes(SERVER_LOGOUT, encoding="UTF-8"))

    # print("LOGOUT")

    connection.close()

    return True


def detect_charge(data):
    return data == CLIENT_RECHARGING


def receive_timeout(connection, buf, timer, length=-1, synt=True):
    try:
        deadline = time.time() + timer
        while not buf.check_input():
            if len(buf.force_get_data()) > length != -1:
                connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
                connection.close()
                return False
            if time.time() >= deadline:
                return False
            connection.settimeout(deadline - time.time())
            buf.add_data(connection.recv(20).decode("UTF-8"))
            deadline = time.time() + 1

    except socket.timeout:
        # print("Timeout")
        if not buf.is_empty() and synt:
            connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    return True


def get_client_hash(connection, buf):
    if not receive_timeout(connection, buf, 1, 20):
        return -1

    received_data = buf.get_cur_data()
    if received_data == CLIENT_FULL_POWER:
        connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))
        connection.close()
    # print(str(received_data))

    if detect_charge(received_data):
        if not receive_timeout(connection, buf, 5):
            return -1
        received_data = buf.get_cur_data()
        if received_data != CLIENT_FULL_POWER:
            connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))

        if not receive_timeout(connection, buf, 1, 20):
            return -1
        received_data = buf.get_cur_data()

    if len(str(received_data)) > 20 or \
            str(received_data)[-2:][0] != '\a' or str(received_data)[-2:][1] != '\b':
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return -1

    client_hash = 0

    for sym in received_data[:-2]:
        client_hash += ord(sym)

    client_hash = (client_hash * 1000) % 65536

    # print("get_hash OK")

    return client_hash


def check_key(key_id, connection):
    if key_id not in range(5):
        connection.send(bytes(SERVER_KEY_OUT_OF_RANGE_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    return True


def server_key_request(connection, client_hash, buf):
    connection.send(bytes(SERVER_KEY_REQUEST, encoding="UTF-8"))
    if not receive_timeout(connection, buf, 1, 5):
        return -1

    received_data = buf.get_cur_data()
    if received_data == CLIENT_FULL_POWER:
        connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))
        connection.close()

    if detect_charge(received_data):
        if not receive_timeout(connection, buf, 5):
            return -1
        received_data = buf.get_cur_data()
        if received_data != CLIENT_FULL_POWER:
            connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))

        if not receive_timeout(connection, buf, 1, 5):
            return -1
        received_data = buf.get_cur_data()

    if received_data[-2:][0] != '\a' or \
            received_data[-2:][1] != '\b' or \
            len(received_data) > 5 or \
            not received_data[:-2].isdecimal():
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return -1

    key_id = int(received_data[:-2])

    if check_key(key_id, connection):
        # print("id: ", key_id)
        confirm_hash = (client_hash + keys[key_id][0]) % 65536
        connection.send(bytes(str(confirm_hash) + "\a\b", encoding="UTF-8"))
        return key_id
    return -1


def server_confirm(connection, client_hash, key_id, buf):
    if not receive_timeout(connection, buf, 1, 7, False):
        return False

    received_data = buf.get_cur_data()
    if received_data == CLIENT_FULL_POWER:
        connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))
        connection.close()

    if detect_charge(received_data):
        if not receive_timeout(connection, buf, 5, False):
            return False
        received_data = buf.get_cur_data()
        if received_data != CLIENT_FULL_POWER:
            connection.send(bytes(SERVER_LOGIC_ERROR, encoding="UTF-8"))
        if not receive_timeout(connection, buf, 1, 7):
            return -1

    if len(received_data) > 7:
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    if not received_data[:-2].isdigit() or \
            received_data[-2:][0] != '\a' or \
            received_data[-2:][1] != '\b':
        connection.send(bytes(SERVER_SYNTAX_ERROR, encoding="UTF-8"))
        connection.close()
        return False

    to_check = int(received_data[:-2])

    # print("Got: ", received_data)

    if ((client_hash + keys[key_id][1]) % 65536) == to_check:
        connection.send(bytes(SERVER_OK, encoding="UTF-8"))
        return True
    else:
        connection.send(bytes(SERVER_LOGIN_FAILED, encoding="UTF-8"))
        return False


def authentication(connection, buf):
    # print("Auth")

    client_hash = get_client_hash(connection, buf)

    # print("hash: ", client_hash)

    if client_hash == -1:
        return False

    key_id = server_key_request(connection, client_hash, buf)

    if key_id == -1:
        return False

    # print("Key ID: ", key_id)

    if server_confirm(connection, client_hash, key_id, buf):
        return True

    return False


def server_thread(connection):
    buffer = InputBuffer()
    if authentication(connection, buffer):
        # print("Auth success")
        path_finding(connection, buffer)
        buffer.clean()
    else:
        # print("Auth failed")
        buffer.clean()
    connection.close()


SERVER_ADDRESS = ("", 3999)

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.bind(SERVER_ADDRESS)
server_socket.listen(10)

#        ID server client
keys = {0: [23019, 32037],
        1: [32037, 29295],
        2: [18789, 13603],
        3: [16443, 29533],
        4: [18189, 21952]}


while True:

    new_connection, address = server_socket.accept()

    th = Thread(target=server_thread, args=(new_connection,))

    th.start()

    # print("new connection from {address}".format(address=address))


