#!/bin/bash
# Exit on error
set -e

#salloc --nodes=1 --tasks-per-node=1 --cpus-per-task=1 --time=02:0:00 --partition=standard --qos=standard --account=c01-eng

source /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf

module list

export INSTALL_DIR=/work/c01/c01/wendiliu/app/MUI

# cp source/v1.0_dev.tar.gz ./
# tar -xzf v1.0_dev.tar.gz
# rm v1.0_dev.tar.gz

wget https://github.com/MxUI/MUI/archive/refs/tags/1.1.tar.gz
tar -xzf 1.1.tar.gz

patch_file=$INSTALL_DIR/patched-MUI-Python-Makefile-FSI 
cp "${patch_file}" $INSTALL_DIR/MUI-1.1/wrappers/Python/Makefile

cd $INSTALL_DIR/MUI-1.1/wrappers/Python
srun --distribution=block:block --hint=nomultithread make COMPILER=GCC package
#make COMPILER=GCC mui4py_mod
#make COMPILER=GCC package
exit

source /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf
module list
export INSTALL_DIR=/work/c01/c01/wendiliu/app/MUI
cd $INSTALL_DIR/v1.0_dev/wrappers/Python
make pip-install