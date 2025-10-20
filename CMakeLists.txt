cmake_minimum_required(VERSION 3.16)
project(DSA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DSA
        main.cpp                 # root-level main.cpp
        Src/HybridStructure.cpp  # source in Src/
)

# so #include "HybridStructure.h" works from anywhere
target_include_directories(DSA PRIVATE
        ${PROJECT_SOURCE_DIR}/Include
)
