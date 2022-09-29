cmake_minimum_required(VERSION 3.10)

# set the project name and version
set(PROJECT_NAME epaperdisplay_sample)
project(${PROJECT_NAME} VERSION 1.0)

include_directories(${CMAKE_SOURCE_DIR}/src/epaperdisplay_sample)

add_executable(${PROJECT_NAME}

    Config/DEV_Config.h
    Config/DEV_Config.c
    e-Paper/EPD_IT8951.h
    e-Paper/EPD_IT8951.c
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

    # fatfs/diskio.c
    # fatfs/diskio.h
    fatfs/ff.c

    fatfs/ff.h
    fatfs/ffconf.h
    fatfs/ffsystem.c
    fatfs/ffunicode.c
    fatfs/mmc_pico_spi.c
    example.h
    example.c
    main.c

    # http://elm-chan.org/fsw/tjpgd/00index.html
    jpeg/tjpgd.c
    jpeg/tjpgd.h
    jpeg/tjpgdcnf.h
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    hardware_interp
    hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})
