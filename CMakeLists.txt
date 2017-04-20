cmake_minimum_required(VERSION 3.7)
project(wordcount)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c tool.c tool.h dict.c dict.h)


add_executable(wordcount ${SOURCE_FILES})