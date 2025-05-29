add_executable(app)

target_sources(app PRIVATE 
    "main.c"
)

target_include_directories(app PRIVATE 
    ${CMAKE_DIR}/stm32cubemx
    ${APP_DIR}/hc_sr04
)

target_link_libraries(app PRIVATE
    stm32cubemx
    hc_sr04
)

target_compile_options(app PUBLIC
    -std=c23
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
)
