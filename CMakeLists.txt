cmake_minimum_required (VERSION 2.4)
project (Assign6)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-std=c++11 -O3 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(Assign6
    main.cpp
    )
