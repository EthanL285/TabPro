cmake_minimum_required(VERSION 3.5)

project(TabPro VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS NetworkAuth)
find_package(Qt6 REQUIRED COMPONENTS HttpServer)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Test)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TabPro
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        src/model/staff/staff.h src/model/staff/staff.cpp
        src/model/staff/symbols/notes/crotchet.h src/model/staff/symbols/notes/crotchet.cpp
        src/model/staff/symbols/notes/quaver.h src/model/staff/symbols/notes/quaver.cpp
        src/model/staff/symbols/notes/note.h src/model/staff/symbols/notes/note.cpp
        src/model/staff/symbols/notes/notetype.h
        src/model/staff/symbols/notes/blank.h src/model/staff/symbols/notes/blank.cpp
        src/model/staff/symbols/notes/blank.h src/model/staff/symbols/notes/blank.cpp
        src/model/staff/symbols/notes/quaverfamily.h
        src/model/staff/symbols/rests/rest.h src/model/staff/symbols/rests/rest.cpp
        src/model/staff/symbols/rests/eighthrest.h src/model/staff/symbols/rests/eighthrest.cpp
        src/model/staff/symbols/rests/halfrest.h src/model/staff/symbols/rests/halfrest.cpp
        src/model/staff/symbols/rests/quarterrest.h src/model/staff/symbols/rests/quarterrest.cpp
        src/model/staff/symbols/rests/resttype.h
        src/model/staff/symbols/rhythmsymbol.h src/model/staff/symbols/rhythmsymbol.cpp
        src/model/staff/symbols/rests/restfactory.h src/model/staff/symbols/rests/restfactory.cpp
        src/model/staff/symbols/notes/notefactory.h src/model/staff/symbols/notes/notefactory.cpp

        src/model/tablature/tabjsonconverter.h src/model/tablature/tabjsonconverter.cpp
        src/model/tablature/tablature.h src/model/tablature/tablature.cpp


        src/model/barlines/barline.h src/model/barlines/barline.cpp
        src/model/helpers/signaturehelper.h src/model/helpers/signaturehelper.cpp

        src/controller/containers/containerwidget.h src/controller/containers/containerwidget.cpp
        src/controller/containers/mainwidget.h src/controller/containers/mainwidget.cpp

        src/controller/user/authmanager.h src/controller/user/authmanager.cpp
        src/controller/user/verificationstatus.h
        src/controller/score/scoreupdater.h src/controller/score/scoreupdater.cpp
        src/controller/sound/sound.h src/controller/sound/sound.cpp

        src/view/menu/menubar.h src/view/menu/menubar.cpp
        src/view/menu/filemenu.h src/view/menu/filemenu.cpp
        src/view/menu/filemenu.h src/view/menu/filemenu.cpp

        src/view/chorddiagram/chordwindow.h src/view/chorddiagram/chordwindow.cpp
        src/view/chorddiagram/chorddiagram.h src/view/chorddiagram/chorddiagram.cpp
        src/view/chorddiagram/toggleswitch.h src/view/chorddiagram/toggleswitch.cpp
        src/view/chorddiagram/chorddisplay.h src/view/chorddiagram/chorddisplay.cpp

        src/view/user/loginui.h src/view/user/loginui.cpp
        src/view/user/registerui.h src/view/user/registerui.cpp
        src/view/user/uiwidgets.h src/view/user/uiwidgets.cpp
        src/view/user/transitions.h src/view/user/transitions.cpp
        src/view/user/passwordui.h src/view/user/passwordui.cpp

        src/view/components/buttons/resetbutton.h src/view/components/buttons/resetbutton.cpp
        src/view/components/buttons/signaturebutton.h src/view/components/buttons/signaturebutton.cpp
        src/view/components/menus/signaturemenu.h src/view/components/menus/signaturemenu.cpp
        src/view/components/spinboxes/spinbox.h src/view/components/spinboxes/spinbox.cpp
        src/view/components/widgets/signaturewidget.h src/view/components/widgets/signaturewidget.cpp

        src/view/tablature/tablatureindent.h src/view/tablature/tablatureindent.cpp
        src/view/tablature/tablaturescrollarea.h src/view/tablature/tablaturescrollarea.cpp

        src/resources/chords/chords.qrc
        src/resources/icons/icons.qrc
        src/resources/sound/sound.qrc

        tests/model/tablature/testtablature.h tests/model/tablature/testtablature.cpp
        tests/model/staff/teststaff.h tests/model/staff/teststaff.cpp
        tests/tabprocontroller.h tests/tabprocontroller.cpp
        src/model/tablature/tabcolumn.h src/model/tablature/tabcolumn.cpp
        src/model/tablature/tabfret.h src/model/tablature/tabfret.cpp
        src/model/tablature/tabstate.h

    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TabPro APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TabPro SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TabPro
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TabPro PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TabPro PRIVATE Qt6::Sql)
target_link_libraries(TabPro PRIVATE Qt6::Network)
target_link_libraries(TabPro PRIVATE Qt6::NetworkAuth)
target_link_libraries(TabPro PRIVATE Qt6::HttpServer)
target_link_libraries(TabPro PRIVATE Qt6::Multimedia)
target_link_libraries(TabPro PRIVATE Qt6::Test)

target_include_directories(
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/controller/sound
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/controller/containers
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/controller/user
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/controller/score
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/tablature
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/staff
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/chorddiagram
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/menu
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/user
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/components/buttons
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/components/labels
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/components/menus
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/components/spinboxes
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/view/components/widgets
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/tablature
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/staff
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/staff/symbols
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/staff/symbols/notes
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/staff/symbols/accidentals
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/staff/symbols/rests
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/barlines
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/model/helpers
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/src/resources/chords
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/tests
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/tests/model/staff
    TabPro PRIVATE ${PROJECT_SOURCE_DIR}/tests/model/tablature
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TabPro)
endif()
set_target_properties(TabPro PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TabPro
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TabPro)
endif()
