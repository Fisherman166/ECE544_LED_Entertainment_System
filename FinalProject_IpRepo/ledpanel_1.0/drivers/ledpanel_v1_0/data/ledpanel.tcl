

proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "ledpanel" "NUM_INSTANCES" "DEVICE_ID"  "C_ledpanel_BASEADDR" "C_ledpanel_HIGHADDR"
}
