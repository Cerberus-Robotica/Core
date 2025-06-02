from flask import Flask, jsonify
from flask_cors import CORS
import threading
import lcm
import time
import sys
import os

# Corrige caminho para importar o módulo 'data'
current_dir = os.path.dirname(os.path.abspath(__file__))
src_dir = os.path.abspath(os.path.join(current_dir, '..', '..', '..', 'src'))
sys.path.append(src_dir)

# Importe suas classes de dados LCM
from data import game_controller, vision, tartarus, ia

app = Flask(__name__)
CORS(app) # CORS habilitado para todas as rotas

# Variável global para armazenar os dados mais recentes
# Inicialização com a estrutura esperada para evitar erros no primeiro acesso
latest_data = {
    "caronte": { "processo": "Inicializando...", "estrategia": "Aguardando dados..." },
    "ia": { "robots_size": 0 },
    "vision": { "timestamp": 0, "field_length": 0.0 },
    "gc": { "team_blue": False, "score_yellow": 0, "score_blue": 0 },
    "tartarus": { "ssl_vision": False, "team_blue_status": False },
}

class Handlers:
    def __init__(self):
        # Inicializa as mensagens com valores padrão ou classes vazias
        # para evitar acesso a atributos inexistentes antes da primeira mensagem LCM
        self.msg_GC = game_controller()
        self.msg_vision = vision()
        self.msg_tartarus = tartarus()
        self.msg_ia = ia()

        # Garante que os atributos necessários existam, mesmo que vazios/padrão
        if not hasattr(self.msg_GC, 'blue'):
            self.msg_GC.blue = type('obj', (object,), {'name': '', 'score': 0})()
        if not hasattr(self.msg_GC, 'yellow'):
            self.msg_GC.yellow = type('obj', (object,), {'name': '', 'score': 0})()
        if not hasattr(self.msg_vision, 'field'):
            self.msg_vision.field = type('obj', (object,), {'field_length': 0.0})()
        if not hasattr(self.msg_ia, 'processo'):
            self.msg_ia.processo = ""
        if not hasattr(self.msg_ia, 'estrategia'):
            self.msg_ia.estrategia = ""


    def handler_GC(self, channel, data):
        self.msg_GC = game_controller.decode(data)
        # Adicione tratamento para 'blue' e 'yellow' caso eles não estejam sempre presentes
        if not hasattr(self.msg_GC, 'blue'):
            self.msg_GC.blue = type('obj', (object,), {'name': '', 'score': 0})()
        if not hasattr(self.msg_GC, 'yellow'):
            self.msg_GC.yellow = type('obj', (object,), {'name': '', 'score': 0})()

    def handler_vision(self, channel, data):
        self.msg_vision = vision.decode(data)
        if not hasattr(self.msg_vision, 'field'):
            self.msg_vision.field = type('obj', (object,), {'field_length': 0.0})()

    def handler_tartarus(self, channel, data):
        self.msg_tartarus = tartarus.decode(data)
        self.msg_tartarus.ssl_vision = False # Mantém sua lógica existente
        # Lógica para determinar team_blue
        if self.msg_GC.blue.name == "Cerberus":
            self.msg_tartarus.team_blue = True
        elif self.msg_GC.yellow.name == "Cerberus":
            self.msg_tartarus.team_blue = False
        else:
            self.msg_tartarus.team_blue = True # Padrão, se Cerberus não for encontrado

    def handler_ia(self, channel, data):
        self.msg_ia = ia.decode(data)
        # Garante que os atributos existem após a decodificação
        if not hasattr(self.msg_ia, 'processo'):
            self.msg_ia.processo = ""
        if not hasattr(self.msg_ia, 'estrategia'):
            self.msg_ia.estrategia = ""


def lcm_listener():
    global latest_data
    lc = lcm.LCM()
    han = Handlers()

    lc.subscribe("GC", han.handler_GC)
    lc.subscribe("vision", han.handler_vision)
    lc.subscribe("tartarus", han.handler_tartarus)
    lc.subscribe("IA", han.handler_ia)

    print("[LCM] Listener iniciado.")

    while True:
        lc.handle_timeout(100)
        # AQUI: Estruture o dicionário para corresponder à interface DataType do React
        latest_data = {
            "caronte": {
                "processo": getattr(han.msg_ia, 'processo', "N/A"), # Use getattr para segurança
                "estrategia": getattr(han.msg_ia, 'estrategia', "N/A"),
            },
            "ia": {
                "robots_size": getattr(han.msg_ia, 'robots_size', 0),
            },
            "vision": {
                "timestamp": getattr(han.msg_vision, 'timestamp', 0),
                "field_length": getattr(getattr(han.msg_vision, 'field', object()), 'field_length', 0.0), # Acesso aninhado
            },
            "gc": {
                "team_blue": getattr(han.msg_GC, 'team_blue', False),
                "score_yellow": getattr(getattr(han.msg_GC, 'yellow', object()), 'score', 0),
                "score_blue": getattr(getattr(han.msg_GC, 'blue', object()), 'score', 0),
            },
            "tartarus": {
                "ssl_vision": getattr(han.msg_tartarus, 'ssl_vision', False),
                "team_blue_status": getattr(han.msg_tartarus, 'team_blue', False),
            },
            # Se 'goalkeeper_id' for necessário no frontend, adicione-o na interface DataType
            # e então adicione-o aqui. Por enquanto, não está na sua DataType atual.
            # "goalkeeper_id": getattr(han.msg_tartarus, 'goalkeeper_id', -1),
        }
        time.sleep(0.01)

@app.route("/data")
def get_data():
    return jsonify(latest_data)

if __name__ == "__main__":
    listener_thread = threading.Thread(target=lcm_listener, daemon=True)
    listener_thread.start()
    print("[FLASK] Servidor iniciado.")
    # Use host='0.0.0.0' para que o servidor seja acessível externamente (ex: de um container Docker ou outra máquina na rede),
    # mas localhost:5000 ainda funcionará. Para uso local, app.run(debug=True) é suficiente.
    app.run(debug=True, host='0.0.0.0', port=5000)