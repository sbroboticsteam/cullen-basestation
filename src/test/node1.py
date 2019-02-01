import zmq
import time
import sys

port = "5556"

if (len(sys.argv) > 1):
    port = sys.argv[1]
    int(port)

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:%s" % port) # tcp://5556

while True:
    message = socket.recv()
    print("Received request: ", message)
    time.sleep(1)
    socket.send_string("REPLY! from %s" % port)

