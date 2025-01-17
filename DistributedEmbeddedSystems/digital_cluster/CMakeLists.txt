cmake_minimum_required(VERSION 3.5)

project(digital_cluster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

set(CMAKE_CTEST_ARGUMENTS "--output-on-failure --force-new-ctest-process")
set(CMAKE_COLOR_MAKEFILE ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(include)
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)
        
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Mqtt Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt)

include_directories(${CMAKE_SOURCE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(digital_cluster
        MANUAL_FINALIZATION
        ${SOURCES} 
        ${HEADERS}
        main.cpp
        )
        qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(digital_cluster SHARED
            ${SOURCES}
        )
        else()
            add_executable(digital_cluster
            ${SOURCES}
            main.cpp
            ${HEADERS}
            )
            endif()
        endif()

target_link_libraries(digital_cluster
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Mqtt
)

file(GLOB TESTS test/*.cpp)

add_executable(digital_cluster_test
    ${SOURCES}
    ${HEADERS}
    ${TESTS}
)

target_link_libraries(digital_cluster_test
    gtest
    gtest_main
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Mqtt
    )

enable_testing()

add_test(NAME DigitalClusterTest COMMAND digital_cluster_test)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(digital_cluster)
endif()
# Set properties for macOS/iOS packaging
set_target_properties(digital_cluster PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install the application
include(GNUInstallDirs)
install(TARGETS digital_cluster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(digital_cluster)
endif()