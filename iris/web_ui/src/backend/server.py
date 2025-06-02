# backend/server.py
from flask import Flask, jsonify
import lcm
import threading
import time
from data import *

app = Flask(__name__)
latest_data = {}

class LCMHandler:
    def __init__(self):
        self.lc = lcm.LCM()
        self.msg_GC = game_controller()
        self.msg_vision = vision()
        self.msg_tartarus = tartarus()
        self.msg_ia = ia()

        self.lc.subscribe("GC", self.handle_GC)
        self.lc.subscribe("vision", self.handle_vision)
        self.lc.subscribe("tartarus", self.handle_tartarus)
        self.lc.subscribe("IA", self.handle_ia)

    def handle_GC(self, channel, data):
        self.msg_GC = game_controller.decode(data)

    def handle_vision(self, channel, data):
        self.msg_vision = vision.decode(data)

    def handle_tartarus(self, channel, data):
        self.msg_tartarus = tartarus.decode(data)
        if self.msg_GC.blue.name == "Cerberus":
            self.msg_tartarus.team_blue = True
        elif self.msg_GC.yellow.name == "Cerberus":
            self.msg_tartarus.team_blue = False
        else:
            self.msg_tartarus.team_blue = True

    def handle_ia(self, channel, data):
        self.msg_ia = ia.decode(data)

    def listen(self):
        while True:
            self.lc.handle_timeout(100)
            global latest_data
            latest_data = {
                "timestamp": self.msg_vision.timestamp,
                "robots_size": self.msg_ia.robots_size,
                "team_blue": self.msg_GC.team_blue,
                "score_yellow": self.msg_GC.yellow.score,
                "score_blue": self.msg_GC.blue.score,
                "field_length": self.msg_vision.field.field_length,
                "processo": self.msg_ia.processo,
                "estrategia": self.msg_ia.estrategia,
                "ssl_vision": self.msg_tartarus.ssl_vision,
                "team_blue_status": self.msg_tartarus.team_blue,
            }
            time.sleep(0.1)

handler = LCMHandler()
threading.Thread(target=handler.listen, daemon=True).start()

@app.route('/api/data')
def get_data():
    return jsonify(latest_data)

if __name__ == '__main__':
    app.run(debug=True, port=5000)
