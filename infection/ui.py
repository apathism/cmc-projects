from PyQt4.QtCore import QRectF, Qt, QPointF, QTimer
from PyQt4.QtGui import QMainWindow, QWidget, QAction, QKeySequence, QMenu, QPainter, QTextOption, \
    QInputDialog, QSlider, QVBoxLayout
from math import ceil
from config import WINDOW_SIZE_X, WINDOW_SIZE_Y, QT_FULL_CRICLE_DEGREE, GRAPHICS_VACCINATED_COLORS, GRAPHICS_ILL_COLORS, \
    BIG_FOR_CAPTION_RADIUS
from model import ModelState, Town, TownInfectionState


class RenderingArea(QWidget):
    def __init__(self, parent):
        super(RenderingArea, self).__init__(parent)
        self.parent = parent

    def get_town_radius(self, town):
        return ceil(float(town.population) ** 0.3)

    def paintTown(self, painter, town, x, y, radius):
        degree_per_person = QT_FULL_CRICLE_DEGREE / town.population
        rectangle = QRectF(x - radius, y - radius, 2 * radius, 2 * radius)
        current_angle = 0.0
        for population, color in reversed(list(zip(town.infection_state.vaccinated_population,
                                                   GRAPHICS_VACCINATED_COLORS))):
            span_angle = degree_per_person * population
            painter.setBrush(color)
            painter.drawPie(rectangle, current_angle, span_angle)
            current_angle += span_angle
        for population, color in zip(town.infection_state.ill_population, GRAPHICS_ILL_COLORS):
            span_angle = degree_per_person * population
            painter.setBrush(color)
            painter.drawPie(rectangle, current_angle, span_angle)
            current_angle += span_angle
        if radius > BIG_FOR_CAPTION_RADIUS:
            painter.setBrush(Qt.white)
            painter.drawEllipse(QPointF(x, y), radius / 2, radius / 2)
            painter.drawText(rectangle, town.name, QTextOption(Qt.AlignHCenter | Qt.AlignVCenter))
        else:
            painter.drawText(QRectF(x - radius, y + radius, 2 * radius, radius),
                             town.name, QTextOption(Qt.AlignHCenter | Qt.AlignTop))

    def paintEvent(self, QPaintEvent):
        painter = QPainter(self)
        for town in self.parent.model.towns:
            radius = self.get_town_radius(town)
            x = WINDOW_SIZE_X // 2 + town.x
            y = WINDOW_SIZE_Y // 2 + town.y
            self.paintTown(painter, town, x, y, radius)


