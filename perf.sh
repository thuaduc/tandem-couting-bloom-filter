set -e
set -x

make main-optimized
FILE=data/urls ./main-optimized