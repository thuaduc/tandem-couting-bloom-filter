//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <random>
#include <string>

#include "bf.hpp"
#include "cbf.hpp"
#include "vbf.hpp"
#include "tbf.hpp"
#include <cassert>
#include <chrono>
#include <ctime>
#include <time.h>
//#include "doctest/doctest.h"

static inline double currentTime(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void printNumber(uint8_t *key, uint16_t keyLength){
    for(uint16_t i = 0; i < keyLength; ++i){
         printf("%d", key[i]);
    }
    printf("\n");
}
int main(int argc, char const *argv[])
{
    std::vector<std::vector<uint8_t>> data;
    std::vector<uint64_t> v;
    uint64_t n = 10000000;
    for (uint64_t i = 0; i < n; i+=2)
        v.push_back(i);
    for (auto x: v) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        data.emplace_back(u.bytes, u.bytes + 4);
    }

    std::vector<std::vector<uint8_t>> data2;
    std::vector<uint64_t> v2;

    for (uint64_t i = 1; i < n; i+=2)
        v2.push_back(i);
    for (auto x: v2) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        data2.emplace_back(u.bytes, u.bytes + 4);
    }

    TandemBloomFilter vbf{80000000, 5, 4};
    double start = currentTime();
    vbf.insert(data.at(0).data(), data.at(0).size());
    vbf.insert(data.at(0).data(), data.at(0).size());     
    assert(vbf.lookup(data.at(0).data(), data.at(0).size()));       
    for(uint64_t i = 0; i < data.size(); i++){
        vbf.insert(data.at(i).data(), data.at(i).size());
    }

    for(uint64_t i = 0; i < data.size(); i++){
        assert(vbf.lookup(data.at(i).data(), data.at(i).size()));
        
    }  
    size_t wrong = 0;
    for(uint64_t i = 0; i < data2.size(); i++){
        if(vbf.lookup(data2.at(i).data(), data2.at(i).size())){
            wrong++;
        }
    }  
    double end = currentTime();
    double time = end - start;

    printf("%.7f\n", 100.f * wrong/data.size());
    printf("%lu\n", data.size());
    printf("Total runtime: %.4f seconds\n", time);
    // size_t index = 16;

    // std::vector<uint8_t> vec{0b11000111, 0b01000010};

    // size_t pos = index >> 3;
    // uint8_t mask = 0b10000000 >> (index - (pos << 3));
    // auto y = vec.at(pos) & mask;
    // std::cout << y << std::endl;
    return 0;
}
/*
TEST_CASE("Test basic insert, remove, lookup - standart bloom filter") {

    // printNumber(data.at(1).data(), data.at(1).size());
    // for(uint64_t i = 0; i < 5; i++){
    //     auto x = hashFunctions.at(i)(data.at(1).data(), data.at(1).size());
    //     printf("%lu\n", x);
    // }
    // for(uint64_t i = 0; i < n; i++){
    //     printNumber(data.at(i).data(), data.at(i).size());
    //     //bf.insert(data.at(i).data(), data.at(i).size());
    // }

}


TEST_CASE("Test basic insert, remove, lookup - couting bloom filter") {

}

TEST_CASE("Test basic insert, remove, lookup - variable couting bloom filter") {

}

TEST_CASE("Test basic insert, remove, lookup - tandem couting bloom filter") {

}
*/