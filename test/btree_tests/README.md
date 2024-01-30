# Tandem-Counting-Bloom-Filter

## Introduction

This is a project from Thua-Duc Nguyen and Cong-Thanh Dang in pactical course data structture engineering at TUM.

## Resouce

- You can find the [original paper](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8880610) here.

## Start

```
mkdir src/build
cmake ..
make
```

## Compile test

Make sure that you have doctest installed in test folder.

```bash
# in test folder
chmod +x test.sh;
./test.sh
```

## TODOs:

- intergrate filter in btree
- test if filter help improve look up, insert, scan, ...
- 2nd hash function
- implement cbf, vbf, tbf
