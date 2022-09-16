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

    # fatfs/diskio.c
    # fatfs/diskio.h
    fatfs/ff.c

    # fatfs/ff.h
    # fatfs/ffconf.h
    fatfs/ffsystem.c
    fatfs/ffunicode.c
    fatfs/mmc_pico_spi.c
    example.h
    example.c
    main.c

    # https://qiita.com/iwatake2222/items/da9a7f1e3552b55aa219
    jpeg/jaricom.c
    jpeg/jcapimin.c
    jpeg/jcapistd.c
    jpeg/jcarith.c
    jpeg/jccoefct.c
    jpeg/jccolor.c
    jpeg/jcdctmgr.c
    jpeg/jchuff.c
    jpeg/jcinit.c
    jpeg/jcmainct.c
    jpeg/jcmarker.c
    jpeg/jcmaster.c
    jpeg/jcomapi.c
    jpeg/jconfig.h
    jpeg/jcparam.c
    jpeg/jcprepct.c
    jpeg/jcsample.c
    jpeg/jctrans.c
    jpeg/jdapimin.c
    jpeg/jdapistd.c
    jpeg/jdarith.c
    jpeg/jdatadst.c
    jpeg/jdatasrc.c
    jpeg/jdcoefct.c
    jpeg/jdcolor.c
    jpeg/jdct.h
    jpeg/jddctmgr.c
    jpeg/jdhuff.c
    jpeg/jdinput.c
    jpeg/jdmainct.c
    jpeg/jdmarker.c
    jpeg/jdmaster.c
    jpeg/jdmerge.c
    jpeg/jdpostct.c
    jpeg/jdsample.c
    jpeg/jdtrans.c
    jpeg/jerror.c
    jpeg/jerror.h
    jpeg/jfdctflt.c
    jpeg/jfdctfst.c
    jpeg/jfdctint.c
    jpeg/jidctflt.c
    jpeg/jidctfst.c
    jpeg/jidctint.c
    jpeg/jinclude.h
    jpeg/jmemansi.c
    jpeg/jmemmgr.c
    jpeg/jmemsys.h
    jpeg/jmorecfg.h
    jpeg/jpegint.h
    jpeg/jpeglib.h
    jpeg/jquant1.c
    jpeg/jquant2.c
    jpeg/jutils.c
    jpeg/jversion.h
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    hardware_interp
    hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})
