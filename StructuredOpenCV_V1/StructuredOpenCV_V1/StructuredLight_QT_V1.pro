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
# \                 /usr/include/eigen3/ \
#                 /usr/include/pcl-1.7/ \
#                 /usr/include/vtk-5.8/ \
#                 /usr/include/boost/ \
#                 /usr/include/flann \
#                 /usr/lib/

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
#LIBS += -lQtGui -lQtCore -lQtOpenGL \
#        -lpcl_registration -lpcl_sample_consensus -lpcl_features -lpcl_filters -lpcl_surface -lpcl_segmentation \
#        -lpcl_search -lpcl_kdtree -lpcl_octree -lflann_cpp -lpcl_common -lpcl_io \
#        -lpcl_visualization \
#        -L/usr/lib -lvtkCommon -lvtksys -lvtkQtChart -lvtkViews -lvtkWidgets  -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtkDICOMParser -lvtkmetaio -lvtkexoIIc -lvtkHybrid \
#        -L/usr/lib -lboost_thread

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

OTHER_FILES += \
    in_calib.xml
