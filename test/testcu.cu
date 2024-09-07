#include <cuda_runtime.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

__global__ void updatePositions(
    double *directions, double *positions, double *speeds,
    double map_sizeX, double map_sizeY, double map_sizeZ,
    double time_slice
) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < 3000) { // 确保线程处理的索引在有效范围内
        double direction = sqrt(
            pow(directions[i * 3], 2) +
            pow(directions[i * 3 + 1], 2) +
            pow(directions[i * 3 + 2], 2)
        );
        double next_posX = positions[i * 3] + speeds[i] * time_slice * directions[i * 3] / direction;
        double next_posY = positions[i * 3 + 1] + speeds[i] * time_slice * directions[i * 3 + 1] / direction;
        double next_posZ = positions[i * 3 + 2] + speeds[i] * time_slice * directions[i * 3 + 2] / direction;

        if (next_posX > map_sizeX || next_posX < 0) directions[i * 3] = -directions[i * 3];
        if (next_posY > map_sizeY || next_posY < 0) directions[i * 3 + 1] = -directions[i * 3 + 1];
        if (next_posZ > map_sizeZ || next_posZ < 0) directions[i * 3 + 2] = -directions[i * 3 + 2];

        positions[i * 3] = next_posX; // 直接赋值计算后的位置
        positions[i * 3 + 1] = next_posY;
        positions[i * 3 + 2] = next_posZ;
    }
}

int main() {
    const int total_count = 3000;
    double *directions, *positions, *speeds;
    double map_sizeX = 100.0; // 假设地图尺寸为 100x100x100
    double map_sizeY = 100.0;
    double map_sizeZ = 100.0;
    double time_slice = 0.1; // 假设时间切片为 0.1

    // 分配 GPU 内存
    cudaMalloc(&directions, total_count * 3 * sizeof(double));
    cudaMalloc(&positions, total_count * 3 * sizeof(double));
    cudaMalloc(&speeds, total_count * sizeof(double));

    // 初始化随机数据
    double *h_directions = new double[total_count * 3];
    double *h_positions = new double[total_count * 3];
    double *h_speeds = new double[total_count];

    srand(time(NULL)); // 设置随机数种子
    for (int i = 0; i < total_count * 3; ++i) {
        h_directions[i] = (rand() / (double)RAND_MAX) * 2 - 1; // -1 到 1 之间的随机数
        h_positions[i] = (rand() / (double)RAND_MAX) * map_sizeX; // 0 到 map_sizeX 之间的随机数
    }
    for (int i = 0; i < total_count; ++i) {
        h_speeds[i] = (rand() / (double)RAND_MAX) * 0.5 + 0.1; // 0.1 到 0.6 之间的随机速度
    }

    // 从主机复制数据到 GPU
    cudaMemcpy(directions, h_directions, total_count * 3 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(positions, h_positions, total_count * 3 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(speeds, h_speeds, total_count * sizeof(double), cudaMemcpyHostToDevice);

    // 定义线程块大小和网格大小
    int blockSize = 256; // 根据你的 GPU 调整
    int gridSize = (total_count + blockSize - 1) / blockSize;

    // 记录开始时间
    clock_t start_time = clock();

    for (int i = 0; i < 3; ++i) { // 执行三次内核调用
        // 调用内核
        updatePositions<<<gridSize, blockSize>>>(directions, positions, speeds, map_sizeX, map_sizeY, map_sizeZ, time_slice);

        // 等待设备完成所有工作
        cudaDeviceSynchronize();

        // 检查是否有错误发生
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
            return -1;
        }
        cudaMemcpy(directions, h_directions, total_count * 3 * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(positions, h_positions, total_count * 3 * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(speeds, h_speeds, total_count * sizeof(double), cudaMemcpyHostToDevice);
    }

    // 记录结束时间
    clock_t end_time = clock();

    // 计算总时间（时钟周期数转换为秒）
    double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;

    // 打印总时间
    std::cout << "Elapsed time: " << elapsed_time << " seconds\n";
    // // 调用内核
    // updatePositions<<<gridSize, blockSize>>>(directions, positions, speeds, map_sizeX, map_sizeY, map_sizeZ, time_slice);

    // // 错误检查
    // cudaDeviceSynchronize(); // 等待 CUDA 设备完成所有先前排队的工作
    // cudaError_t err = cudaGetLastError();
    // if (err != cudaSuccess) {
    //     std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
    //     return -1;
    // }

    // 复制结果回主机内存（如果需要）
    double *h_positions_copy = new double[total_count * 3];
    cudaMemcpy(h_positions_copy, positions, total_count * 3 * sizeof(double), cudaMemcpyDeviceToHost);

    // TODO: 验证结果

    // 释放 GPU 内存
    cudaFree(directions);
    cudaFree(positions);
    cudaFree(speeds);

    // 释放主机内存
    delete[] h_directions;
    delete[] h_positions;
    delete[] h_speeds;
    delete[] h_positions_copy;

    return 0;
}