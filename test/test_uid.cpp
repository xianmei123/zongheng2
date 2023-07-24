#include <iostream>
#include "UniqueIDGenerator.h"

int main() {
    UniqueIDGenerator uidGenerator;

    // 生成并输出10个唯一ID
    for (int i = 0; i < 10; ++i) {
        int uid = uidGenerator.generateUID();
        std::cout << "Generated UID: " << uid << std::endl;
    }

    return 0;
}
