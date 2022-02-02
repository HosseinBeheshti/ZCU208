#-----------------------<EDIT  HERE>-----------------------
set project_folder build/apu/
set xpr_name vivado
set top_name design_1_wrapper
set core_cnt 8 
set platform_export_path build/pl
#-----------------------</EDIT HERE>----------------------- 
#----------------------------------------------------------
set origin_dir [file dirname [file dirname [info script]]]
#----------------------------------------------------------
cd ${origin_dir}

# set the workspace
setws ./${project_folder}
#--------------------------
# CREATE FSBL FROM TEMPLATE
#--------------------------
app create -name FSBL -template {Zynq MP FSBL} -proc psu_cortexa53_0 -hw ./${top_name}.xsa  -os standalone
app config -name FSBL build-config release
platform generate ${top_name}
#---------------------------
# CREATE PMUFW FROM TEMPLATE
#---------------------------
app create -name PMUFW -template {ZynqMP PMU Firmware} -proc psu_pmu_0 -hw ./${top_name}.xsa -os standalone
app config -name PMUFW build-config release
platform generate design_1_wrapper_1
#--------------------------------------
# CREATE A53 DDR TEST APP FROM TEMPLATE
#--------------------------------------
app create -name A53_DDR_Test -template {Zynq MP DRAM tests} -proc psu_cortexa53_0 -hw ./${top_name}.xsa -os standalone
app config -name A53_DDR_Test build-config release
platform generate design_1_wrapper_2
#-----------------------------------------------------
# CREATE R5-0 BARE METAL HELLO WORLD APP FROM TEMPLATE
#-----------------------------------------------------
app create -name R50_app -template {Hello World} -proc psu_cortexr5_0 -hw ./${top_name}.xsa -os standalone
app config -name R50_app build-config release
platform generate design_1_wrapper_3
#---------------------------------------------------
# CREATE R5-1 FREERTOS HELLO WORLD APP FROM TEMPLATE
#---------------------------------------------------
app create -name R51_app -template {FreeRTOS Hello World} -proc psu_cortexr5_1 -hw ./${top_name}.xsa -os freertos10_xilinx
app config -name R51_app build-config release
platform generate design_1_wrapper_4

exit


