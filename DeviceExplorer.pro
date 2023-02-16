TEMPLATE = app
QT += widgets serialport network qml quick quickwidgets
CONFIG += qt serialport c++14
QMAKE_CFLAGS += -std=gnu11
DEFINES *= QT_USE_QSTRINGBUILDER

DEFINES += QT
DEFINES += USE_LOG_TREE

INCLUDEPATH += LIB/QHID
#FIXME: find it and include: QMAKE_LIBS += /usr/lib/libQHidApi.a
unix|win32:LIBS += -ludev

INCLUDEPATH += LIB
INCLUDEPATH += LIB/QHID
INCLUDEPATH += LIB/systools
INCLUDEPATH += test


HEADERS += app_config.h toolbar.h MainWindow.h \
    MainTreeWidget.h \
    LocalHostWidget.h \
    fdefault.h \
    fdevice.h \
    LIB/smo.h \
    LIB/zvvlib.h \
    LIB/smolib.h \
    LIB/SmoRecord.h \
    LIB/deviceinfo.h \
    LIB/deplugin.h \
    LIB/fcomport.h \
    LIB/fhidport.h \
    LIB/fmessage.h \
    LIB/LogTree.h \
    LIB/treelib.h \
    LIB/jconfig.h \
    LIB/tdiportserial.h \
    LIB/debugqt.h \
    LIB/CRC.h \
    LIB/Offseters.h \
    LIB/SmoBusData.h \
    LIB/g8enum.h \
    smoui.h \
    smouipageheader.h \
    smouitextline.h \
    smouilogin.h \
    smouipassline.h \
    smouiselectitem.h \
    smouiMessage.h \
    LIB/realtime32.h \
    LIB/smofoldertype.h \
    LIB/tdiport.h \
    LIB/tdiporthid.h \
    LIB/fappconfig.h \
    deconfig.h \
    fdockqml.h \
    test/fbitem.h \
    LIB/dewidget.h \
    LIB/smodatatypes.h \
    LIB/smomodel.h \
    LIB/smoinfo.h \
    LIB/smolistproperty.h \
    LIB/treeitem.h

SOURCES += main.cpp \
    toolbar.cpp \
    MainWindow.cpp \
    MainTreeWidget.cpp \
    LocalHostWidget.cpp \
    fdefault.cpp \
    fdevice.cpp \
    LIB/smo.cpp \
    LIB/zvvlib.cpp \
    LIB/smolib.cpp \
    LIB/deviceinfo.cpp \
    LIB/deplugin.cpp \
    LIB/fcomport.cpp \
    LIB/fhidport.cpp \
    LIB/fmessage.cpp \
    LIB/LogTree.cpp \
    LIB/treelib.cpp \
    LIB/jconfig.cpp \
    LIB/tdiportserial.cpp \
    LIB/CRC.c \
    LIB/Offseters.c \
    LIB/g8enum.cpp \
    smoui.cpp \
    smouipageheader.cpp \
    smouitextline.cpp \
    smouilogin.cpp \
    smouipassline.cpp \
    smouiselectitem.cpp \
    smouiMessage.cpp \
    LIB/realtime32.cpp \
    LIB/tdiport.cpp \
    LIB/tdiporthid.cpp \
    LIB/fappconfig.cpp \
    deconfig.cpp \
    fdockqml.cpp \
    test/fbitem.cpp \
    LIB/dewidget.cpp \
    LIB/smomodel.cpp \
    LIB/smoinfo.cpp

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

RESOURCES += DE.qrc

OTHER_FILES += "QML/LIB/*.*"
OTHER_FILES += "QML/APPS/FATTEST/DATA/*.*"
OTHER_FILES += "QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/*.*"
OTHER_FILES += "QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/*.*"
OTHER_FILES += "QML/APPS/FATTEST/DATA/RAM DATA/Complex types/*.*"
OTHER_FILES += "QML/APPS/FATTEST/FORMATS/*.*"
OTHER_FILES += "QML/APPS/FATTEST/Config/*.*"

QMAKE_POST_LINK = "/usr/bin/nm -S $(OBJECTS_DIR)/$(TARGET) >$(OBJECTS_DIR)/$(TARGET).nm"
OTHER_FILES += $$OUT_PWD/*.nm

DISTFILES += \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/MyTxtComponent.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/Completed_Destruction_events_Component.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/SearchLineEdit.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/SearchBar.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/Button.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/Complex types/ZTumbler.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/Complex types/ZCircularGauge.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/Complex types/ZGauge.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/TumblerListModel.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/AppTheme.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/Stack_layout.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/Tumbler_example.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/DEHeader.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/SCircularGauge.qml \
    QML/LIB/SCircularGaugeStyle.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/DBexample.qml \
    QML/APPS/FATTEST/DATA/RAM DATA/DYNAMIC/UICTRL.txt/MyButton.qml
