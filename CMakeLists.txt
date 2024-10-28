cmake_minimum_required(VERSION 3.29)
project(SMS)

set(CMAKE_CXX_STANDARD 20)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Include SQLite3 directories
include_directories(${SQLite3_INCLUDE_DIRS})

# Define the executable and source files
add_executable(SMS main.cpp
        Student.h
        Student.cpp  # <-- Make sure this line is here to include Student.cpp
        Grade.h
        Grade.cpp
        Report.h
        Report.cpp
        Database.h
        Database.cpp
        Student.cpp
)

# Link SQLite3 library
target_link_libraries(SMS ${SQLite3_LIBRARIES})
