

proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "nesip" "NUM_INSTANCES" "DEVICE_ID"  "C_NES_BASEADDR" "C_NES_HIGHADDR"
}
