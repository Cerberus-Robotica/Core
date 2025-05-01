from PyQt5.QtCore import QObject, pyqtSignal

class LCMHandler(QObject):
    update_text_signal = pyqtSignal(str)

    def __init__(self, info_view, field_view):
        super().__init__()
        self.info_view = info_view
        self.field_view = field_view
        self.running = False
        self.thread = None

        # Conecta o sinal ao método de atualização
        self.update_text_signal.connect(self.field_view.update_text)

    def start_reading(self):
        if self.running:
            return
        self.running = True
        self.thread = threading.Thread(target=self.read_loop, daemon=True)
        self.thread.start()

    def stop_reading(self):
        self.running = False

    def read_loop(self):
        count = 0
        while self.running:
            msg = f"Mensagem recebida do LCM {count}"
            print(msg)

            # Emite o sinal com o texto
            self.update_text_signal.emit(msg)

            time.sleep(1)
