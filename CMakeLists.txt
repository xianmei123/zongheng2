cmake_minimum_required(VERSION 3.27)
project(MyProject)

# 设置 CUDA 架构
set(CMAKE_CUDA_ARCHITECTURES 70)

# 设置渲染
set(RENDER_LIBS renderLib)
add_subdirectory(render)




# 启用 CUDA 语言
#set(CUDA_TOOLKIT_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.4/bin")
#find_package(CUDAToolkit REQUIRED)
enable_language(CUDA)

# 查找 CUDA Toolkit
find_package(CUDAToolkit REQUIRED)

# 设置 C++ 和 CUDA 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CUDA_STANDARD 11)
# 收集头文件和源文件
file(GLOB_RECURSE HDRS "include/*.h" "include/*.cuh")
file(GLOB_RECURSE CPP_SRCS "src/*.cpp")
file(GLOB_RECURSE CUDA_SRCS "src/*.cu")

# 将头文件和源文件列表合并
list(APPEND SRCS ${HDRS} ${CPP_SRCS} ${CUDA_SRCS})

# 添加可执行文件
add_executable(main ${SRCS})

# 设置包含目录
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${CUDAToolkit_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/render/include
)

# 打印变量
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CUDAToolkit_INCLUDE_DIRS: ${CUDAToolkit_INCLUDE_DIRS}")
message(STATUS "Source Files: ${SRCS}")

# 设置 CUDA 属性
# 注意：这里不需要再次设置，除非你想覆盖之前的设置
# set_property(TARGET main PROPERTY CUDA_ARCHITECTURES "sm_35;sm_50;sm_60")

# 链接 CUDA Toolkit
target_link_libraries(main PRIVATE
    CUDA::cudart
    ${RENDER_LIBS}
   
)

# 如果需要链接其他 CUDA 库，可以直接使用 target_link_libraries
# target_link_libraries(main PRIVATE
#     CUDA::cublas # 例如，链接 cuBLAS 库
#     CUDA::cufft  # 链接 cuFFT 库
# )

# #为了不使用visual studio的cl.exe添加的配置文件
# set(CMAKE_CUDA_HOST_COMPILER "C:/msys64/mingw64/bin/g++.exe")
# set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
# set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
# set(CMAKE_RC_COMPILER "C:/msys64/mingw64/bin/windres.exe")
