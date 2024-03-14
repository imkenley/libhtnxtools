// 提取一个阶元的所有子阶元，并输出完整的谱系信息
#include <fstream>
#include <iostream>

#include "libhtnxtools.h"


int main() {
    std::cout << "开始读取所有阶元的名称..." << std::endl;
    Names names("data/taxdump/names.dmp");

    std::cout << "开始读取阶元信息以及各阶元间的从属信息..." << std::endl;
    Nodes nodes("data/taxdump/nodes.dmp", names);

    std::cout << "获取哺乳动物的所有子阶元并输出到文件..." << std::endl;
    std::ofstream os("output/mammalia.txt");
    WriteTree(nodes.Get("40674"), os); // 40674为所有哺乳动物的最近公共祖先
}