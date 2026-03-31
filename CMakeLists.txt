cmake_minimum_required(VERSION 3.16)

project(AtracsysSessionApp VERSION 0.1 LANGUAGES CXX)

# ==============================
# Qt Settings
# ==============================
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================
# Find Qt
# ==============================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# ==============================
# Sources
# ==============================
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    sessionmanager.h
    sessionmanager.cpp
    atracsys_manager.h
    atracsys_manager.cpp
)

# ==============================
# Executable
# ==============================
qt_add_executable(AtracsysSessionApp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    trackingworker.h trackingworker.cpp
    resources.qrc
)

# ==============================
# Atracsys Wrapper Path (IMPORTANT)
# ==============================
set(ATRACSYS_WRAPPER "C:/Users/sabar/Downloads/AtracsysWrapper-v1.1/ReleasePackage")

# Include headers
target_include_directories(AtracsysSessionApp PRIVATE
    "${ATRACSYS_WRAPPER}/include"
)

# Link wrapper library ONLY
target_link_libraries(AtracsysSessionApp
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    "${ATRACSYS_WRAPPER}/lib/AtracsysWrapper.lib"
)

# ==============================
# Finalize
# ==============================
qt_finalize_executable(AtracsysSessionApp)
