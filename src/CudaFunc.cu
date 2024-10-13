#include "CudaFunc.cuh"
#include "device_launch_parameters.h"

__global__ void updatePositions(
    double *directions, double *positions, double *speeds, Vertex *vertices, int *indices, int *status, int* unit_class,
    int map_sizeX, int map_sizeY, int map_sizeZ,
    int time_slice, int unit_num
) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= unit_num) return;  // 确保线程处理的索引在有效范围内
    if (status[i] == 0) {
        vertices[i].status = 0;
        return;
    }
    double direction = sqrt(
        pow(directions[i * 3], 2) +
        pow(directions[i * 3 + 1], 2) +
        pow(directions[i * 3 + 2], 2)
    );

    if (direction == 0) return;

    double posX_add  = speeds[i] * time_slice * directions[i * 3]     / direction; // 三个方向上的增量
    double posY_add  = speeds[i] * time_slice * directions[i * 3 + 1] / direction;
    double posZ_add  = speeds[i] * time_slice * directions[i * 3 + 2] / direction;
    double next_posX = positions[i * 3]     + posX_add;
    double next_posY = positions[i * 3 + 1] + posY_add;
    double next_posZ = positions[i * 3 + 2] + posZ_add;

    if (next_posX > map_sizeX || next_posX < 0) posX_add = -posX_add; // 根据地图范围修正增量
    if (next_posY > map_sizeY || next_posY < 0) posY_add = -posY_add;
    if (next_posZ > map_sizeZ || next_posZ < 0) posZ_add = -posZ_add;

    positions[i * 3]     = positions[i * 3]     + posX_add;
    positions[i * 3 + 1] = positions[i * 3 + 1] + posY_add;
    positions[i * 3 + 2] = positions[i * 3 + 2] + posZ_add;

    float scale = 2000.0;
    glm::vec3 pos((positions[i * 3] - 5000) / scale, (positions[i * 3 + 1] - 5000) / scale, positions[i * 3 + 2]);
    glm::vec3 col(1, 0, 0);
    int camp = (i < unit_num / 2) ? 0 : 1;

    // indices[i] = i;
    vertices[i] = Vertex(pos, col, status[i], camp);
}

__global__ void calculateDistances(
    double *positions, double *target_positions, double *distances, int unit_num
) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < unit_num) { // 各个单位与目标之间的distances
        distances[i] = sqrt(pow(target_positions[i * 3]     - positions[i * 3],     2) 
                          + pow(target_positions[i * 3 + 1] - positions[i * 3 + 1], 2) 
                          + pow(target_positions[i * 3 + 2] - positions[i * 3 + 2], 2));
    } else if (i < unit_num * 2) { // 每个单位与camp0阵营的基站（索引为0）的距离
        distances[i] = sqrt(pow(positions[0] - positions[(i - unit_num) * 3],     2) 
                          + pow(positions[1] - positions[(i - unit_num) * 3 + 1], 2) 
                          + pow(positions[2] - positions[(i - unit_num) * 3 + 2], 2));
    } else if (i < unit_num * 3) { // 每个单位与camp1阵营的基站（索引为unit_num / 2）的距离
        distances[i] = sqrt(pow(positions[(unit_num / 2) * 3]     - positions[(i - unit_num * 2) * 3],     2) 
                          + pow(positions[(unit_num / 2) * 3 + 1] - positions[(i - unit_num * 2) * 3 + 1], 2) 
                          + pow(positions[(unit_num / 2) * 3 + 2] - positions[(i - unit_num * 2) * 3 + 2], 2));
    }
}

