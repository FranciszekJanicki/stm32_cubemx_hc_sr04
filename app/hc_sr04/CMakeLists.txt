add_library(hc_sr04 STATIC)

target_sources(hc_sr04 PRIVATE 
    "hc_sr04.cpp"
    "interrupts.cpp"
)

target_include_directories(hc_sr04 PUBLIC 
    "."
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(hc_sr04 PRIVATE
    utility
    stm32cubemx
)

target_compile_options(hc_sr04 PUBLIC
    -std=c++23
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Waddress
    -pedantic
    -Wdeprecated
    -Wsign-conversion
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wcast-align
    -fconcepts
)
