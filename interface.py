import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QGridLayout, QLabel, QLineEdit, QFileDialog
import subprocess

class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def add_open(self):
        FileOpen = QFileDialog.getOpenFileName(self, 'Open file', './')
        self.label1.setText(FileOpen[0])

    def sh(self):
        subprocess.call('./example.sh', shell=True)

    def initUI(self):
        grid = QGridLayout()
        self.setLayout(grid)
        self.label1 = QLabel(' ', self)
        btn1 = QPushButton('bag 파일 불러오기', self)
        btn1.clicked.connect(self.add_open)
        btn2 = QPushButton('지도 제작하기', self)
        btn2.clicked.connect(self.sh)
        #grid.addWidget(QLabel('Bag : '), 0, 0)
        grid.addWidget(btn1, 0, 0)
        grid.addWidget(btn2, 0, 1)
        grid.addWidget(self.label1, 1, 0)

        # btn2.setText('제작')
        #
        # btn3 = QPushButton('확인', self)

        # vbox = QVBoxLayout()
        # vbox.addWidget(btn1)
        # vbox.addWidget(btn2)
        # vbox.addWidget(btn3)
        #self.setLayout(vbox)

        self.setWindowTitle('Interface')
        self.resize(300, 100)
        #self.setGeometry(300, 300, 300, 200)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())