__global__ void unitUpdate(
    double *positions, double *target_positions, double *directions, double *distances,
    int *target_ids, double *init_positions, int *weapon_nums, int *status, double *attack_radius, int *unit_class,
    int unit_num
) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= unit_num ) return;
    if (status[i] == 0) return;
    if (unit_class[i] == 0) return;

    curandStateXORWOW_t rand_state;
    curand_init(0,i,0,&rand_state);

    if (target_positions[i * 3] == 0 && target_positions[i * 3 + 1] == 0 && target_positions[i * 3 + 2] == 0) { // 如果没有目标，随机前进
        directions[i * 3]           = curand_uniform(&rand_state) * 10;          // 更新运动方向
        directions[i * 3 + 1]       = curand_uniform(&rand_state) * 10;
        if (unit_class[i] == 1 || unit_class[i] == 2) directions[i * 3 + 2] = curand_uniform(&rand_state) * 10; // 飞行单位才有z轴速度
        else directions[i * 3 + 2] = 0;
    } else if (distances[i] < attack_radius[i]) { // 有目标，距离足够
        if (weapon_nums[i] > 0) weapon_nums[i] -= 1;        // 攻击
        if (target_ids[i] != 0) {   // 更新攻击目标status
            status[target_ids[i]] = 0;  
            target_ids[i] = 0;
        }
        if (unit_class[i] == 2) status[i] = 0;              // 如果是Missle，自己也更新status
        target_positions[i * 3]     = 0;                    // 更新目标位置
        target_positions[i * 3 + 1] = 0;
        target_positions[i * 3  +2] = 0;
        if (unit_class[i] == 3 || unit_class[i] == 4) { // 地面单位需要返回初始位置
            directions[i * 3]       = init_positions[i * 3]     - positions[i * 3];       // 更新运动方向
            directions[i * 3 + 1]   = init_positions[i * 3 + 1] - positions[i * 3 + 1];
            directions[i * 3 + 2]   = init_positions[i * 3 + 2] - positions[i * 3 + 2];
        } else {
            directions[i * 3]       = curand_uniform(&rand_state) * 10;          // 更新运动方向
            directions[i * 3 + 1]   = curand_uniform(&rand_state) * 10;
            directions[i * 3 + 2]   = curand_uniform(&rand_state) * 10;
        }
    } else { // 距离不够，继续前进
        directions[i * 3]           = target_positions[i * 3]     - positions[i * 3];
        directions[i * 3 + 1]       = target_positions[i * 3 + 1] - positions[i * 3 + 1];
        directions[i * 3 + 2]       = target_positions[i * 3 + 2] - positions[i * 3 + 2];
    }
}

int cudaUpdatePositions(
    double *directions, double *positions, double *speeds, Vertex *vertices, int *indices,
    double *g_directions, double *g_positions, double *g_speeds, Vertex *g_vertices, int *g_indices, int *g_status, int* g_unit_class,
    int map_sizeX, int map_sizeY, int map_sizeZ,
    int time_slice, int unit_num
) {
    cudaError_t err;

    // 定义线程块大小和网格大小
    int blockSize = 1024; // 根据你的 GPU 调整
    int gridSize = (unit_num + blockSize - 1) / blockSize;

    // 调用内核
    updatePositions<<<gridSize, blockSize>>>(g_directions, g_positions, g_speeds, g_vertices, g_indices, g_status, g_unit_class, map_sizeX, map_sizeY, map_sizeZ, time_slice, unit_num);

    // 错误检查
    cudaDeviceSynchronize(); // 等待 CUDA 设备完成所有先前排队的工作
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(positions, g_positions,  sizeof(double) * 3 * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(vertices, g_vertices,    sizeof(Vertex) * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(indices, g_indices,      sizeof(int) * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem D2H Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    return 1;
}

int cudaCalculateDistances(
    double *positions, double *target_positions, double *distances,
    double *g_positions, double *g_target_positions, double *g_distances,
    int unit_num
) {
    cudaError_t err;

    int blockSize = 1024; // 根据你的 GPU 调整
    int gridSize = (unit_num * 3 + blockSize - 1) / blockSize;

    // 调用内核
    calculateDistances<<<gridSize, blockSize>>>(g_positions, g_target_positions, g_distances, unit_num);

    // 错误检查
    cudaDeviceSynchronize(); // 等待 CUDA 设备完成所有先前排队的工作
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(distances, g_distances, sizeof(double) * 3 * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem D2H Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    return 1;
}

int cudaUnitUpdate(
    double *g_positions, double *g_target_positions, double *g_directions, double *g_distances,
    int *g_target_ids, double *g_init_positions, int *g_weapon_nums, int *g_status, double *g_attack_radius, int *g_unit_class,
    int *target_ids, double *directions, int *weapon_nums, int *status, double *target_positions,
    int unit_num
) {
    cudaError_t err;

    cudaMemcpy(g_target_ids, target_ids, sizeof(int) * unit_num, cudaMemcpyHostToDevice); // 基站的update会更新target_ids
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(g_target_positions, target_positions, sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice); // 基站的update会更新target_positions
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    int blockSize = 1024; // 根据你的 GPU 调整
    int gridSize = (unit_num + blockSize - 1) / blockSize;

    // 调用内核
    unitUpdate<<<gridSize, blockSize>>>(g_positions, g_target_positions, g_directions, g_distances, 
                                        g_target_ids, g_init_positions, g_weapon_nums, g_status, g_attack_radius, g_unit_class,
                                        unit_num);

    // 错误检查
    cudaDeviceSynchronize(); // 等待 CUDA 设备完成所有先前排队的工作
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(directions, g_directions, sizeof(double) * 3 * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem D2H Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(status, g_status, sizeof(int) * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem D2H Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(target_ids, g_target_ids, sizeof(int) * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    cudaMemcpy(target_positions, g_target_positions, sizeof(double) * 3 * unit_num, cudaMemcpyDeviceToHost);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Mem Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    return 1;
}