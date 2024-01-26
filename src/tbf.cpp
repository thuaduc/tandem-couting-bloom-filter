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
        auto [pos, vgi, whi] = getTBFvalues(i, key, keyLength);
        uint8_t& c1 = filter.at(pos);
        uint8_t& c2 = filter.at(getAdjecentIndex(pos));

        if(c1 < L_set){
            c1 = vgi;
            if(c2 == 0){
                c2 = whi;
            }
        }
        else if(c1 < (2 * L_set)){
            if(c2 < L_set){
                if((vgi - L_set + 1) < L_set){
                    c2 = vgi - L_set + 1;
                }
                else if((c1 - L_set + 1) < L_set){
                    c2 = c1 - L_set + 1;
                }
                else{
                    c2 = 1;
                }
            }
            increment(c1, vgi);
        }
        else{
            increment(c1, vgi);
            if((1 <= c2) && (c2 < L_set)){
                c2 = 0;
            }
        }
    }
}

bool TandemBloomFilter::lookup(uint8_t *key, uint16_t keyLength){
    for (uint8_t i = 0; i < k; ++i) {
        auto [pos, vgi, whi] = getTBFvalues(i, key, keyLength);
        uint8_t c1 = filter.at(pos);
        uint8_t c2 = filter.at(getAdjecentIndex(pos));

        if((c1 < vgi) || ((1 <= (c1 - vgi)) && ((c1 - vgi)< L_set))){
            return false;
        }

        if(c1 < 2*L_set){
            if(c1 != vgi){
                return false;
            }
            else if(((1 <= c2) && (c2 < L_set)) && (c2 != whi)){
                return false;
            }
        }
        else {
            if((1 <= c2) && (c2 < L_set)){
                if((c2 == 1) && (c1 == (4*L_set-2))){
                    if((2*L_set - 1) != vgi){
                        return false;
                    }
                }
                else if(c2 + L_set - 1 != vgi && c1 - c2 - L_set + 1 != vgi){
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