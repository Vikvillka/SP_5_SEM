cmake_minimum_required(VERSION 3.10)
project(Lab-03b-client C)

set(CMAKE_C_STANDARD 11)

add_executable(Lab-03b-client Lab-03b-client.c)

target_compile_definitions(Lab-03b-client PRIVATE 
    _UNICODE 
    UNICODE 
    _CRT_SECURE_NO_WARNINGS
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(Lab-03b-client PRIVATE 
        -municode 
        -finput-charset=UTF-8
    )
endif()


target_link_libraries(Lab-03b-client PRIVATE 
    kernel32
    user32
)