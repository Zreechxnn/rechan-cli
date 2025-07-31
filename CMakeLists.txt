cmake_minimum_required(VERSION 3.15)
project(rechan VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann
)

set(QRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")
qt_add_resources(Rechan_RCC ${QRC_FILE})

set(SOURCES
    main.cpp
    command_dispatcher.cpp
)

add_executable(rechan
    ${SOURCES}
    ${Rechan_RCC}
)

target_link_libraries(rechan PRIVATE
    Qt5::Widgets
    Qt5::Core
)

install(TARGETS rechan DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/commands.json DESTINATION share/rechan)