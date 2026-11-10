cmake_minimum_required(VERSION 3.10)
project(MetadataManager VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(EXIV2 REQUIRED IMPORTED_TARGET exiv2)

set(SOURCES
    src/main.cpp
    src/ImageMetadata.cpp
    src/MetadataReader.cpp
    src/MetadataEditor.cpp
    src/MetadataDeleter.cpp
)

add_executable(metadata_manager ${SOURCES})

target_include_directories(metadata_manager PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Вот ключевая строка — тянет и -I, и -L, и -lexiv2
target_link_libraries(metadata_manager PRIVATE PkgConfig::EXIV2)