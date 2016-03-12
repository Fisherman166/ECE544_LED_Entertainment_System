
`timescale 1 ns / 1 ps

	module ledpanel_v1_0 #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface ledpanel
		parameter integer C_ledpanel_DATA_WIDTH	= 32,
		parameter integer C_ledpanel_ADDR_WIDTH	= 5
	)
	(
		// Users to add ports here 
        //Outputs to panel
        output   [2:0]  led_rgb1,
        output   [2:0]  led_rgb2,
        output   [2:0]  led_abc,
        output          led_clk,
        output          led_latch,
        output          led_oe,
		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface ledpanel
		input wire  ledpanel_aclk,
		input wire  ledpanel_aresetn,
		input wire [C_ledpanel_ADDR_WIDTH-1 : 0] ledpanel_awaddr,
		input wire [2 : 0] ledpanel_awprot,
		input wire  ledpanel_awvalid,
		output wire  ledpanel_awready,
		input wire [C_ledpanel_DATA_WIDTH-1 : 0] ledpanel_wdata,
		input wire [(C_ledpanel_DATA_WIDTH/8)-1 : 0] ledpanel_wstrb,
		input wire  ledpanel_wvalid,
		output wire  ledpanel_wready,
		output wire [1 : 0] ledpanel_bresp,
		output wire  ledpanel_bvalid,
		input wire  ledpanel_bready,
		input wire [C_ledpanel_ADDR_WIDTH-1 : 0] ledpanel_araddr,
		input wire [2 : 0] ledpanel_arprot,
		input wire  ledpanel_arvalid,
		output wire  ledpanel_arready,
		output wire [C_ledpanel_DATA_WIDTH-1 : 0] ledpanel_rdata,
		output wire [1 : 0] ledpanel_rresp,
		output wire  ledpanel_rvalid,
		input wire  ledpanel_rready
	);
  
// Instantiation of Axi Bus Interface ledpanel
	ledpanel_v1_0_ledpanel # ( 
		.C_S_AXI_DATA_WIDTH(C_ledpanel_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_ledpanel_ADDR_WIDTH)
	) ledpanel_v1_0_ledpanel_inst (
		.S_AXI_ACLK(ledpanel_aclk),
		.S_AXI_ARESETN(ledpanel_aresetn),
		.S_AXI_AWADDR(ledpanel_awaddr),
		.S_AXI_AWPROT(ledpanel_awprot),
		.S_AXI_AWVALID(ledpanel_awvalid),
		.S_AXI_AWREADY(ledpanel_awready),
		.S_AXI_WDATA(ledpanel_wdata),
		.S_AXI_WSTRB(ledpanel_wstrb),
		.S_AXI_WVALID(ledpanel_wvalid),
		.S_AXI_WREADY(ledpanel_wready),
		.S_AXI_BRESP(ledpanel_bresp),
		.S_AXI_BVALID(ledpanel_bvalid),
		.S_AXI_BREADY(ledpanel_bready),
		.S_AXI_ARADDR(ledpanel_araddr),
		.S_AXI_ARPROT(ledpanel_arprot),
		.S_AXI_ARVALID(ledpanel_arvalid),
		.S_AXI_ARREADY(ledpanel_arready),
		.S_AXI_RDATA(ledpanel_rdata),
		.S_AXI_RRESP(ledpanel_rresp),
		.S_AXI_RVALID(ledpanel_rvalid),
		.S_AXI_RREADY(ledpanel_rready),
        
        .led_rgb1(led_rgb1),
        .led_rgb2(led_rgb2),
        .led_abc(led_abc),
        .led_clk(led_clk),
        .led_latch(led_latch),
        .led_oe(led_oe)
	);

	// Add user logic here

	// User logic ends

	endmodule
