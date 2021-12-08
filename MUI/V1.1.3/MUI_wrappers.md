Instructions for compiling MUI and MUI utility wrappers for ARCHER2
===================================================================

These instructions are for compiling wrappers of MUI 1.1.3 and MUI_Utilities 
on [ARCHER2](https://www.archer2.ac.uk).
Wrappers of MUI 1.1.3 and MUI_Utilities are installed with the following utilities:
```bash
  -GNU: 10.2.0
  -python: 3.8.5
  -cmake: 3.21.3
```

Create and set the installation and build folders
---------------------------------------------
For simplicity, MUI is installed in the same INSTALL_FOLDER as FEniCS. Some paths showed below assume this.

```bash
  export INSTALL_FOLDER=`pwd`
  mkdir ${INSTALL_FOLDER}/MUI

  export BUILD_DIR_MUI=${INSTALL_FOLDER}/MUI
```

Source fenics2019_eCSE_FSI.conf
---------------------------------------------
If not sourced yet, this file [fenics2019_eCSE_FSI.conf](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf) needs to be sourced to set up some environment variables.

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
Some extra files are required to continue the installation, as for instance [patched-MUI-Python-Makefile-FSI](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/MUI/patched-MUI-Python-Makefile-FSI), which needs to be copied to ARCHER2 in the ${BUILD_DIR_MUI} directory.

```bash
  export patch_file=${BUILD_DIR_MUI}/patched-MUI-Python-Makefile-FSI 
  cp "${patch_file}" ${BUILD_DIR_MUI}/V1.1.3/wrappers/Python/Makefile
```

Compile MUI Python wrapper
---------------------------------------------
Because of the large memory required, the MUI Python wrapper needs to be compiled in an interactive session of ARCHER2. An example of how to set up such a session is provided here, but more information is available in ARCHER2 documentation pages. The following command is typed from a terminal session:

```bash
  salloc --nodes=1 --tasks-per-node=1 --cpus-per-task=1 --time=01:00:00 --partition=standard --qos=standard --account=budget_code
  cd ${BUILD_DIR_MUI}/V1.1.3/wrappers/Python
```

where budget_code should be set by the user.

Before running the tests, some environment variables should be set, and the file [fenics2019_eCSE_FSI.conf](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf), if not already available on the machine, should be copied to ARCHER2 and adapted for the current installation, by changing your_own_installation_path in Line 3 (L3) to the actual installation path. It is then sourced as:

```bash
  source ${BUILD_DIR_MUI}/../FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf
```

The following command should be used to compile the Python wrapper.

```bash
  srun --distribution=block:block --hint=nomultithread make COMPILER=GNU package
```

After compiling, the following command is run to install it into the fenics2019_eCSE_FSI virtualenv.

```bash
  make pip-install
```

As the pip installation of the Python wrapper is done, the next steps are to exit from the interactive session and go back to the login node.

```bash
  exit
```

Compile MUI C wrapper
---------------------------------------------
The C wrapper is now built as:

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
  cd ${BUILD_DIR_MUI}
  wget https://github.com/MUI-Utilities/MUI_Utilities/archive/refs/heads/main.zip
  unzip main.zip
```

Patch Makefile of MUI Utilities Python wrapper for ARCHER2 installation
---------------------------------------------
Another patch is required for the utilities, and the [patched-MUI_Utilities-Python-Makefile-FSI](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/MUI/patched-MUI_Utilities-Python-Makefile-FSI) patch needs to be copied on ARCHER2, in the ${BUILD_DIR_MUI} directory.

```bash
  export patch_file_util=${BUILD_DIR_MUI}/patched-MUI_Utilities-Python-Makefile-FSI
  cp "${patch_file_util}" ${BUILD_DIR_MUI}/MUI_Utilities-main/fsiCouplingLab/wrappers/Python/Makefile
```

Compile and pip install of MUI Utilities Python wrapper
---------------------------------------------
```bash
  cd ${BUILD_DIR_MUI}/MUI_Utilities-main/fsiCouplingLab/wrappers/Python
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

It is possible to check the installation by running some unit tests to be found [here](https://github.com/MxUI/MUI-demo/tree/master/mui4py). The would need to be run on an interactive node of ARCHER2.
