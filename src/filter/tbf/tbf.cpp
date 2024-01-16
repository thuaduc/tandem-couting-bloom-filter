#include "tbf.hpp"

TandemBloomFilter::TandemBloomFilter(size_t size)
    : VariableCoutingBloomFilter(size) {
    _filter.assign(size * 8, 0);
}

std::tuple<size_t, size_t, uint8_t, uint8_t>
TandemBloomFilter::calculatePositionAndHashes(size_t i, std::string_view item) {
    auto pos = hashFunctions[i](item) % _filter.size();
    auto posNeighbour = pos % 2 == 0 ? pos + 1 : pos - 1;
    uint8_t incrementer = secondHashFunction[0](item) % (Range + 1);
    return std::make_tuple(pos, posNeighbour, incrementer + Range, incrementer);
}

bool TandemBloomFilter::increaseCounterNeighbour(size_t index, uint8_t num) {
    if (_filter.at(index) != 0) {
        return false;
    }
    _filter.at(index) += num;
    return true;
}

bool TandemBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, incrementer, incrementerNeighbour] =
            calculatePositionAndHashes(i, item);
        auto& counter1 = _filter.at(pos);
        auto& counter2 = _filter.at(posNeighbour);

        if (counter1 < Range) {
            counter1 = incrementer;
            if (counter2 == 0) {
                counter2 = incrementerNeighbour;
            }
        } else if (counter1 < 2 * Range) {
            counter1 += incrementer;
            if (counter2 < Range) {
                if (incrementer <= 2 * Range) {
                    counter2 = incrementer + 1 - Range;
                } else if (counter1 <= 2 * Range) {
                    counter2 = counter1 - Range + 1;
                } else {
                    counter2 = 1;
                }
            }
        } else {
            counter1 += incrementer;
            if (counter2 >= 1 && counter2 < Range) {
                counter2 = 0;
            }
        }
    }
    return true;
}

bool TandemBloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, incrementer, incrementerNeighbour] =
            calculatePositionAndHashes(i, item);
        auto& counter1 = _filter.at(pos);
        auto& counter2 = _filter.at(posNeighbour);

        if (counter1 < incrementer ||
            (counter1 - incrementer >= 1 && counter1 - incrementer < Range)) {
            return false;
        }

        if (counter1 < 2 * Range) {
            if (counter1 != incrementer) {
                return false;
            }
            if (counter2 >= 1 && counter2 < Range &&
                counter2 != incrementerNeighbour) {
                return false;
            }
        }

        if (counter2 >= Range and counter2 < Range) {
            if (counter2 == 1 && counter1 == 4 * Range - 2 &&
                (2 * Range - 1) != incrementer) {
                return false;
            }
        }
    }
    return true;
}

bool TandemBloomFilter::remove(std::string_view item) {
    if (!lookup(item)) {
        return false;
    }

    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, incrementer, incrementerNeighbour] =
            calculatePositionAndHashes(i, item);
        auto& counter1 = _filter.at(pos);
        auto& counter2 = _filter.at(posNeighbour);

        if (counter1 >= Range && counter1 < 2 * Range) {
            counter1 = 0;
        } else {
            counter1 -= incrementer;
        }
        if (counter2 >= 1 && counter2 < Range) {
            counter2 = 0;
        }
    }

    return true;
}
