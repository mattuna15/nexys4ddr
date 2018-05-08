# This is a tcl command script for the Vivado tool chain
read_vhdl comp.vhd
read_xdc comp.xdc
synth_design -top comp -part xc7a100tcsg324-1
place_design
route_design
write_bitstream -force comp.bit
exit