QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MatToQImage.cpp \
    main.cpp \
    mainwindow.cpp \
    thread_grabframe_basler.cpp \
    threadbaslerpylon.cpp

HEADERS += \
    MatToQImage.h \
    mainwindow.h \
    thread_grabframe_basler.h \
    threadbaslerpylon.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




win32:CONFIG(release, debug|release): LIBS += -LG:/OpenCv/opencv_4.1.0/build/x64/vc15/lib/ -lopencv_world410
else:win32:CONFIG(debug, debug|release): LIBS += -LG:/OpenCv/opencv_4.1.0/build/x64/vc15/lib/ -lopencv_world410d
else:unix: LIBS += -LG:/OpenCv/opencv_4.1.0/build/x64/vc15/lib/ -lopencv_world410

INCLUDEPATH += G:/OpenCv/opencv_4.1.0/build/include
DEPENDPATH += G:/OpenCv/opencv_4.1.0/build/include

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

# LIB Pylon
win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/Basler/pylon 6/Development/lib/x64/' -lPylonC
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/Basler/pylon 6/Development/lib/x64/' -lPylonCd
else:unix: LIBS += -L'C:/Program Files/Basler/pylon 6/Development/lib/x64/' -lPylonC

INCLUDEPATH += 'C:/Program Files/Basler/pylon 6/Development/include'
DEPENDPATH += 'C:/Program Files/Basler/pylon 6/Development/include'
