cmake_minimum_required(VERSION 3.10)

# set the project name and version

set(PROJECT_NAME epaperdisplay_sample)
project(${PROJECT_NAME} VERSION 1.0)


add_executable(${PROJECT_NAME}
src/epaperdisplay_sample/Config/DEV_Config.c
src/epaperdisplay_sample/Config/DEV_Config.h
src/epaperdisplay_sample/e-Paper/EPD_IT8951.c
src/epaperdisplay_sample/e-Paper/EPD_IT8951.h
src/epaperdisplay_sample/Fonts/font8.c
src/epaperdisplay_sample/Fonts/font12.c
src/epaperdisplay_sample/Fonts/font12CN.c
src/epaperdisplay_sample/Fonts/font16.c
src/epaperdisplay_sample/Fonts/font20.c
src/epaperdisplay_sample/Fonts/font24.c
src/epaperdisplay_sample/Fonts/font24CN.c
src/epaperdisplay_sample/Fonts/fonts.h
src/epaperdisplay_sample/GUI/GUI_BMPfile.c
src/epaperdisplay_sample/GUI/GUI_BMPfile.h
src/epaperdisplay_sample/GUI/GUI_Paint.c
src/epaperdisplay_sample/GUI/GUI_Paint.h
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
