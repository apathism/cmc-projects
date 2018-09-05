import sys
from PyQt4.QtGui import QApplication
from ui import MainWindow

application = QApplication(sys.argv)
main_window = MainWindow()
main_window.show()
application.exec()