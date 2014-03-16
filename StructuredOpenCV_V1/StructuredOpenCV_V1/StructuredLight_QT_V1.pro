#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T04:14:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StructuredLight_QT_V1
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree

SOURCES += main.cpp\
        mainwindow.cpp \
    CameraCalibrator.cpp \
    Capturador.cpp \
    Decoder.cpp \
    MatrixUtil.cpp

HEADERS  += mainwindow.h \
    CameraCalibrator.h \
    Capturador.h \
    Decoder.h \
    MatrixUtil.h

FORMS    += mainwindow.ui
