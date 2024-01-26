#include <cmath>

namespace HELP {
size_t calculateFilterLength(size_t N) {
    return static_cast<size_t>(N * -1.44 * log2(0.2));
}
}  // namespace HELP
