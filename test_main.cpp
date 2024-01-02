#include <algorithm>
#include <csignal>
#include <fstream>
#include <string>
#include <vector>

#include "PerfEvent.hpp"
#include "src/btree/printTree.hpp"
#include "tester.hpp"

using namespace std;

void runTest(vector<vector<uint8_t>> &keys, PerfEvent &perf) {
    Tester t{};

    std::vector<uint8_t> emptyKey{};
    uint64_t count = keys.size();

    {
        PerfEventBlock peb(perf, count, {"insert"});
        for (uint64_t i = 0; i < count; ++i) {
            t.insert(keys[i], keys[i]);
        }
    }
    {
        PerfEventBlock peb(perf, count, {"lookup"});
        for (uint64_t i = 0; i < count; ++i) {
            t.lookup(keys[i]);
        }
    }
}

std::vector<uint8_t> stringToVector(const std::string &str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

int main() {
    srand(42);
    PerfEvent perf;
    if (getenv("FILE")) {
        vector<vector<uint8_t>> data;
        ifstream in(getenv("FILE"));
        string line;
        while (getline(in, line)) {
            data.push_back(stringToVector(line));
        }
        runTest(data, perf);
    }
    return 0;
}