class MainWindow(QMainWindow):
    menu_structure = \
        [
            {
                "type": QMenu,
                "name": "&Модель",
                "contents":
                    [
                        {
                            "type": QAction,
                            "name": "&Новая",
                            "internal_name": "new_model",
                            "shortcut": QKeySequence.New,
                        },
                        {
                            "type": QAction,
                            "name": "&Открыть",
                            "internal_name": "open_model",
                            "shortcut": QKeySequence.Open,
                        },
                        {
                            "type": QAction,
                            "name": "&Сохранить",
                            "internal_name": "save_model",
                            "shortcut": QKeySequence.Save,
                        },
                    ],
            },
            {
                "type": QMenu,
                "name": "&Объекты",
                "contents":
                    [
                        {
                            "type": QMenu,
                            "name": "Города",
                            "contents": [
                                {
                                    "type": QAction,
                                    "name": "Мегаполис",
                                    "internal_name": "new_city",
                                    "shortcut": QKeySequence("Ctrl+1"),
                                },
                                {
                                    "type": QAction,
                                    "name": "Крупный город",
                                    "internal_name": "new_town",
                                    "shortcut": QKeySequence("Ctrl+2"),
                                },
                                {
                                    "type": QAction,
                                    "name": "Маленький город",
                                    "internal_name": "new_village",
                                    "shortcut": QKeySequence("Ctrl+3"),
                                },
                            ],
                        },
                        {
                            "type": QAction,
                            "name": "Дорога",
                            "internal_name": "new_road",
                            "shortcut": QKeySequence("Ctrl+0"),
                        },
                    ],
            },
            {
                "type": QMenu,
                "name": "&Выполнение",
                "contents":
                    [
                        {
                            "type": QAction,
                            "name": "Запуск",
                            "internal_name": "start_modelling",
                            "shortcut": QKeySequence("Ctrl+S"),
                        },
                        {
                            "type": QAction,
                            "name": "Пауза",
                            "shortcut": QKeySequence("Ctrl+P"),
                            "internal_name": "pause_modelling",
                            "enabled": False,
                        },
                        {
                            "type": QAction,
                            "name": "Остановка",
                            "shortcut": QKeySequence("Ctrl+Q"),
                            "internal_name": "stop_modelling",
                            "enabled": False,
                        },
                    ],
            }
        ]
    timer = QTimer()

    def new_city_clicked(self):
        dialog = QInputDialog(self)
        city_name = QInputDialog.getText(dialog, "Имя города", "Введите имя города:")
        if not city_name[1] and city_name[0] == '':
            return
        ill_percent = QInputDialog.getDouble(dialog, "Заболеваемость", "Введите процент заболевших:", 0, 0, 100, 2)
        if not ill_percent[1] and ill_percent[0] == '':
            return
        town = Town(city_name[0], 1000000, 0, 0)
        state = TownInfectionState(town)
        ill_percent = ill_percent[0] * 0.01
        state.ill_population = (1000000 * ill_percent, 0, 0, 0)
        state.vaccinated_population = (1000000 * (1 - ill_percent), 0, 0, 0)
        town.infection_state = state
        self.model.towns.append(town)

    def start_modelling_clicked(self):
        self.timer.start()
        self.start_modelling.setDisabled(True)
        self.pause_modelling.setDisabled(False)

    def pause_modelling_clicked(self):
        if self.timer.isActive():
            self.timer.stop()
        else:
            self.timer.start()

    def build_menu(self, position, structure):
        if isinstance(structure, list):
            for item in structure:
                self.build_menu(position, item)
        elif structure["type"] is QAction:
            action = QAction(structure["name"], self)
            if "shortcut" in structure:
                action.setShortcut(structure["shortcut"])
            if "enabled" in structure and not structure["enabled"]:
                action.setDisabled(True)
            if "internal_name" in structure:
                setattr(self, structure["internal_name"], action)
                if hasattr(self, structure["internal_name"] + "_clicked"):
                    action.triggered.connect(getattr(self, structure["internal_name"] + "_clicked"))
            position.addAction(action)
        elif structure["type"] is QMenu:
            menu = QMenu(structure["name"], self)
            position.addMenu(menu)
            for item in structure["contents"]:
                self.build_menu(menu, item)

    def demonstration_model(self):
        moscow = Town("Москва", 12000000, -200, -100)
        self.model.towns.append(moscow)
        moscow.infection_state.vaccinated_population = (5000000, 1000000, 1000000, 1000000)
        moscow.infection_state.ill_population = (1000000, 2000000, 500000, 500000)
        self.model.towns.append(Town("Тверь", 500000, 100, 200))
        self.model.towns.append(Town("Мытищи", 100000, -300, 100))

    def timer_tick(self):
        self.model.make_step()
        self.rendering_area.repaint()

    def __init__(self, parent=None):
        self.model = ModelState()
        self.demonstration_model()
        super(MainWindow, self).__init__(parent)
        self.setWindowTitle("Распространение вирусного заболевания")
        self.rendering_area = RenderingArea(self)
        self.layout = QVBoxLayout()
        self.timer.setInterval(1000)
        self.timer_slider = QSlider(Qt.Horizontal)
        self.layout.addWidget(self.rendering_area)
        self.layout.addWidget(self.timer_slider)
        central_widget = QWidget()
        central_widget.setLayout(self.layout)
        self.setCentralWidget(central_widget)
        self.setFixedSize(WINDOW_SIZE_X, WINDOW_SIZE_Y)
        self.build_menu(self.menuBar(), self.menu_structure)
        self.timer.timeout.connect(self.timer_tick)