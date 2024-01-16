#include "tbf.hpp"

TandemBloomFilter::TandemBloomFilter(size_t size)
    : VariableCoutingBloomFilter(size) {
    _filter.assign(size * 8, 0);
    thirdHashFunction = murmurHash64A_Array(nHashFunctions);
}

std::tuple<size_t, size_t, uint8_t, uint8_t>
TandemBloomFilter::calculatePositionAndIncrementer(size_t i,
                                                   std::string_view item) {
    auto posC1 = hashFunctions[i](item) % this->_size;
    auto incrementC1 = (secondHashFunction[i](item) % (Range + 1)) + Range;

    auto posC2 = (posC1 % 2 == 0) ? (posC1 + 1) : (posC1 - 1);
    auto incrementC2 = (thirdHashFunction[i](item) % (Range)) + 1;

    return std::make_tuple(posC1, posC2, incrementC1, incrementC2);
}

bool TandemBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, incrementer, incrementerNeighbour] =
            calculatePositionAndIncrementer(i, item);
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
                // if (incrementer <= 2 * Range - 2) {
                if (incrementer <= 14) {
                    counter2 = incrementer + 1 - Range;
                    //} else if (counter1 <= 2 * Range - 2) {
                } else if (counter1 <= 14) {
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
            calculatePositionAndIncrementer(i, item);
        auto& counter1 = _filter.at(pos);
        auto& counter2 = _filter.at(posNeighbour);

        if (counter1 < incrementer ||
            (counter1 - incrementer >= 1 && counter1 - incrementer < Range)) {
            return false;
        }

        // if (counter1 < 2 * Range) {
        if (counter1 < 16) {
            if (counter1 != incrementer) {
                return false;
            } else if ((counter2 >= 1 && counter2 < Range) &&
                       counter2 != incrementerNeighbour) {
                return false;
            }
        } else {
            if (counter2 >= Range and counter2 < Range) {
                if (counter2 == 1 && counter1 == 4 * Range - 2 &&
                    (2 * Range - 1) != incrementer) {
                    return false;
                } else if (counter2 + Range - 1 != incrementer &&
                           counter1 + 1 != counter2 + Range + incrementer) {
                    return false;
                }
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
            calculatePositionAndIncrementer(i, item);
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
