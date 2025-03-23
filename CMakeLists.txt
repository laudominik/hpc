cmake_minimum_required(VERSION 3.10)

option(BUILD_CUDA "Build project with CUDA libraries" OFF)

if(BUILD_CUDA)
    project(HPC_SAMPLES C CUDA)
    file(GLOB SRC src/*.c src/*.cu)
else()
    project(HPC_SAMPLES C)
    file(GLOB SRC src/*.c)
endif()


find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

foreach(source_file ${SRC})
    get_filename_component(executable_name ${source_file} NAME_WE)
    add_executable(${executable_name} ${source_file})
    target_include_directories(${executable_name} PRIVATE ${MPI_INCLUDE_PATH})
    target_include_directories(${executable_name} PUBLIC include/)
    target_link_libraries(${executable_name} PRIVATE MPI::MPI_C OpenMP::OpenMP_C m)
endforeach()

