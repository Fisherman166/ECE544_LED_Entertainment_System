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
  input clk,              // 100MHz clock from on-board oscillator
  input btnCpuReset,      // red pushbutton input -> db_btns[0]

  input nes_data1,        // Serial input line from first NES controller
  input nes_data2,        // Serial input line from second NES controller
  
  input uart_rtl_rxd,     // UART RX
  output uart_rtl_txd,    // UART TX

  output  nes_latch1,     // Latch output to first NES controller
  output  nes_pulse1,     // Pulse output to first NES controller

  output  nes_latch2,     // Latch output to second NES controller
  output  nes_pulse2,     // Pulse output to second NES controller

  output  [2:0] led_rgb1, // RGB output for top half of LED Panel
  output  [2:0] led_rgb2, // RGB output for bottom half of LED Panel
  output  [2:0] led_abc,  // LED Panel Row address bits
  output        led_clk,  // LED Panel Clock
  output        led_latch,// LED Panel Latch
  output        led_oe,    // LED Panel output enable
  
  output [15:0] led
); 
  // Clock and reset
  wire  sysclk = clk;					  // 100MHz clock from on-board oscillator	
  wire  sysreset = btnCpuReset;	// system reset signal - asserted high to force reset
  wire  reset_high = ~sysreset; // Invert reset signal
  
  wire [7:0] nes_buttons2;
  wire [7:0] nes_buttons1;
  wire [4:0] x_address;
  wire [3:0] y_address;
  wire [2:0] color;
  wire        new_data, update_panel, clock;
  //            15,14,  13,       12,           11-7,      6-3,      2-0 
  //assign led = {2'b0, new_data, update_panel, x_address, y_address, color};
  
    //            15,   11,      10,        9,      8-6,      5-3,      2-0 
  assign led = {4'b0, led_oe, led_clk, led_latch, led_rgb2, led_rgb1, led_abc};
  

  //instantiate the embedded system module
  design_1 DESIGN_1
  (
    .led_abc(led_abc),
    .led_clk(led_clk),
    .led_latch(led_latch),
    .led_oe(led_oe),
    .led_rgb1(led_rgb1),
    .led_rgb2(led_rgb2),
    .nes_data1(nes_data1),
    .nes_data2(nes_data2),
    .nes_latch1(nes_latch1),
    .nes_latch2(nes_latch2),
    .nes_pulse1(nes_pulse1),
    .nes_pulse2(nes_pulse2),
    .reset_high(reset_high),
    .sysclk(sysclk),
    .sysreset(sysreset),
    .uart_rtl_rxd(uart_rtl_rxd),
    .uart_rtl_txd(uart_rtl_txd),
    .nes_buttons1(nes_buttons1),
    .nes_buttons2(nes_buttons2),
    .color(color),
    .x_address(x_address),
    .y_address(y_address),
    .update_panel(update_panel),
    .new_data(new_data),
    .clock(clock)
  );
        
  //instantiate LED Panel module
  // ledpanel l1
  // (
    //Inputs from board
    // .clk(clock),
    // .reset(reset_high),
    //Inputs from software
    // .x_address(x_address),
    // .y_address(y_address),
    // .color(color),
    // .update_panel(update_panel),
    // .new_data(new_data),
    //Outputs to panel
    // .led_rgb1(led_rgb1),
    // .led_rgb2(led_rgb2),
    // .led_abc(led_abc),
    // .led_clk(led_clk),
    // .led_latch(led_latch),
    // .led_oe(led_oe) 
  // );

    
endmodule