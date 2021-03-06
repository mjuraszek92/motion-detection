#-------------------------------------------------
#
# Project created by QtCreator 2014-11-21T00:33:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zez
TEMPLATE = app


SOURCES += main.cpp\
    manual.cpp \
    mainwindow.cpp \
    ggthread.cpp \
    common.cpp \
    full_auto.cpp \
    thread_auto.cpp \
    ffthread.cpp






HEADERS  += mainwindow.h \
    ggthread.h \
    ffthread.h

FORMS    += mainwindow.ui

CONFIG += c++11

INCLUDEPATH += D:\\Programy\\opencv\\build\\include

LIBS += -LD:\\Programy\\opencv\\build_with_Qt\\lib\\Debug \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_core249d \
    -lopencv_features2d249d \
    -lopencv_flann249d \
    -lopencv_gpu249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_legacy249d \
    -lopencv_ml249d \
    -lopencv_nonfree249d \
    -lopencv_objdetect249d \
    -lopencv_ocl249d \
    -lopencv_photo249d \
    -lopencv_stitching249d \
    -lopencv_superres249d \
    -lopencv_ts249d \
    -lopencv_video249d \
    -lopencv_videostab249d
