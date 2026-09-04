cmake_minimum_required(VERSION 3.15)

project(my_freertos_container C)

set(FREERTOS_KERNEL_PATH "$ENV{HOME}/FreeRTOS-Kernel")

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config

    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)

add_subdirectory("${FREERTOS_KERNEL_PATH}"
    FreeRTOS-Kernel
)

add_executable(${PROJECT_NAME}
    main.c
    container/container.c
    container/supervisor.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/container"
)

target_link_libraries(${PROJECT_NAME}
    freertos_kernel
    freertos_config
)
