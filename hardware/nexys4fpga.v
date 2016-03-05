// nexys4fpga.v
//
// ECE 544 - FInal Project - Winter 2016
//
// Nathan Morelli, Sean Koppenhafer, Jessica Bare
//
// Last Modified:	3/4/2016
//
// Description:
// ------------
// Top level module to test out RGB LED Panel 
// prior to building our actual final project.
//
///////////////////////////////////////////////////////////////////////////

module Nexys4fpga (
	input 			    clk,          // 100MHz clock from on-board oscillator
	input				    btnCpuReset,  // red pushbutton input -> db_btns[0]

	input	  [15:0]  sw,					  // switch inputs
	output	[15:0]  led,  			  // LED outputs	
  
  input           nes_data1,    // Serial input line from first NES controller
  input           nes_data2,    // Serial input line from second NES controller
    
  output          nes_latch1,   // Latch output to first NES controller
  output          nes_pulse1,   // Pulse output to first NES controller
  
  output          nes_latch2,   // Latch output to second NES controller
  output          nes_pulse2,   // Pulse output to second NES controller
  
  //Signals to led panel
  output  [2:0]   led_rgb1,
  output  [2:0]   led_rgb2,
  output  [2:0]   led_abc,
  output          led_clk,
  output          led_latch,
  output          led_oe
  
); 

	// Outputs of Debounce block
	wire 	[15:0]		db_sw;			// debounced switches
  wire  [5:0]     db_btns;    // debounced buttons only used for Reset
	
  // Clock and reset
	wire  sysclk = clk;					  // 100MHz clock from on-board oscillator	
	wire  sysreset = db_btns[0];	// system reset signal - asserted high to force reset
  wire  reset_high = ~sysreset; // Invert reset signal
  
  // Output of NES controller modules
  wire  [7:0]  nes_btns1, nes_btns2;
  
  wire  [7:0] controller1 = ~nes_btns1;
  wire  [7:0] controller2 = ~nes_btns2;
  
  // Assign NES controllers to LEDs for debugging
  assign led = {controller1, controller2};

	//instantiate the debounce module
	debounce
	#(
		.RESET_POLARITY_LOW(1),
		.SIMULATE(0)
	)  	DB
	(
		.clk(sysclk),	
		.pbtn_in({1'b0,1'b0,1'b0,1'b0,1'b0,btnCpuReset}),
		.pbtn_db(db_btns),
		.switch_in(sw),
		.swtch_db(db_sw)
		

	);	


  //instantiate first nes controller module
  nescontrol
  #(
    .UPDATE_100_HZ(100)
  ) NES1 
  (
    .clk(sysclk),
    .reset(reset_high),
    .nes_data(nes_data1),
    .nes_latch(nes_latch1),
    .nes_pulse(nes_pulse1),
    .nes_btns(nes_btns1)
  );
    
  //instantiate second nes controller module
  nescontrol
  #(
    .UPDATE_100_HZ(100)
  ) NES2 
  (
    .clk(sysclk),
    .reset(reset_high),
    .nes_data(nes_data2),
    .nes_latch(nes_latch2),
    .nes_pulse(nes_pulse2),
    .nes_btns(nes_btns2)
  );
  
  //instantiate LED Panel module
  ledpanel
  #(
  
  ) LED
  (
    .clk(sysclk),
    .reset(reset_high),
    .led_rgb1(led_rgb1),
    .led_rgb2(led_rgb2),
    .led_abc(led_abc),
    .led_clk(led_clk),
    .led_latch(led_latch),
    .led_oe(led_oe)
  );
    
endmodule