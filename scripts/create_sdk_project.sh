#!/bin/bash
shopt -s extglob
# directory settings
ORG_DIR=$(pwd) 
PROJ_DIR=$ORG_DIR/build/ps/

echo "Started at" >> $ORG_DIR/create_sdk_project_runtime.txt
date >> $ORG_DIR/create_sdk_project_runtime.txt
if [ -d "$PROJ_DIR" ]; then
     printf "Removing previous files ...\n"
     rm -rf $PROJ_DIR;
fi
mkdir -p ./build/ps/
cp $ORG_DIR/build/pl/design_1_wrapper.xsa $ORG_DIR/build/ps/design_1_wrapper.xsa

xsct $ORG_DIR/tcl/create_sdk_project.tcl

echo "Finished at" >> $ORG_DIR/create_sdk_project_runtime.txt
date >> $ORG_DIR/create_sdk_project_runtime.txt
