cmake_minimum_required(VERSION 3.19)

project(buddycoding LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(buddycoding
    WIN32 MACOSX_BUNDLE

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    loginwindow.cpp
    loginwindow.h

    signupwindow.cpp
    signupwindow.h

    authmanager.cpp
    authmanager.h
)

target_link_libraries(buddycoding
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

install(TARGETS buddycoding
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET buddycoding
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})