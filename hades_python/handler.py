import lcm
from data import *
import os


class Globals():
    def __init__(self):
        self.team_blue = False

def handler_tartarus(channel, data_tartarus):

    msg_tartarus = tartarus.decode(data_tartarus)
    #print(f"Received message on channel \"{channel}\"")
    #print(f"   competition mode = {msg_tartarus.competition_mode}")

def handler_GC(channel, data_GC):
    #os.system('clear')
    #print("\n" + "-"*40 + "\n            C E R B E R U S \n" + "-"*40)
    #print(f"Received message on channel \"{channel}\"")
    msg_GC = game_controller.decode(data_GC)
    #print(f"   time_azul    = {msg_GC.team_blue}\n")
    g.team_blue = msg_GC.team_blue
    #print(f"score team blue:   {msg_GC.blue.score} \n")



def handler_vision(channel, data_vision):
    os.system('clear')
    print("\n" + "-"*40 + "\n            C E R B E R U S \n" + "-"*40)
    print(f"Received message on channel \"{channel}\"")
    msg_vision = vision.decode(data_vision)
    
    print(f"   timestamp   = {msg_vision.timestamp}\n")
    #print(f"   time_azul    = {msg_vision.team_blue}\n")
    #print(f"   a \n")
    print(f"   robots_yellow_size    = {msg_vision.robots_yellow_size}")
    print(f"   robots_blue_size    = {msg_vision.robots_blue_size}\n")
    print(f"   {msg_vision.timestamp} \n")
    



    msg_ia.timestamp = msg_vision.timestamp
    if (g.team_blue): #se for time azul
        msg_ia.robots_size = msg_vision.robots_blue_size
        robots_source = msg_vision.robots_blue
    else:
        msg_ia.robots_size = msg_vision.robots_yellow_size
        robots_source = msg_vision.robots_yellow

    #msg_ia.robots = [robot() for _ in range(msg_ia.robots_size)]

    for i in range(msg_ia.robots_size):
        robots_ia = msg_ia.robots[i]
        robots_sour = robots_source[i]

        robots_ia.id = robots_sour.robot_id
        robots_ia.spinner = False
        robots_ia.kick = False

        robots_ia.vel_tang = 0
        robots_ia.vel_normal = 0
        robots_ia.vel_ang = 0

        robots_ia.kick_speed_x = 0
        robots_ia.kick_speed_z = 0

        robots_ia.wheel_speed = False
        robots_ia.wheel_fr = 0
        robots_ia.wheel_fl = 0
        robots_ia.wheel_bl = 0
        robots_ia.wheel_br = 0
        

        msg_ia.robots.append(robots_ia)
    
    print(f"Field length: {msg_vision.field.field_length}")

    lc.publish("IA", msg_ia.encode())
    print(f"Bola:  pos_x{msg_vision.balls.position_x}  pos_y{msg_vision.balls.position_y} \n")
    print("Sent message on channel IA")
    for i in range(msg_ia.robots_size):
        
        print(f"  ID{i}: {msg_vision.robots_yellow[i].robot_id} ")
        print(f"  POS_X: {msg_vision.robots_yellow[i].position_x} ")
        print(f"  POS_Y: {msg_vision.robots_yellow[i].position_y} ")
        print(f"  ORIENTATION: {msg_vision.robots_yellow[i].orientation} \n")
    
        

g = Globals()
msg_ia = ia()
lc = lcm.LCM()

subscription = lc.subscribe("tartarus", handler_tartarus)
subscription = lc.subscribe("GC", handler_GC)
subscription = lc.subscribe("vision", handler_vision)

try:
    print("Waiting for messages on channels vision and GC")
    while True:
        lc.handle()
except KeyboardInterrupt:
    print("Shutting down...")
    pass