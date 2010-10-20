CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (ImageTools)

ADD_DEFINITIONS("-O3")

ADD_EXECUTABLE (extract_rectangles_ppm extract_rectangles_ppm.cpp)
TARGET_LINK_LIBRARIES (extract_rectangles_ppm "-lnetpbm")

ADD_EXECUTABLE (replace_color replace_color.cpp)
TARGET_LINK_LIBRARIES (replace_color "-lnetpbm")
