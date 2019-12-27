# 
# Synthesis run script generated by Vivado
# 

set TIME_start [clock seconds] 
proc create_report { reportName command } {
  set status "."
  append status $reportName ".fail"
  if { [file exists $status] } {
    eval file delete [glob $status]
  }
  send_msg_id runtcl-4 info "Executing : $command"
  set retval [eval catch { $command } msg]
  if { $retval != 0 } {
    set fp [open $status w]
    close $fp
    send_msg_id runtcl-5 warning "$msg"
  }
}
create_project -in_memory -part xc7z020clg400-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.cache/wt [current_project]
set_property parent.project_path E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.xpr [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_FIFO XPM_MEMORY} [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_repo_paths e:/WorkSpace/ip [current_project]
set_property ip_output_repo e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
add_files E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/BIT10TABLE.coe
read_verilog -library xil_defaultlib {
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/imports/new/SC30_v1_0_S00_AXI.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/imports/new/Trgger.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/imports/new/axis_switch.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/bayer2rgb.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/cmos_sampling.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/data_conv_model.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/data_put_together.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/imports/hdl/design_1_wrapper.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/isp_model_axis.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/maxtri3x3_shift.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/new/video_caputure.v
  E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/imports/new/top.v
}
add_files E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/design_1.bd
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0/design_1_processing_system7_0_0.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_rst_processing_system7_0_100M_0/design_1_rst_processing_system7_0_100M_0_board.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_rst_processing_system7_0_100M_0/design_1_rst_processing_system7_0_100M_0.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_rst_processing_system7_0_100M_0/design_1_rst_processing_system7_0_100M_0_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0_board.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_1/design_1_axi_vdma_0_1.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_1/design_1_axi_vdma_0_1_clocks.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_1/design_1_axi_vdma_0_1_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_1_0/design_1_axi_vdma_1_0.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_1_0/design_1_axi_vdma_1_0_clocks.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_1_0/design_1_axi_vdma_1_0_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_2/design_1_axi_vdma_0_2.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_2/design_1_axi_vdma_0_2_clocks.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_vdma_0_2/design_1_axi_vdma_0_2_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_gpio_0_1/design_1_axi_gpio_0_1_board.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_gpio_0_1/design_1_axi_gpio_0_1_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_axi_gpio_0_1/design_1_axi_gpio_0_1.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_xbar_0/design_1_xbar_0_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_xbar_1/design_1_xbar_1_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_xbar_2/design_1_xbar_2_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_auto_pc_2/design_1_auto_pc_2_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_auto_pc_1/design_1_auto_pc_1_ooc.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/ip/design_1_auto_pc_0/design_1_auto_pc_0_ooc.xdc]
set_property used_in_implementation false [get_files -all E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/bd/design_1/design_1_ooc.xdc]

read_ip -quiet E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_generator_0/fifo_generator_0.xci
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_generator_0/fifo_generator_0.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_generator_0/fifo_generator_0_clocks.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_generator_0/fifo_generator_0_ooc.xdc]

read_ip -quiet E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_maxtrix/fifo_maxtrix.xci
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_maxtrix/fifo_maxtrix.xdc]
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/fifo_maxtrix/fifo_maxtrix_ooc.xdc]

read_ip -quiet E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/axis_dwidth_converter_0/axis_dwidth_converter_0.xci
set_property used_in_implementation false [get_files -all e:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/sources_1/ip/axis_dwidth_converter_0/axis_dwidth_converter_0_ooc.xdc]

# Mark all dcp files as not used in implementation to prevent them from being
# stitched into the results of this synthesis run. Any black boxes in the
# design are intentionally left as such for best results. Dcp files will be
# stitched into the design at a later time, either when this synthesis run is
# opened, or when it is stitched into a dependent implementation run.
foreach dcp [get_files -quiet -all -filter file_type=="Design\ Checkpoint"] {
  set_property used_in_implementation false $dcp
}
read_xdc E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/constrs_1/new/system.xdc
set_property used_in_implementation false [get_files E:/WorkSpace/project/FPGA/backup_prj_sc2238_v2.0_emmc_RGB/prj_sc2238_RGB/prj_sc2238_RGB.srcs/constrs_1/new/system.xdc]

read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]
set_param ips.enableIPCacheLiteLoad 0
close [open __synthesis_is_running__ w]

synth_design -top top -part xc7z020clg400-1


# disable binary constraint mode for synth run checkpoints
set_param constraints.enableBinaryConstraints false
write_checkpoint -force -noxdef top.dcp
create_report "synth_1_synth_report_utilization_0" "report_utilization -file top_utilization_synth.rpt -pb top_utilization_synth.pb"
file delete __synthesis_is_running__
close [open __synthesis_is_complete__ w]