#-----------------------<EDIT  HERE>-----------------------
set xpr_name vivado
set top_name design_1_wrapper
set core_cnt 8 
#-----------------------</EDIT HERE>----------------------- 
#----------------------------------------------------------
set origin_dir [file dirname [file dirname [info script]]]
set prj_dir ${origin_dir}/build/apu/
#----------------------------------------------------------
cd ${origin_dir}

# set the workspace
setws ${origin_dir}/build/apu/
#-----------------
# create platform 
#-----------------
platform create -name "zcu208_platform" -hw ${prj_dir}/${top_name}.xsa -proc psu_cortexa53_0 -os standalone 
platform generate
#-------------------
# create application 
#-------------------
app create -name "zcu208_init" -platform zcu208_platform
importsources -name ${app_name} -path ./sdk_src/ -linker-script
#--------------
# build project
#--------------
sysproj build -all 

exit


