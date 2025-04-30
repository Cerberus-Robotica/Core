# DataModel.py
from PyQt5.QtCore import QObject, pyqtProperty, pyqtSignal

class DataModel(QObject):
    dataChanged = pyqtSignal()

    def __init__(self):
        super().__init__()
        self._data = {}

    @pyqtProperty('QString', notify=dataChanged)
    def info(self):
        return "\n".join([
            f"timestamp sslvision: {self._data.get('timestamp', 0)}",
            f"Nossos robos: {self._data.get('robots_size', 0)}",
            f"Time azul: {self._data.get('team_blue', False)}",
            f"Gols amarelo: {self._data.get('score_yellow', 0)}",
            f"Gols azul: {self._data.get('score_blue', 0)}",
            f"Field length: {self._data.get('field_length', 0)}",
            f"Processo: {self._data.get('processo', '')}",
            f"Estrategia: {self._data.get('estrategia', '')}",
            f"SSL Vision: {self._data.get('ssl_vision', False)}",
            f"Team Blue Status: {self._data.get('team_blue_status', False)}",
        ])

    def updateData(self, data):
        self._data = data
        self.dataChanged.emit()
