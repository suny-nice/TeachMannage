cmake_minimum_required(VERSION 3.19)
project(StudentMannageSystem LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Charts)

qt_standard_project_setup()

qt_add_resources(RESOURCES img.qrc)

qt_add_executable(StudentMannageSystem
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    database.cpp
    database.h
    studentwidget.h studentwidget.cpp studentwidget.ui
    addstudentdialog.h addstudentdialog.cpp
    addfinancialdialog.h addfinancialdialog.cpp
    ${RESOURCES}
    schedulewidget.h schedulewidget.cpp schedulewidget.ui
    financialwidget.h financialwidget.cpp financialwidget.ui

)

target_include_directories(StudentMannageSystem
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(StudentMannageSystem
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Charts
)

include(GNUInstallDirs)

install(TARGETS StudentMannageSystem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET StudentMannageSystem
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
