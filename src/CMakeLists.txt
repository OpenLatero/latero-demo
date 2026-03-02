set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "--std=gnu++11 -fpermissive ${CMAKE_CXX_FLAGS}")

find_package(laterographics)
find_package(Boost REQUIRED COMPONENTS program_options)

set(latero_demo_SRCS
    mainwindow.h
    mainwindow.cpp
    main.cpp
    managerwidget.h
    managerwidget.cpp
    drawing.h
    drawing.cpp)

add_executable(latero-demo ${latero_demo_SRCS})
target_link_libraries(latero-demo "-framework Foundation" Boost::program_options laterographics::laterographics)
install_programs(/bin latero-demo)
