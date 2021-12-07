Instructions for compiling MUI and MUI utility wrappers for ARCHER2
===================================================================

These instructions are for compiling wrappers of MUI 1.1.3 and MUI_Utilities 
on [ARCHER2](https://www.archer2.ac.uk).
wrappers of MUI 1.1.3 and MUI_Utilities are installed with the following software:
```bash
  -GNU: 10.2.0
  -python: 3.8.5
  -cmake: 3.21.3
```

Create and set the installation and build folders
---------------------------------------------
```bash
  export INSTALL_FOLDER=`pwd`
  mkdir ${INSTALL_FOLDER}/MUI

  export BUILD_DIR_MUI=${INSTALL_FOLDER}/MUI
```

Source fenics2019_eCSE_FSI.conf
---------------------------------------------
```bash
  source ${BUILD_DIR_MUI}/../FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf
```

Get MUI V1.1.3
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}
  wget https://github.com/MxUI/MUI/archive/refs/tags/1.1.3.zip
  unzip 1.1.3.zip
  mv MUI-1.1.3 V1.1.3
```

Patch Makefile of MUI Python wrapper for ARCHER2 installation
---------------------------------------------
```bash
  patch_file=${BUILD_DIR_MUI}/patched-MUI-Python-Makefile-FSI 
  cp "${patch_file}" ${BUILD_DIR_MUI}/V1.1.3/wrappers/Python/Makefile
```

Go to the main folder of MUI Python wrapper
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}/V1.1.3/wrappers/Python
```

Compile MUI Python wrapper
---------------------------------------------
In here, due to the large memory required, we need to compile MUI Python wrapper using an interactive session on ARCHER2. An example of how to set up such a session is provided here, but more information is available in ARCHER2 documentation pages. The following command is typed from a terminal session:

```bash
  salloc --nodes=1 --tasks-per-node=1 --cpus-per-task=1 --time=01:00:00 --partition=standard --qos=standard --account=budget_code
```

where budget_code should be set by the user.

Before running the tests, some environment variables should be set, and the file [fenics2019_eCSE_FSI.conf](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf) should be copied to ARCHER2 and adapted for the current installation, by changing your_own_installation_path in Line 3 (L3) to the actual installation path. It is then sourced as:

```bash
  source ${BUILD_DIR_MUI}/../FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf
```

The following command should be used to compile the Python wrapper. It will take about 13min-15min to finish.

```bash
  srun --distribution=block:block --hint=nomultithread make COMPILER=GNU package
```

After compiling, we need to run the following command to install it into the fenics2019_eCSE_FSI virenv

```bash
  make pip-install
```

After the pip installation of the Python wrapper, we now exit from the interactive session and back to the login node.

```bash
  exit
```

Compile MUI C wrapper
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}/V1.1.3/wrappers/C
  sed -i '2s/mpicc/cc/' Makefile
  sed -i '3s/mpic++/CC/' Makefile
  sed -i '22s/libmui_c_wrapper.so/libmui_c_wrapper.so mui_c_wrapper_general.o/' Makefile
  sed -i '23s/libmui_c_wrapper.a/libmui_c_wrapper.a mui_c_wrapper_general.o/' Makefile
  sed -i '25s/${CC}/#${CC}/' Makefile
  sed -i '27s/${MPI}/#${MPI}/' Makefile
  make
```

Get MUI Utilities
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}/
  wget https://github.com/MUI-Utilities/MUI_Utilities/archive/refs/heads/main.zip
  unzip main.zip
  cd ${BUILD_DIR_MUI}/MUI_Utilities-main
```

Patch Makefile of MUI Utilities Python wrapper for ARCHER2 installation
---------------------------------------------
```bash
  patch_file_util=${BUILD_DIR_MUI}/patched-MUI_Utilities-Python-Makefile-FSI
  cp "${patch_file_util}" ${BUILD_DIR_MUI}/MUI_Utilities-main/fsiCouplingLab/wrappers/Python/Makefile
```

Go to the main folder of MUI Utilities Python wrapper
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}/MUI_Utilities-main/fsiCouplingLab/wrappers/Python
```

Compile and pip install of MUI Utilities Python wrapper
---------------------------------------------
```bash
make package
make pip-install
```

Compile MUI Utilities C wrapper
---------------------------------------------
```bash
cd ${BUILD_DIR_MUI}/MUI_Utilities-main/fsiCouplingLab/wrappers/C
sed -i '4s/=/=..\/..\/..\/..\/V1.1.3/' Makefile_CAPI
sed -i '9s/-I/-I..\/..\/..\/..\/..\/FEniCS\/V2019.1.0\/eigen-3.3.9\/build\/build\/include\/eigen3/' Makefile_CAPI
make -f Makefile_CAPI
```
