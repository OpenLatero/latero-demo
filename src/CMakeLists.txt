find_package(laterographics)
find_package(Boost REQUIRED COMPONENTS program_options)

set(latero_demo_SRCS
    mainwindow.h
    mainwindow.cpp
    main.cpp)

add_executable(latero-demo ${latero_demo_SRCS})
set_target_properties(latero-demo PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(latero-demo "-framework Foundation" Boost::program_options laterographics::laterographics)
install_programs(/bin latero-demo)

#target_compile_definitions(latero-demo PRIVATE GDK_DISABLE_DEPRECATED GTK_DISABLE_DEPRECATED GTKMM_DISABLE_DEPRECATED)
