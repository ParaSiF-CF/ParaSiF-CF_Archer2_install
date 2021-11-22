#!/bin/bash
# Exit on error
set -e

#salloc --nodes=1 --tasks-per-node=1 --cpus-per-task=1 --time=02:0:00 --partition=standard --qos=standard --account=c01-eng

INSTALL_FOLDER=/work/c01/c01/wendiliu/app_test
cd ${INSTALL_FOLDER}/MUI

source ${INSTALL_FOLDER}/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf

module list

export INSTALL_DIR=${INSTALL_FOLDER}/MUI

wget https://github.com/MxUI/MUI/archive/refs/tags/1.1.3.zip
unzip 1.1.3.zip
rm 1.1.3.zip

patch_file=$INSTALL_DIR/patched-MUI-Python-Makefile-FSI 
cp "${patch_file}" $INSTALL_DIR/MUI-1.1.3/wrappers/Python/Makefile

cd $INSTALL_DIR/MUI-1.1.3/wrappers/Python
srun --distribution=block:block --hint=nomultithread make COMPILER=GNU package
#make COMPILER=GCC mui4py_mod
#make COMPILER=GCC package
exit

source /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf
module list
cd $INSTALL_DIR/MUI-1.1.3/wrappers/Python
make pip-install


cd $INSTALL_DIR/MUI-1.1.3/wrappers/C
sed -i '2s/mpicc/cc/' Makefile
sed -i '3s/mpic++/CC/' Makefile
sed -i '22s/libmui_c_wrapper.so/libmui_c_wrapper.so mui_c_wrapper_general.o/' Makefile
sed -i '23s/libmui_c_wrapper.a/libmui_c_wrapper.a mui_c_wrapper_general.o/' Makefile
sed -i '25s/${CC}/#${CC}/' Makefile
sed -i '27s/${MPI}/#${MPI}/' Makefile
make

cd $INSTALL_DIR/
wget https://github.com/MUI-Utilities/MUI_Utilities/archive/refs/heads/main.zip
unzip main.zip
rm main.zip
cd $INSTALL_DIR/MUI_Utilities-main

patch_file_util=$INSTALL_DIR/patched-MUI_Utilities-Python-Makefile-FSI
cp "${patch_file_util}" $INSTALL_DIR/MUI_Utilities-main/fsiCouplingLab/wrappers/Python/Makefile
cd $INSTALL_DIR/MUI_Utilities-main/fsiCouplingLab/wrappers/Python
make package
make pip-install

cd $INSTALL_DIR/MUI_Utilities-main/fsiCouplingLab/wrappers/C
sed -i '4s/=/=..\/..\/..\/..\/MUI-1.1.3/' Makefile_CAPI
sed -i '9s/-I/-I..\/..\/..\/..\/..\/FEniCS\/V2019.1.0\/eigen-3.3.9\/build\/build\/include\/eigen3/' Makefile_CAPI
make -f Makefile_CAPI