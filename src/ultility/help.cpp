#include <cmath>

namespace HELP {
constexpr size_t calculateFilterLength(size_t N) {
    return 8 * N;
    // return static_cast<size_t>(N * -1.44 * log2(0.2));
}
}  // namespace HELP
