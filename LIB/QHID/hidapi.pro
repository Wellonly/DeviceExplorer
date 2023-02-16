
TARGET = QHidApi

TEMPLATE  = lib

DESTDIR = $$PWD/../../../../libs

DEFINES += QHIDAPI_LIBRARY
#DEFINES += USE_LIBUSB

CONFIG += build_hidapi_lib
CONFIG += static

include(hidapi.pri)

#QMAKE_POST_LINK = "/usr/bin/objdump -tgsx $(DESTDIR)/$(TARGET) >$(OBJECTS_DIR)/$(TARGET).objdump"
#OTHER_FILES += $$OUT_PWD/*.objdump
