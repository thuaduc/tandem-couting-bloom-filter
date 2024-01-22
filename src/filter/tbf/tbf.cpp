#include "tbf.hpp"


TandemBloomFilter::TandemBloomFilter(size_t size, uint8_t nHashFunctions, uint8_t L_set): filter(size), f_set{nHashFunctions}, g_set{nHashFunctions}, h_set{nHashFunctions}{
    if(!(2 <= L_set && (L_set & (L_set - 1)) == 0)){
        std::cerr << "L ≥ 2 should be a positive integer of the form L = 2^i" << std::endl;
        exit(0);
    }
    this->L_set = L_set;
}

void TandemBloomFilter::add(uint8_t *key, uint16_t keyLength){
    for (size_t i = 0; i < f_set.size(); ++i) {
        auto [index, c1, c2] = getTandemValues(i, key, keyLength);
        uint8_t& initC1 = filter.at(index);
        uint8_t& initC2 = filter.at(getAdjecentIndex(index));

        if(initC1 < L_set){
            initC1 = c1;
            if(initC2 == 0){
                initC2 = c2;
            }
        }
        else if (c1 < 2*L_set){
            initC1 += c1;
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
            initC1 += c1;
            if(1 <= initC2 && initC2 < L_set){
                initC2 = 0;
            }
        }
    }
}

bool TandemBloomFilter::lookup(uint8_t *key, uint16_t keyLength){
    for (size_t i = 0; i < f_set.size(); ++i) {
        auto [index, c1, c2] = getTandemValues(i, key, keyLength);
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
                else if(initC2 + L_set - 1 != c1 && initC1 - initC2 - L_set + 1 != c1){
                    return false;
                }
            }
        }

    }

    return true;
}

bool TandemBloomFilter::remove(uint8_t *key, uint16_t keyLength){
    for (size_t i = 0; i < f_set.size(); ++i) {
        auto [index, c1, c2] = getTandemValues(i, key, keyLength);
        uint8_t& initC1 = filter.at(index);
        uint8_t& initC2 = filter.at(getAdjecentIndex(index));
        if(L_set <= initC1 && initC1 < 2*L_set){
            initC1 = 0;
        }
        else{
            initC1 -= c1;
        }
        if(1 <= initC2 && initC2 < L_set){
            initC2 = 0;
        }
    }
}

uint8_t TandemBloomFilter::getAdjecentIndex(size_t index){
    return index & 1 == 0 ? index + 1 : index - 1; 
}

std::tuple<size_t, uint8_t, uint8_t> TandemBloomFilter::getTandemValues(uint8_t i, uint8_t *key, uint16_t keyLength){
    return std::make_tuple(
    f_set.at(i)(key, keyLength) % filter.size(),
    (g_set.at(i)(key, keyLength) % (L_set + 1)) + L_set, //[L - 2L-1]
    (h_set.at(i)(key, keyLength) % (L_set - 1)) + 1); //[1 - L-1]
}

/*
    f_set.at(i)(key, keyLength) % filter.size(),
    (g_set.at(i)(key, keyLength) % (L_set + 1)) + L_set, //[L - 2L-1]
    (h_set.at(i)(key, keyLength) % (L_set - 1)) + 1); //[1 - L-1]
    */



void TandemBloomFilter::increment(size_t index, uint8_t varIncrement){
    if(UINT8_MAX - varIncrement < filter.at(index)){
        std::cerr << "Counter overflow by couting bloom filter" << std::endl;
        return;
    }
    filter.at(index) += varIncrement;
}
void TandemBloomFilter::decrement(size_t index, uint8_t varIncrement){
    if(filter.at(index) < varIncrement){
        std::cerr << "Counter underflow by couting bloom filter" << std::endl;
        return;
    }
    filter.at(index) -= varIncrement;
}