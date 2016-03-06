# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "C_NES_DATA_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_NES_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_NES_BASEADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_NES_HIGHADDR" -parent ${Page_0}


}

proc update_PARAM_VALUE.CLK_FREQUENCY_HZ { PARAM_VALUE.CLK_FREQUENCY_HZ } {
	# Procedure called to update CLK_FREQUENCY_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CLK_FREQUENCY_HZ { PARAM_VALUE.CLK_FREQUENCY_HZ } {
	# Procedure called to validate CLK_FREQUENCY_HZ
	return true
}

proc update_PARAM_VALUE.CNTR_WIDTH { PARAM_VALUE.CNTR_WIDTH } {
	# Procedure called to update CNTR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNTR_WIDTH { PARAM_VALUE.CNTR_WIDTH } {
	# Procedure called to validate CNTR_WIDTH
	return true
}

proc update_PARAM_VALUE.SHIFTER_CLK { PARAM_VALUE.SHIFTER_CLK } {
	# Procedure called to update SHIFTER_CLK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SHIFTER_CLK { PARAM_VALUE.SHIFTER_CLK } {
	# Procedure called to validate SHIFTER_CLK
	return true
}

proc update_PARAM_VALUE.UPDATE_100_HZ { PARAM_VALUE.UPDATE_100_HZ } {
	# Procedure called to update UPDATE_100_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.UPDATE_100_HZ { PARAM_VALUE.UPDATE_100_HZ } {
	# Procedure called to validate UPDATE_100_HZ
	return true
}

proc update_PARAM_VALUE.C_NES_DATA_WIDTH { PARAM_VALUE.C_NES_DATA_WIDTH } {
	# Procedure called to update C_NES_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_NES_DATA_WIDTH { PARAM_VALUE.C_NES_DATA_WIDTH } {
	# Procedure called to validate C_NES_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_NES_ADDR_WIDTH { PARAM_VALUE.C_NES_ADDR_WIDTH } {
	# Procedure called to update C_NES_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_NES_ADDR_WIDTH { PARAM_VALUE.C_NES_ADDR_WIDTH } {
	# Procedure called to validate C_NES_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_NES_BASEADDR { PARAM_VALUE.C_NES_BASEADDR } {
	# Procedure called to update C_NES_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_NES_BASEADDR { PARAM_VALUE.C_NES_BASEADDR } {
	# Procedure called to validate C_NES_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_NES_HIGHADDR { PARAM_VALUE.C_NES_HIGHADDR } {
	# Procedure called to update C_NES_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_NES_HIGHADDR { PARAM_VALUE.C_NES_HIGHADDR } {
	# Procedure called to validate C_NES_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_NES_DATA_WIDTH { MODELPARAM_VALUE.C_NES_DATA_WIDTH PARAM_VALUE.C_NES_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_NES_DATA_WIDTH}] ${MODELPARAM_VALUE.C_NES_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_NES_ADDR_WIDTH { MODELPARAM_VALUE.C_NES_ADDR_WIDTH PARAM_VALUE.C_NES_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_NES_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_NES_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.CLK_FREQUENCY_HZ { MODELPARAM_VALUE.CLK_FREQUENCY_HZ PARAM_VALUE.CLK_FREQUENCY_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CLK_FREQUENCY_HZ}] ${MODELPARAM_VALUE.CLK_FREQUENCY_HZ}
}

proc update_MODELPARAM_VALUE.SHIFTER_CLK { MODELPARAM_VALUE.SHIFTER_CLK PARAM_VALUE.SHIFTER_CLK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SHIFTER_CLK}] ${MODELPARAM_VALUE.SHIFTER_CLK}
}

proc update_MODELPARAM_VALUE.UPDATE_100_HZ { MODELPARAM_VALUE.UPDATE_100_HZ PARAM_VALUE.UPDATE_100_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.UPDATE_100_HZ}] ${MODELPARAM_VALUE.UPDATE_100_HZ}
}

proc update_MODELPARAM_VALUE.CNTR_WIDTH { MODELPARAM_VALUE.CNTR_WIDTH PARAM_VALUE.CNTR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CNTR_WIDTH}] ${MODELPARAM_VALUE.CNTR_WIDTH}
}

