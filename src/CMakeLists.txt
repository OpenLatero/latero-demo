set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "--std=gnu++11 -fpermissive ${CMAKE_CXX_FLAGS}")

find_package(laterographics)

include(FindPkgConfig)
pkg_check_modules(DEPS REQUIRED gtkmm-2.4)
link_directories(${DEPS_LIBRARY_DIRS})
include_directories(${DEPS_INCLUDE_DIRS})


set(latero_demo_SRCS
    mainwindow.h
    mainwindow.cpp
    main.cpp
    managerwidget.h
    managerwidget.cpp
    drawing.h
    drawing.cpp)

add_executable(latero-demo ${latero_demo_SRCS})
target_link_libraries(latero-demo "-framework Foundation" boost_program_options-mt laterographics::laterographics)
install_programs(/bin latero-demo)
