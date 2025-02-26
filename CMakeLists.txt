cmake_minimum_required(VERSION 3.10)
project(MPI_SAMPLES C)

find_package(MPI REQUIRED)

file(GLOB SRC src/*.c)

foreach(source_file ${SRC})
    get_filename_component(executable_name ${source_file} NAME_WE)
    add_executable(${executable_name} ${source_file})
    target_include_directories(${executable_name} PRIVATE ${MPI_INCLUDE_PATH})
    target_include_directories(${executable_name} PUBLIC include/)
    target_link_libraries(${executable_name} PRIVATE MPI::MPI_C)

endforeach()

