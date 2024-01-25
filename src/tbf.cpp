#include "tbf.hpp"

//m has to be even since every counter should have an adjecent counter
TandemBloomFilter::TandemBloomFilter(size_t m, uint8_t k, uint8_t L_set): m{m}, k{k}, filter(((m & 1) == 0 ? m : m + 1)), f_set{murmurHash64A_Vector(k)}, g_set{murmurHash64A_Vector(k)}, h_set{murmurHash64A_Vector(k)}, L_set{L_set}{
    if(2 > L_set || (L_set & (L_set - 1)) != 0 || 32 < L_set){
        std::cerr << "L ≥ 2 should be a positive integer of the form L = 2^i" << std::endl;
        std::cerr << "L < should be (for our implementation) <= 32" << std::endl;
        exit(0);
    }
}

void TandemBloomFilter::insert(uint8_t *key, uint16_t keyLength){
    for (uint8_t i = 0; i < k; ++i) {
        auto [index, c1, c2] = getTBFvalues(i, key, keyLength);
        uint8_t& initC1 = filter.at(index);
        uint8_t& initC2 = filter.at(getAdjecentIndex(index));

        if(initC1 < L_set){
            initC1 = c1;
            if(initC2 == 0){
                initC2 = c2;
            }
        }
        else if (initC1 < 2*L_set){
            increment(initC1, c1);
            if(initC2 < L_set){
                if(c1 - L_set + 1 < L_set){
                    initC2 = c1 - L_set + 1;          
                }
                else if(initC1 - L_set + 1 < L_set){
                    initC2 = initC1 - L_set + 1;
                }
                else{
                    initC2 = 1;
                }
            }
        }
        else{
            increment(initC1, c1);
            if(1 <= initC2 && initC2 < L_set){
                initC2 = 0;
            }
        }
    }
}

bool TandemBloomFilter::lookup(uint8_t *key, uint16_t keyLength){
    for (uint8_t i = 0; i < k; ++i) {
        auto [index, c1, c2] = getTBFvalues(i, key, keyLength);
        uint8_t initC1 = filter.at(index);
        uint8_t initC2 = filter.at(getAdjecentIndex(index));
        if(initC1 < c1 || (1 <= (initC1 - c1) && (initC1 - c1) < L_set)){
            return false;
        }

        if(initC1 < 2*L_set){
            if(initC1 != c1){
                return false;
            }
            else if((1 <= initC2 && initC2 < L_set) && initC2 != c2){
                return false;
            }
        }

        else{
            if(1 <= initC2 && initC2 < L_set){
                if(initC2 == 1 && initC1 == 4*L_set - 2 && 2*L_set - 1 != c1){
                    return false;
                }
                else if(initC2 + L_set - 1 != c1 && initC1 - initC2 - L_set + 1 != c1 + initC2){
                    return false;
                }
            }
        }
    }

    return true;
}

bool TandemBloomFilter::remove(uint8_t *key, uint16_t keyLength){
    for (uint8_t i = 0; i < k; ++i) {
        auto [index, c1, c2] = getTBFvalues(i, key, keyLength);
        uint8_t& initC1 = filter.at(index);
        uint8_t& initC2 = filter.at(getAdjecentIndex(index));
        if(L_set <= initC1 && initC1 < 2*L_set){
            initC1 = 0;
        }
        else{
            decrement(initC1, c1);
        }
        if(1 <= initC2 && initC2 < L_set){
            initC2 = 0;
        }
    }
    return true;
}

size_t TandemBloomFilter::getAdjecentIndex(size_t index){
    return (index & 1) == 0 ? index + 1 : index - 1; 
}

std::tuple<size_t, uint8_t, uint8_t> TandemBloomFilter::getTBFvalues(uint8_t i, uint8_t *key, uint16_t keyLength){
    return std::make_tuple(
    f_set.at(i)(key, keyLength) % m,
    (g_set.at(i)(key, keyLength) % L_set) + L_set, //[L-set - 2*L_set-1]
    (h_set.at(i)(key, keyLength) % (L_set - 1)) + 1); //[1 - L_set-1]
}

void TandemBloomFilter::increment(uint8_t& toInc, uint8_t varIncrement){
    if(UINT8_MAX - varIncrement < toInc){
        std::cerr << "Counter overflow by couting bloom filter" << std::endl;
        return;
    }
    toInc += varIncrement;
}
void TandemBloomFilter::decrement(uint8_t& toInc, uint8_t varIncrement){
    if(toInc < varIncrement){
        std::cerr << "Counter underflow by couting bloom filter" << std::endl;
        return;
    }
    toInc -= varIncrement;
}