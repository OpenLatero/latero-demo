include(FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "--std=gnu++11 -fpermissive ${CMAKE_CXX_FLAGS}")

# add /usr/loca/lib/pkgconfig (necessary on macOS)
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig" )

# todo: added manually - should it be necessary or already set up by latero-graphics?
find_package(Boost CONFIG COMPONENTS system thread filesystem REQUIRED)
find_package(latero)

pkg_check_modules (LATERO_GRAPHICS latero-graphics)
link_directories(${LATERO_GRAPHICS_LIBRARY_DIRS})
include_directories(${LATERO_GRAPHICS_INCLUDE_DIRS})

set(latero_demo_SRCS
    mainwindow.h
    mainwindow.cpp
    main.cpp
    managerwidget.h
    managerwidget.cpp
    drawing.h
    drawing.cpp)

add_executable(latero-demo ${latero_demo_SRCS})
target_link_libraries(latero-demo "-framework Foundation" ${LATERO_GRAPHICS_LIBRARIES} boost_program_options-mt latero::latero)
install_programs(/bin latero-demo)
