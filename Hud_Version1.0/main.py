from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QAction, QGraphicsView, QGraphicsScene, QSizePolicy, QGroupBox,
    QLabel, QTextEdit, QPushButton, QLineEdit, QFrame, QGraphicsTextItem  # <- aqui está!
)

from PyQt5.QtGui import QIcon, QKeyEvent
from PyQt5.QtCore import Qt
import sys
import subprocess
import os
import shutil
import glob

# Adiciona o caminho da pasta Core ao sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "Core")))

from lcm_handler import LCMHandler

class MainWindow(QMainWindow):
    def __init__(self, widget):
        super().__init__()
        self.setWindowTitle("Cerberus IRIS")
        self.setWindowIcon(QIcon(r"./assets/Logo_IRIS.png"))
        self.setMinimumSize(1920, 1080)

        self.setCentralWidget(widget)

        self.menuBar = self.menuBar()
        self.menuBar.setObjectName("mainMenu")

        self.menu = self.menuBar.addMenu("Menu")
        self.menu.setObjectName("optMenu")

        exit = QAction("Exit", self)
        exit.setShortcut("Ctrl+Q")
        exit.triggered.connect(lambda: app.quit())
        self.menu.addAction(exit)

        self.divMenu = self.menuBar.addMenu("Divisão")
        self.divMenu.setObjectName("optDiv")

        self.widMenu = self.menuBar.addMenu("Widgets")
        self.widMenu.setObjectName("optWid")

        self.configMenu = self.menuBar.addMenu("Configurações")
        self.configMenu.setObjectName("optConfig")

class FieldView(QGraphicsView):
    def __init__(self):
        super().__init__()
        self.setObjectName("fieldView")
        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.setFixedSize(940, 550)

        # Adiciona item de texto à cena
        self.text_item = QGraphicsTextItem()
        self.scene.addItem(self.text_item)

    def update_text(self, content):
        self.text_item.setPlainText(content)
        self.text_item.setPos(0, 0)  # Garante que o texto fique no canto superior esquerdo
        self.scene.setSceneRect(self.text_item.boundingRect())  # Ajusta a cena ao texto
        self.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)


    def resizeEvent(self, event):
        self.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)
        super().resizeEvent(event)

class InfoView(QGraphicsView):
    def __init__(self):
        super().__init__()
        self.setObjectName("infoView")
        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.setFixedSize(500, 805)

        self.info_label = QLabel("Informações da IA")
        self.info_label.setObjectName("infoLabel")
        self.scene.addWidget(self.info_label)

    def resizeEvent(self, event):
        self.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)
        super().resizeEvent(event)

