QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Parser.cpp \
    STEList.cpp \
    STList.cpp \
    ast.cpp \
    fd.cpp \
    fdTest.cpp \
    main.cpp \
    mainST.cpp \
    scanner.cpp \
    stable.cpp

HEADERS += \
    Parser.h \
    STEList.h \
    STList.h \
    ast.h \
    fd.h \
    scanner.h \
    stable.h \
    stentry.h

FORMS +=

TRANSLATIONS += \
    Compiler2023_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
