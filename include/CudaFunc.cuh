#ifndef CUDA_FUNC_H
#define CUDA_FUNC_H
#include <cuda_runtime.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
#include <curand_kernel.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position; // 位置
    glm::vec3 color;
    
    int status; // 状态
    int camps;  // 阵营
    int padding;

    // 默认构造函数
    __host__ __device__ Vertex() 
        : position(0.0f), color(0.0f), status(0), camps(0), padding(0) {}

    // 构造函数，加上 __host__ __device__ 修饰符，并且接受status和camps参数
    __host__ __device__ Vertex(const glm::vec3& position, const glm::vec3& color, int status = 0, int camps = 0)
        : position(position), color(color), status(status), camps(camps), padding(0) {}
};

struct DataChunk {
    Vertex* vertices;
    int* indices;
    int unit_count;
};

extern "C" int cudaUpdatePositions(
    double *directions, double *positions, double *speeds, Vertex *vertices, int *indices, 
    double *g_directions, double *g_positions, double *g_speeds, Vertex *g_vertices, int *g_indices, int *g_status, int* g_unit_class,
    int map_sizeX, int map_sizeY, int map_sizeZ,
    int time_slice, int unit_num
);

extern "C" int cudaCalculateDistances(
    double *positions, double *target_positions, double *distances,
    double *g_positions, double *g_target_positions, double *g_distances,
    int unit_num
);

extern "C" int cudaUnitUpdate(
    double *g_positions, double *g_target_positions, double *g_directions, double *g_distances,
    int *g_target_ids, double *g_init_positions, int *g_weapon_nums, int *g_status, double *g_attack_radius, int *g_unit_class,
    int *target_ids, double *directions, int *weapon_nums, int *status, double *target_positions,
    int unit_num
);
 
#endif