QT       += core gui webenginewidgets webenginecore

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#INCLUDEPATH = A:/Qt/xlnt_x64-windows/include
#LIBS += A:/Qt/xlnt_x64-windows/debug/lib/xlntd.lib

INCLUDEPATH = C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/include
LIBS += C:/Users/kznts/vcpkg/packages/xlnt_x64-windows/debug/lib/xlntd.lib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/screenshot.cpp \
    classes/table.cpp \
    forms/mainwindow.cpp \
    forms/web.cpp \
    main.cpp \

HEADERS += \
    classes/screenshot.h \
    classes/table.h \
    forms/mainwindow.h \
    forms/web.h

FORMS += \
    forms/mainwindow.ui \
    forms/web.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
