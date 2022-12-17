cmake_minimum_required(VERSION 3.10)
project(LibraryProject)
set(SOURCE_LIB Rule.cpp Grammar.cpp Situation.cpp ParserEarley.cpp)
add_library(parsers_lib SHARED ${SOURCE_LIB})