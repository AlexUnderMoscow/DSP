TEMPLATE = app
TARGET = DSP


greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

include(../propertybrowser/qtpropertybrowser.pri)

# install
target.path = $$PWD/debug
INSTALLS += target

QMAKE_CXXFLAGS += -pipe -fno-keep-inline-dllexport -g -std=gnu++11 -frtti -Wall -Wextra -fexceptions -mthreads -DUNICODE -DQT_QML_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB  -std=c++11 -lwpcap

INCLUDEPATH += g:/src/WpdPack/Include
LIBS += -L g:/src/WpdPack/Lib -lwpcap -lpacket -lwsock32


FORMS = adaptivefilter.ui \
	bitgenerator.ui \
	descrform.ui \
	fileclass.ui \
	filesave.ui \
	io.ui \
	iqdem.ui \
	psk2mod.ui \
	qpskmod.ui \
	summ.ui \
        xormod.ui \
    test1.ui \
    config.ui
RESOURCES   = diagramscene.qrc
HEADERS       = adaptivefilter.h \
                arrow.h \
                bitgenerator.h \
                descrform.h \
                diagramitem.h \
                diagramscene.h \
                diagramtextitem.h \
                fifo.h \
                fileclass.h \
                filesave.h \
                interface.h \
		io.h \
		iqdem.h \
		mainwindow.h \
		manager.h \
		operations.h \
		paint.h \
		phasediagramwidget.h \
		pool.h \
		psk2mod.h \
		qpskmod.h \
		spectrumwidget.h \
		struct.h \
		summ.h \
		thread.h \
                xormod.h \
    test1.h \
    qcustomplot.h \
    config.h
	

SOURCES       = main.cpp \
                adaptivefilter.cpp \
                arrow.cpp \
                bitgenerator.cpp \
                descrform.cpp \
                diagramitem.cpp \
                diagramscene.cpp \
                diagramtextitem.cpp \
                fifo.cpp \
                fileclass.cpp \
                filesave.cpp \
		interface.cpp \
		io.cpp \
		iqdem.cpp \
		mainwindow.cpp \
		manager.cpp \
		operations.cpp \
 		paint.cpp \
		phasediagramwidget.cpp \
		psk2mod.cpp \
		qpskmod.cpp \
		spectrumwidget.cpp \
		summ.cpp \
		thread.cpp \
    xormod.cpp \
    test1.cpp \
    qcustomplot.cpp \
    config.cpp

QT           += widgets

DISTFILES += \
    Makefile