class TerminalView(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("terminalView")

        self.username = "User"
        self.hostname = "Cerberus"
        self.cwd = os.path.expanduser("~")
        self.setFixedSize(940, 250)

        self.command_history = []
        self.history_index = -1
        self.autocomplete_matches = []
        self.autocomplete_index = 0

        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)

        container = QGroupBox()
        container.setObjectName("terminalBox")
        container_layout = QVBoxLayout()
        container_layout.setContentsMargins(10, 10, 10, 10)

        self.status_label = QLabel("Status: Conectando...")
        self.status_label.setObjectName("statusLabel")
        container_layout.addWidget(self.status_label)

        self.log_area = QTextEdit()
        self.log_area.setReadOnly(True)
        self.log_area.setObjectName("logArea")
        container_layout.addWidget(self.log_area)

        self.input_line = QLineEdit()
        self.input_line.setObjectName("inputLine")
        self.update_prompt()
        self.input_line.returnPressed.connect(self.execute_command)
        self.input_line.installEventFilter(self)
        container_layout.addWidget(self.input_line)

        btn_layout = QHBoxLayout()

        btn_clear = QPushButton("🧹 Limpar Log")
        btn_clear.clicked.connect(self.log_area.clear)
        btn_layout.addWidget(btn_clear)

        self.toggle_button = QPushButton("🟢 Iniciar Leitura")
        self.toggle_button.setCheckable(True)
        self.toggle_button.clicked.connect(self.toggle_reading)
        btn_layout.addWidget(self.toggle_button)

        btn_layout.addStretch()
        container_layout.addLayout(btn_layout)

        container.setLayout(container_layout)
        main_layout.addWidget(container)
        self.setLayout(main_layout)

    def update_prompt(self):
        relative_path = os.path.relpath(self.cwd, os.path.expanduser("~"))
        if relative_path == ".":
            relative_path = "~"
        else:
            relative_path = f"~/{relative_path}"
        self.prompt = f"{self.username}@{self.hostname}:{relative_path}$ "
        self.input_line.setPlaceholderText(self.prompt)

    def toggle_reading(self):
        if self.toggle_button.isChecked():
            self.toggle_button.setText("🔴 Parar Leitura")
            self.log_area.append(f"<span style='color: cyan'>{self.prompt} leitura iniciada...</span>")
        else:
            self.toggle_button.setText("🟢 Iniciar Leitura")
            self.log_area.append(f"<span style='color: orange'>{self.prompt} leitura parada.</span>")

    def execute_command(self):
        full_input = self.input_line.text()
        if not full_input.strip():
            return

        self.log_area.append(f"<span style='color: lightgreen'>{self.prompt}</span>{full_input}")
        self.command_history.append(full_input)
        self.history_index = len(self.command_history)
        self.input_line.clear()

        tokens = full_input.strip().split()
        command = tokens[0]

        if command == "cd":
            if len(tokens) > 1:
                new_dir = tokens[1]
                try:
                    path = os.path.abspath(os.path.join(self.cwd, new_dir))
                    if os.path.isdir(path):
                        self.cwd = path
                    else:
                        self.log_area.append(f"<span style='color:red'>cd: nenhum tal arquivo ou diretório: {new_dir}</span>")
                except Exception as e:
                    self.log_area.append(f"<span style='color:red'>cd error: {e}</span>")
            else:
                self.cwd = os.path.expanduser("~")
            self.update_prompt()
            return

        try:
            result = subprocess.run(
                full_input, shell=True, cwd=self.cwd,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
            )
            if result.stdout:
                self.log_area.append(result.stdout.strip())
            if result.stderr:
                self.log_area.append(f"<span style='color:red'>{result.stderr.strip()}</span>")
        except Exception as e:
            self.log_area.append(f"<span style='color:red'>Erro ao executar comando: {e}</span>")

    def eventFilter(self, obj, event):
        if obj == self.input_line and event.type() == event.KeyPress:
            if event.key() == Qt.Key_Tab:
                self.handle_tab_autocomplete()
                return True
            elif event.key() == Qt.Key_Up:
                self.navigate_history(-1)
                return True
            elif event.key() == Qt.Key_Down:
                self.navigate_history(1)
                return True
        return super().eventFilter(obj, event)

    def handle_tab_autocomplete(self):
        text = self.input_line.text()
        if not text.strip():
            return

        parts = text.strip().split()
        if len(parts) < 2:
            return

        base_path = self.cwd
        if parts[0] == "cd":
            prefix = parts[1]
            try:
                matches = [f for f in os.listdir(base_path) if f.startswith(prefix)]
                matches = [f for f in matches if os.path.isdir(os.path.join(base_path, f))]
                if matches:
                    if matches != self.autocomplete_matches:
                        self.autocomplete_matches = matches
                        self.autocomplete_index = 0
                    else:
                        self.autocomplete_index = (self.autocomplete_index + 1) % len(matches)
                    parts[1] = self.autocomplete_matches[self.autocomplete_index]
                    self.input_line.setText(" ".join(parts))
            except Exception:
                pass

    def navigate_history(self, direction):
        if not self.command_history:
            return

        self.history_index += direction
        self.history_index = max(0, min(self.history_index, len(self.command_history)))

        if self.history_index < len(self.command_history):
            self.input_line.setText(self.command_history[self.history_index])
        else:
            self.input_line.clear()

class FieldContainer(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("fieldContainer")

        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)

        self.field = FieldView()
        self.terminal = TerminalView()

        layout.addWidget(self.field)
        layout.addWidget(self.terminal)

        self.setLayout(layout)

class InfoContainer(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("infoContainer")

        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)

        self.info = InfoView()

        layout.addWidget(self.info)

        self.setLayout(layout)

if __name__ == "__main__":
    app = QApplication(sys.argv)

    with open("style.qss", "r") as file:
        app.setStyleSheet(file.read())

    mainWidget = QWidget()
    mainLayout = QHBoxLayout()

    fieldContainer = FieldContainer()
    infoContainer = InfoContainer()

    leftLayout = QVBoxLayout()
    leftLayout.addWidget(fieldContainer)
    leftLayout.addStretch()

    leftContainer = QWidget()
    leftContainer.setLayout(leftLayout)

    screen_width = app.primaryScreen().size().width()
    leftContainer.setMaximumWidth(int(0.5 * screen_width))

    rightLayout = QVBoxLayout()
    rightLayout.addWidget(infoContainer)
    rightLayout.addStretch()

    rightContainer = QWidget()
    rightContainer.setLayout(rightLayout)

    mainLayout.addWidget(leftContainer)
    mainLayout.addWidget(rightContainer)

    mainWidget.setLayout(mainLayout)

    window = MainWindow(mainWidget)
    window.show()

    # Passa o field diretamente para o LCMHandler
    lcm_handler = LCMHandler(infoContainer.info, fieldContainer.field)

    fieldContainer.terminal.toggle_button.clicked.connect(
        lambda: lcm_handler.start_reading() if fieldContainer.terminal.toggle_button.isChecked()
        else lcm_handler.stop_reading()
    )

    sys.exit(app.exec_())
