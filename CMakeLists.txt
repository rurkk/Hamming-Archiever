cmake_minimum_required(VERSION 3.23)

project(labwork_4_rurkk)

set(CMAKE_CXX_STANDARD 17)

SET(TARGET_H
        hamarc.h
        hamming.h
        )

SET(TARGET_SRC
        main.cpp
        hamarc.cpp
        hamming.cpp
        )

add_executable(main.cpp hamarc.cpp hamming.cpp "main.cpp" hamarc.h hamming.h)
