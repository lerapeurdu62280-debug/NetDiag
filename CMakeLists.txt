cmake_minimum_required(VERSION 3.10)
project(NetDiag)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows specific settings
if(WIN32)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Utils.cpp
    src/NetworkInfo.cpp
    src/PingTest.cpp
    src/DNSTest.cpp
    src/TracerouteTest.cpp
    src/PortScanTest.cpp
    src/SpeedTest.cpp
)

# Header files
set(HEADERS
    src/Utils.h
    src/NetworkInfo.h
    src/PingTest.h
    src/DNSTest.h
    src/TracerouteTest.h
    src/PortScanTest.h
    src/SpeedTest.h
)

# Create executable
add_executable(NetDiag ${SOURCES} ${HEADERS})

# Link Windows libraries
if(WIN32)
    target_link_libraries(NetDiag
        ws2_32
        iphlpapi
        icmpapi
        wininet
        setupapi
    )
endif()

# Include directories
target_include_directories(NetDiag PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Output directory
set_target_properties(NetDiag PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)