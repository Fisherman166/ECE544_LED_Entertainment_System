
`timescale 1 ns / 1 ps

	module nesip_v1_0 #
	(
		// Users to add parameters here
          //Parameters
          parameter integer    CLK_FREQUENCY_HZ        = 100000000, 
          parameter integer    SHIFTER_CLK               = 200000,
          parameter integer    UPDATE_100_HZ           = 100,
          parameter integer     CNTR_WIDTH                   = 32,
        
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface NES
		parameter integer C_NES_DATA_WIDTH	= 32,
		parameter integer C_NES_ADDR_WIDTH	= 4
	)
	(
		// Users to add ports here
          //Inputs from board
          input clk,
          input reset,     
          //Input from NES
          input nes_data,
          output  nes_latch,
          output  nes_pulse,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface NES
		input wire  nes_aclk,
		input wire  nes_aresetn,
		input wire [C_NES_ADDR_WIDTH-1 : 0] nes_awaddr,
		input wire [2 : 0] nes_awprot,
		input wire  nes_awvalid,
		output wire  nes_awready,
		input wire [C_NES_DATA_WIDTH-1 : 0] nes_wdata,
		input wire [(C_NES_DATA_WIDTH/8)-1 : 0] nes_wstrb,
		input wire  nes_wvalid,
		output wire  nes_wready,
		output wire [1 : 0] nes_bresp,
		output wire  nes_bvalid,
		input wire  nes_bready,
		input wire [C_NES_ADDR_WIDTH-1 : 0] nes_araddr,
		input wire [2 : 0] nes_arprot,
		input wire  nes_arvalid,
		output wire  nes_arready,
		output wire [C_NES_DATA_WIDTH-1 : 0] nes_rdata,
		output wire [1 : 0] nes_rresp,
		output wire  nes_rvalid,
		input wire  nes_rready
	);
// Instantiation of Axi Bus Interface NES
	nesip_v1_0_NES # ( 
	   //controller params
        .CLK_FREQUENCY_HZ(CLK_FREQUENCY_HZ), 
        .SHIFTER_CLK(SHIFTER_CLK),
        .UPDATE_100_HZ(UPDATE_100_HZ),
        .CNTR_WIDTH(CNTR_WIDTH),
		
		.C_S_AXI_DATA_WIDTH(C_NES_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_NES_ADDR_WIDTH)
	) nesip_v1_0_NES_inst (
		.S_AXI_ACLK(nes_aclk),
		.S_AXI_ARESETN(nes_aresetn),
		.S_AXI_AWADDR(nes_awaddr),
		.S_AXI_AWPROT(nes_awprot),
		.S_AXI_AWVALID(nes_awvalid),
		.S_AXI_AWREADY(nes_awready),
		.S_AXI_WDATA(nes_wdata),
		.S_AXI_WSTRB(nes_wstrb),
		.S_AXI_WVALID(nes_wvalid),
		.S_AXI_WREADY(nes_wready),
		.S_AXI_BRESP(nes_bresp),
		.S_AXI_BVALID(nes_bvalid),
		.S_AXI_BREADY(nes_bready),
		.S_AXI_ARADDR(nes_araddr),
		.S_AXI_ARPROT(nes_arprot),
		.S_AXI_ARVALID(nes_arvalid),
		.S_AXI_ARREADY(nes_arready),
		.S_AXI_RDATA(nes_rdata),
		.S_AXI_RRESP(nes_rresp),
		.S_AXI_RVALID(nes_rvalid),
		.S_AXI_RREADY(nes_rready),
		
          //Inputs from board
          .clk(clk),
          .reset(reset),
          //Input from NES
          .nes_data(new_data),
          .nes_latch(nes_latch),
          .nes_pulse(nes_pulse)
		
	);

	// Add user logic here

	// User logic ends

	endmodule
