# LCMHandler.py
from PyQt5.QtCore import QObject, pyqtSignal, QThread
import lcm
from data import *
import time

class LCMWorker(QObject):
    dataUpdated = pyqtSignal(dict)
    robotsUpdated = pyqtSignal(list) 

    def __init__(self):
        super().__init__()
        self.running = True
        self.han = self.Handlers(self)

    class Handlers:
        def __init__(self, parent):
            self.parent = parent
            self.msg_GC = game_controller()
            self.msg_vision = vision()
            self.msg_tartarus = tartarus()
            self.msg_ia = ia()

        def handler_GC(self, channel, data_GC):
            self.msg_GC = game_controller.decode(data_GC)

        def handler_vision(self, channel, data_vision):
            self.msg_vision = vision.decode(data_vision)

        def handler_tartarus(self, channel, data_tartarus):
            self.msg_tartarus = tartarus.decode(data_tartarus)
            self.msg_tartarus.ssl_vision = False

            if self.msg_GC.blue.name == "Cerberus":
                self.msg_tartarus.team_blue = True 
            elif self.msg_GC.yellow.name == "Cerberus":
                self.msg_tartarus.team_blue = False
            else:
                self.msg_tartarus.team_blue = True

        def handler_ia(self, channel, data_ia):
            self.msg_ia = ia.decode(data_ia)

    def run(self):
        lc = lcm.LCM()
        lc.subscribe("GC", self.han.handler_GC)
        lc.subscribe("vision", self.han.handler_vision)
        lc.subscribe("tartarus", self.han.handler_tartarus)
        lc.subscribe("IA", self.han.handler_ia)

        while self.running:
            lc.handle_timeout(100)
            data = {
                "timestamp": self.han.msg_vision.timestamp,
                "robots_size": self.han.msg_ia.robots_size,
                "team_blue": self.han.msg_GC.team_blue,
                "score_yellow": self.han.msg_GC.yellow.score,
                "score_blue": self.han.msg_GC.blue.score,
                "field_length": self.han.msg_vision.field.field_length,
                "processo": self.han.msg_ia.processo,
                "estrategia": self.han.msg_ia.estrategia,
                "ssl_vision": self.han.msg_tartarus.ssl_vision,
                "team_blue_status": self.han.msg_tartarus.team_blue,
            }
            self.dataUpdated.emit(data)
            time.sleep(0.001)
