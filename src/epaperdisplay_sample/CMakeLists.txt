cmake_minimum_required(VERSION 3.10)

# set the project name and version

set(PROJECT_NAME epaperdisplay_sample)
project(${PROJECT_NAME} VERSION 1.0)


add_executable(${PROJECT_NAME}
Config/DEV_Config.c
Config/DEV_Config.h
e-Paper/EPD_IT8951.c
e-Paper/EPD_IT8951.h
Fonts/font8.c
Fonts/font12.c
Fonts/font12CN.c
Fonts/font16.c
Fonts/font20.c
Fonts/font24.c
Fonts/font24CN.c
Fonts/fonts.h
GUI/GUI_BMPfile.c
GUI/GUI_BMPfile.h
GUI/GUI_Paint.c
GUI/GUI_Paint.h
example.h
example.c
main.c
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME} 
pico_stdlib
hardware_pio
hardware_interp
hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})
