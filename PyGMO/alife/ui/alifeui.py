# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'alifeui.ui'
#
# Created: Mon Aug 16 17:40:26 2010
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ALife(object):
    def setupUi(self, ALife):
        ALife.setObjectName("ALife")
        ALife.resize(760, 800)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(ALife.sizePolicy().hasHeightForWidth())
        ALife.setSizePolicy(sizePolicy)
        self.centralwidget = QtGui.QWidget(ALife)
        self.centralwidget.setObjectName("centralwidget")
        self.main_view = QtGui.QWidget(self.centralwidget)
        self.main_view.setGeometry(QtCore.QRect(20, 10, 721, 311))
        self.main_view.setObjectName("main_view")
        self.start_pause = QtGui.QPushButton(self.centralwidget)
        self.start_pause.setGeometry(QtCore.QRect(180, 320, 165, 32))
        self.start_pause.setAutoDefault(False)
        self.start_pause.setDefault(False)
        self.start_pause.setFlat(False)
        self.start_pause.setObjectName("start_pause")
        self.line_2 = QtGui.QFrame(self.centralwidget)
        self.line_2.setGeometry(QtCore.QRect(20, 350, 711, 20))
        self.line_2.setFrameShape(QtGui.QFrame.HLine)
        self.line_2.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.layoutWidget = QtGui.QWidget(self.centralwidget)
        self.layoutWidget.setGeometry(QtCore.QRect(20, 380, 735, 361))
        self.layoutWidget.setObjectName("layoutWidget")
        self.gridLayout = QtGui.QGridLayout(self.layoutWidget)
        self.gridLayout.setObjectName("gridLayout")
        self.line = QtGui.QFrame(self.layoutWidget)
        self.line.setFrameShape(QtGui.QFrame.VLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName("line")
        self.gridLayout.addWidget(self.line, 0, 2, 8, 1)
        self.line1 = QtGui.QFrame(self.layoutWidget)
        self.line1.setFrameShape(QtGui.QFrame.VLine)
        self.line1.setFrameShadow(QtGui.QFrame.Sunken)
        self.line1.setObjectName("line1")
        self.gridLayout.addWidget(self.line1, 0, 2, 9, 1)
        self.weights_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.weights_label.sizePolicy().hasHeightForWidth())
        self.weights_label.setSizePolicy(sizePolicy)
        self.weights_label.setObjectName("weights_label")
        self.gridLayout.addWidget(self.weights_label, 1, 3, 1, 1)
        self.weights = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.weights.sizePolicy().hasHeightForWidth())
        self.weights.setSizePolicy(sizePolicy)
        self.weights.setObjectName("weights")
        self.gridLayout.addWidget(self.weights, 1, 4, 1, 1)
        self.legs_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.legs_label.sizePolicy().hasHeightForWidth())
        self.legs_label.setSizePolicy(sizePolicy)
        self.legs_label.setObjectName("legs_label")
        self.gridLayout.addWidget(self.legs_label, 2, 0, 1, 1)
        self.legs = QtGui.QComboBox(self.layoutWidget)
        self.legs.setObjectName("legs")
        self.legs.addItem("")
        self.legs.addItem("")
        self.legs.addItem("")
        self.gridLayout.addWidget(self.legs, 2, 1, 1, 1)
        self.evolve = QtGui.QPushButton(self.layoutWidget)
        self.evolve.setObjectName("evolve")
        self.gridLayout.addWidget(self.evolve, 2, 4, 1, 1)
        self.body_density_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.body_density_label.sizePolicy().hasHeightForWidth())
        self.body_density_label.setSizePolicy(sizePolicy)
        self.body_density_label.setObjectName("body_density_label")
        self.gridLayout.addWidget(self.body_density_label, 3, 0, 1, 1)
        self.body_density = QtGui.QComboBox(self.layoutWidget)
        self.body_density.setObjectName("body_density")
        self.body_density.addItem("")
        self.body_density.addItem("")
        self.body_density.addItem("")
        self.body_density.addItem("")
        self.gridLayout.addWidget(self.body_density, 3, 1, 1, 1)
        self.leg_density_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.leg_density_label.sizePolicy().hasHeightForWidth())
        self.leg_density_label.setSizePolicy(sizePolicy)
        self.leg_density_label.setObjectName("leg_density_label")
        self.gridLayout.addWidget(self.leg_density_label, 4, 0, 1, 1)
        self.leg_density = QtGui.QComboBox(self.layoutWidget)
        self.leg_density.setMinimumSize(QtCore.QSize(0, 26))
        self.leg_density.setObjectName("leg_density")
        self.leg_density.addItem("")
        self.leg_density.addItem("")
        self.leg_density.addItem("")
        self.leg_density.addItem("")
        self.gridLayout.addWidget(self.leg_density, 4, 1, 1, 1)
        self.fitness_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.fitness_label.sizePolicy().hasHeightForWidth())
        self.fitness_label.setSizePolicy(sizePolicy)
        self.fitness_label.setObjectName("fitness_label")
        self.gridLayout.addWidget(self.fitness_label, 4, 3, 1, 1)
        self.fitness = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.fitness.sizePolicy().hasHeightForWidth())
        self.fitness.setSizePolicy(sizePolicy)
        self.fitness.setObjectName("fitness")
        self.gridLayout.addWidget(self.fitness, 4, 4, 1, 1)
        self.algorithm_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.algorithm_label.sizePolicy().hasHeightForWidth())
        self.algorithm_label.setSizePolicy(sizePolicy)
        self.algorithm_label.setObjectName("algorithm_label")
        self.gridLayout.addWidget(self.algorithm_label, 5, 3, 1, 1)
        self.algorithm = QtGui.QComboBox(self.layoutWidget)
        self.algorithm.setObjectName("algorithm")
        self.algorithm.addItem("")
        self.algorithm.addItem("")
        self.algorithm.addItem("")
        self.gridLayout.addWidget(self.algorithm, 5, 4, 1, 1)
        self.generations_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.generations_label.sizePolicy().hasHeightForWidth())
        self.generations_label.setSizePolicy(sizePolicy)
        self.generations_label.setObjectName("generations_label")
        self.gridLayout.addWidget(self.generations_label, 6, 3, 1, 1)
        self.generations = QtGui.QComboBox(self.layoutWidget)
        self.generations.setObjectName("generations")
        self.generations.addItem("")
        self.generations.addItem("")
        self.generations.addItem("")
        self.generations.addItem("")
        self.generations.addItem("")
        self.generations.addItem("")
        self.generations.addItem("")
        self.gridLayout.addWidget(self.generations, 6, 4, 1, 1)
        self.islands_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.islands_label.sizePolicy().hasHeightForWidth())
        self.islands_label.setSizePolicy(sizePolicy)
        self.islands_label.setObjectName("islands_label")
        self.gridLayout.addWidget(self.islands_label, 7, 3, 1, 1)
        self.islands = QtGui.QComboBox(self.layoutWidget)
        self.islands.setObjectName("islands")
        self.islands.addItem("")
        self.islands.addItem("")
        self.islands.addItem("")
        self.islands.addItem("")
        self.islands.addItem("")
        self.gridLayout.addWidget(self.islands, 7, 4, 1, 1)
        self.individuals_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.individuals_label.sizePolicy().hasHeightForWidth())
        self.individuals_label.setSizePolicy(sizePolicy)
        self.individuals_label.setObjectName("individuals_label")
        self.gridLayout.addWidget(self.individuals_label, 8, 3, 1, 1)
        self.individuals = QtGui.QComboBox(self.layoutWidget)
        self.individuals.setObjectName("individuals")
        self.individuals.addItem("")
        self.individuals.addItem("")
        self.individuals.addItem("")
        self.gridLayout.addWidget(self.individuals, 8, 4, 1, 1)
        self.robot_environment_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.robot_environment_label.sizePolicy().hasHeightForWidth())
        self.robot_environment_label.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.robot_environment_label.setFont(font)
        self.robot_environment_label.setObjectName("robot_environment_label")
        self.gridLayout.addWidget(self.robot_environment_label, 0, 0, 1, 2)
        self.evolution_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.evolution_label.sizePolicy().hasHeightForWidth())
        self.evolution_label.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.evolution_label.setFont(font)
        self.evolution_label.setObjectName("evolution_label")
        self.gridLayout.addWidget(self.evolution_label, 0, 3, 1, 2)
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.load = QtGui.QPushButton(self.layoutWidget)
        self.load.setEnabled(True)
        self.load.setObjectName("load")
        self.gridLayout_2.addWidget(self.load, 0, 0, 1, 1)
        self.save = QtGui.QPushButton(self.layoutWidget)
        self.save.setEnabled(True)
        self.save.setObjectName("save")
        self.gridLayout_2.addWidget(self.save, 0, 1, 1, 1)
        self.gridLayout.addLayout(self.gridLayout_2, 3, 4, 1, 1)
        self.asteroid_mass_label = QtGui.QLabel(self.layoutWidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.asteroid_mass_label.sizePolicy().hasHeightForWidth())
        self.asteroid_mass_label.setSizePolicy(sizePolicy)
        self.asteroid_mass_label.setObjectName("asteroid_mass_label")
        self.gridLayout.addWidget(self.asteroid_mass_label, 1, 0, 1, 1)
        self.asteroid_mass = QtGui.QComboBox(self.layoutWidget)
        self.asteroid_mass.setObjectName("asteroid_mass")
        self.asteroid_mass.addItem("")
        self.asteroid_mass.addItem("")
        self.asteroid_mass.addItem("")
        self.asteroid_mass.addItem("")
        self.asteroid_mass.addItem("")
        self.gridLayout.addWidget(self.asteroid_mass, 1, 1, 1, 1)
        self.reset = QtGui.QPushButton(self.centralwidget)
        self.reset.setGeometry(QtCore.QRect(410, 320, 165, 32))
        self.reset.setObjectName("reset")
        ALife.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(ALife)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 760, 22))
        self.menubar.setObjectName("menubar")
        ALife.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(ALife)
        self.statusbar.setObjectName("statusbar")
        ALife.setStatusBar(self.statusbar)

        self.retranslateUi(ALife)
        self.legs.setCurrentIndex(1)
        self.body_density.setCurrentIndex(1)
        self.algorithm.setCurrentIndex(1)
        self.asteroid_mass.setCurrentIndex(1)
        QtCore.QMetaObject.connectSlotsByName(ALife)

    def retranslateUi(self, ALife):
        ALife.setWindowTitle(QtGui.QApplication.translate("ALife", "ALife On An Asteroid", None, QtGui.QApplication.UnicodeUTF8))
        self.start_pause.setText(QtGui.QApplication.translate("ALife", "Start Simulation", None, QtGui.QApplication.UnicodeUTF8))
        self.weights_label.setText(QtGui.QApplication.translate("ALife", "Control weights:", None, QtGui.QApplication.UnicodeUTF8))
        self.weights.setText(QtGui.QApplication.translate("ALife", "Evolve or load to create", None, QtGui.QApplication.UnicodeUTF8))
        self.legs_label.setText(QtGui.QApplication.translate("ALife", "No. Legs On The Robot:", None, QtGui.QApplication.UnicodeUTF8))
        self.legs.setItemText(0, QtGui.QApplication.translate("ALife", "2", None, QtGui.QApplication.UnicodeUTF8))
        self.legs.setItemText(1, QtGui.QApplication.translate("ALife", "4", None, QtGui.QApplication.UnicodeUTF8))
        self.legs.setItemText(2, QtGui.QApplication.translate("ALife", "6", None, QtGui.QApplication.UnicodeUTF8))
        self.evolve.setText(QtGui.QApplication.translate("ALife", "Evolve New", None, QtGui.QApplication.UnicodeUTF8))
        self.body_density_label.setText(QtGui.QApplication.translate("ALife", "Robot Body Density:", None, QtGui.QApplication.UnicodeUTF8))
        self.body_density.setItemText(0, QtGui.QApplication.translate("ALife", "0.25", None, QtGui.QApplication.UnicodeUTF8))
        self.body_density.setItemText(1, QtGui.QApplication.translate("ALife", "0.3", None, QtGui.QApplication.UnicodeUTF8))
        self.body_density.setItemText(2, QtGui.QApplication.translate("ALife", "0.4", None, QtGui.QApplication.UnicodeUTF8))
        self.body_density.setItemText(3, QtGui.QApplication.translate("ALife", "0.5", None, QtGui.QApplication.UnicodeUTF8))
        self.leg_density_label.setText(QtGui.QApplication.translate("ALife", "Robot Leg Density:             ", None, QtGui.QApplication.UnicodeUTF8))
        self.leg_density.setItemText(0, QtGui.QApplication.translate("ALife", "0.25", None, QtGui.QApplication.UnicodeUTF8))
        self.leg_density.setItemText(1, QtGui.QApplication.translate("ALife", "0.3", None, QtGui.QApplication.UnicodeUTF8))
        self.leg_density.setItemText(2, QtGui.QApplication.translate("ALife", "0.4", None, QtGui.QApplication.UnicodeUTF8))
        self.leg_density.setItemText(3, QtGui.QApplication.translate("ALife", "0.5", None, QtGui.QApplication.UnicodeUTF8))
        self.fitness_label.setText(QtGui.QApplication.translate("ALife", "Fitness:", None, QtGui.QApplication.UnicodeUTF8))
        self.fitness.setText(QtGui.QApplication.translate("ALife", "Unknown", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm_label.setText(QtGui.QApplication.translate("ALife", "Algorithm:", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(0, QtGui.QApplication.translate("ALife", "DE", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(1, QtGui.QApplication.translate("ALife", "IHS", None, QtGui.QApplication.UnicodeUTF8))
        self.algorithm.setItemText(2, QtGui.QApplication.translate("ALife", "PSO", None, QtGui.QApplication.UnicodeUTF8))
        self.generations_label.setText(QtGui.QApplication.translate("ALife", "No. Generations:", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(0, QtGui.QApplication.translate("ALife", "1", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(1, QtGui.QApplication.translate("ALife", "2", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(2, QtGui.QApplication.translate("ALife", "3", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(3, QtGui.QApplication.translate("ALife", "4", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(4, QtGui.QApplication.translate("ALife", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(5, QtGui.QApplication.translate("ALife", "10", None, QtGui.QApplication.UnicodeUTF8))
        self.generations.setItemText(6, QtGui.QApplication.translate("ALife", "20", None, QtGui.QApplication.UnicodeUTF8))
        self.islands_label.setText(QtGui.QApplication.translate("ALife", "No. Islands:", None, QtGui.QApplication.UnicodeUTF8))
        self.islands.setItemText(0, QtGui.QApplication.translate("ALife", "1", None, QtGui.QApplication.UnicodeUTF8))
        self.islands.setItemText(1, QtGui.QApplication.translate("ALife", "2", None, QtGui.QApplication.UnicodeUTF8))
        self.islands.setItemText(2, QtGui.QApplication.translate("ALife", "3", None, QtGui.QApplication.UnicodeUTF8))
        self.islands.setItemText(3, QtGui.QApplication.translate("ALife", "4", None, QtGui.QApplication.UnicodeUTF8))
        self.islands.setItemText(4, QtGui.QApplication.translate("ALife", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.individuals_label.setText(QtGui.QApplication.translate("ALife", "No. Individuals Per Island:", None, QtGui.QApplication.UnicodeUTF8))
        self.individuals.setItemText(0, QtGui.QApplication.translate("ALife", "3", None, QtGui.QApplication.UnicodeUTF8))
        self.individuals.setItemText(1, QtGui.QApplication.translate("ALife", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.individuals.setItemText(2, QtGui.QApplication.translate("ALife", "10", None, QtGui.QApplication.UnicodeUTF8))
        self.robot_environment_label.setText(QtGui.QApplication.translate("ALife", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Lucida Grande\'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Robot and Environment</p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.evolution_label.setText(QtGui.QApplication.translate("ALife", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Lucida Grande\'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Evolution</p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.load.setText(QtGui.QApplication.translate("ALife", "Load", None, QtGui.QApplication.UnicodeUTF8))
        self.save.setText(QtGui.QApplication.translate("ALife", "Save", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass_label.setText(QtGui.QApplication.translate("ALife", "Asteroid Mass:", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass.setItemText(0, QtGui.QApplication.translate("ALife", "5000", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass.setItemText(1, QtGui.QApplication.translate("ALife", "10000", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass.setItemText(2, QtGui.QApplication.translate("ALife", "15000", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass.setItemText(3, QtGui.QApplication.translate("ALife", "20000", None, QtGui.QApplication.UnicodeUTF8))
        self.asteroid_mass.setItemText(4, QtGui.QApplication.translate("ALife", "30000", None, QtGui.QApplication.UnicodeUTF8))
        self.reset.setText(QtGui.QApplication.translate("ALife", "Reset Simulation", None, QtGui.QApplication.UnicodeUTF8))

