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
        _file_name = FileOpen[0].split('/')[-1].split('.')[0]
        self.file_name = _file_name
        self.file_directory = FileOpen[0]
        print("input: "+self.file_name+" and "+self.file_directory)

    def map_add_open(self):
        FileOpen = QFileDialog.getOpenFileName(self, 'Open file', './')
        self.label2.setText(FileOpen[0])
        self.pcd_name = FileOpen[0]
        print("pcd file set to : "+self.pcd_name)

    def map_sh(self):
        subprocess.call('./UI/UI_start_mapping.sh '+self.file_directory+' '+self.file_name, shell=True)

    def view_sh(self):
        print("hi")
        subprocess.call('./pcl/build/pcd_viewer_test '+self.pcd_name, shell=True)
        #subprocess.call('.sh '+self.file_directory+' '+self.file_name, shell=True)

    def initUI(self):
        grid = QGridLayout()
        self.setLayout(grid)
        self.label1 = QLabel(' ', self)
        self.label2 = QLabel(' ', self)

        btn1 = QPushButton('bag 파일 불러오기', self)
        btn1.clicked.connect(self.add_open)
        btn2 = QPushButton('지도 제작하기', self)
        btn2.clicked.connect(self.map_sh)
        btn3= QPushButton('지도 불러오기', self)
        btn3.clicked.connect(self.map_add_open)
        btn4= QPushButton('지도 확인하기', self)
        btn4.clicked.connect(self.view_sh)
        #grid.addWidget(QLabel('Bag : '), 0, 0)
        grid.addWidget(btn1, 0, 0)
        grid.addWidget(btn2, 0, 1)
        grid.addWidget(btn3, 0, 2)
        grid.addWidget(btn4, 0, 3)
        grid.addWidget(self.label1, 1, 0)
        grid.addWidget(self.label2, 2, 0)

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