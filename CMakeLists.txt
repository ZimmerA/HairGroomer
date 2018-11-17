cmake_minimum_required(VERSION 3.12)
project(HairGroomer)
set(CMAKE_CXX_STANDARD 14)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# TODO: compiler check and use different versions
set (CMAKE_PREFIX_PATH "C:/Qt/5.11.2/mingw53_32")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# library includes
# TODO: include_directories(${PROJECT_SOURCE_DIR}/libs/fbxsdk/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}/libs/glm)
include_directories(${PROJECT_SOURCE_DIR}/libs/json/include)

# QT includes
include_directories(${CMAKE_PREFIX_PATH}/include)
include_directories(${CMAKE_PREFIX_PATH}/include/QtCore)
include_directories(${CMAKE_PREFIX_PATH}/include/QtWidgets)
include_directories(${CMAKE_PREFIX_PATH}/include/QtGui)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.h* ${PROJECT_SOURCE_DIR}/src/*.c*
                        ${PROJECT_SOURCE_DIR}/libs/*.h* ${PROJECT_SOURCE_DIR}/libs/*.c*)

add_executable(HairGroomer ${SRC})

# link dlls
target_link_libraries(HairGroomer Qt5::Widgets)
target_link_libraries(HairGroomer Qt5::OpenGL)

# Copy Shaders and resources to binary directory
file(COPY ${PROJECT_SOURCE_DIR}/src/rendering/shaders DESTINATION ${CMAKE_BINARY_DIR}/src/rendering/)
file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/)

# Windows specific post build steps
if(WIN32)
    # Run winddeployqt if it can be found to copy required dlls to binary directory
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${CMAKE_PREFIX_PATH} ENV QTDIR PATH_SUFFIXES bin)
    add_custom_command(TARGET HairGroomer POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} --list source $<TARGET_FILE:HairGroomer>)
endif()