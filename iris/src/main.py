import lcm
import os
from data import *
import time

class handlers:
    def __init__(self):
        self.msg_GC = game_controller()
        self.msg_vision = vision()
        self.msg_tartarus = tartarus()
        self.msg_ia = ia()

    def handler_GC(self, channel, data_GC):
        #print(f"Received message on channel \"{channel}\"")
        han.msg_GC = game_controller.decode(data_GC)

    def handler_vision(self, channel, data_vision):
        #print(f"Received message on channel \"{channel}\"")
        han.msg_vision = vision.decode(data_vision)

    def handler_tartarus(self, channel, data_tartarus):
        #print(f"Received message on channel \"{channel}\"")
        han.msg_tartarus = tartarus.decode(data_tartarus)
        han.msg_tartarus.ssl_vision = False
        time.sleep(0.03)
        lc.publish("tartarus", han.msg_tartarus.encode())

    def handler_ia(self, channel, data_ia):
        #print(f"Received message on channel \"{channel}\"")
        han.msg_ia = ia.decode(data_ia)
        #lc.publish("IA", han.msg_ia.encode())

han = handlers()
lc = lcm.LCM()

subscription = lc.subscribe("GC", han.handler_GC)
subscription = lc.subscribe("vision", han.handler_vision)
subscription = lc.subscribe("tartarus", han.handler_tartarus)
subscription = lc.subscribe("IA", han.handler_ia)

try:
    # Initialize the tartarus message
    lc.publish("tartarus", han.msg_tartarus.encode())
    while True:
        os.system('clear')
        print("\n" + "-"*40 + "\n            C E R B E R U S \n" + "-"*40)
        print(f"    timestamp sslvision: {han.msg_vision.timestamp} \n")
        print(f"    Nossos robos:   {han.msg_ia.robots_size} ")
        print(f"    Time_azul:   {han.msg_GC.team_blue} ")
        print(f"    Gols time amarelo {han.msg_GC.yellow.score} ")
        print(f"    Gols time azul   {han.msg_GC.blue.score} ")
        print(f"    Field length: {han.msg_vision.field.field_length}\n")
        print(f"    processo    = {han.msg_tartarus.processo}")
        print(f"    estrategia    = {han.msg_tartarus.estrategia}")
        print(f"    ssl_vision    = {han.msg_tartarus.ssl_vision}")
        lc.handle()
except KeyboardInterrupt:
    print("Shutting down...")
    pass