// ledpanel.v
//
// ECE 544 - FInal Project - Winter 2016
//
// Nathan Morelli, Sean Koppenhafer, Jessica Bare
//
// Last Modified:	3/6/2016
//
// Description:
// ------------
// Module controls the LED panel used as a display for our final project.
// Contains two FSMs to control the RGB LED Panel.  One FSM controls the 
// shift registers which drive the RGB LEDs.  The other updates the memory
// arrays containing the color of each LED.
//
///////////////////////////////////////////////////////////////////////////

module ledpanel
#(
  //Parameters
  parameter integer	CLK_FREQUENCY_HZ		= 100000000, 
  parameter integer	SHIFTER_CLK   			= 1000000,     //Defaults to 1MHz, which produces a 500KHz clock for the LED panel
  parameter integer	UPDATE_CLK 		      = 1000,         //Defaults to 100 Hz
  parameter integer CNTR_WIDTH 		  		= 32
)

(
  //Ports
  //Inputs from board
  input clk,
  input reset,
  
  //Inputs from software
  input [4:0] x_address,
  input [3:0] y_address,
  input [2:0] color,
  input       update_panel,
  input       new_data,
 
  //Outputs to panel
  output reg  [2:0]   led_rgb1,
  output reg  [2:0]   led_rgb2,
  output reg  [2:0]   led_abc,
  output reg          led_clk,
  output reg          led_latch,
  output reg          led_oe
  
);

  //========================
  //ADDRESS DECODE FUNCTION
  //========================
  function [2:0] decode_abc;
  input    [2:0] abc;
  begin
    case (abc)
      3'b000: decode_abc = 3'b100;
      3'b001: decode_abc = 3'b010;
      3'b010: decode_abc = 3'b110;
      3'b011: decode_abc = 3'b001;
      3'b100: decode_abc = 3'b101;
      3'b101: decode_abc = 3'b011;
      3'b110: decode_abc = 3'b111;
      3'b111: decode_abc = 3'b000;
    endcase
  end
  endfunction
  //========================
  //  END DECODE FUNCTION
  //========================

  
  //========================
  //       CLOCK GEN
  //========================
  //Variables for Shifter clock
  reg                     tick_shift;
  reg   [CNTR_WIDTH-1:0]  clk_cnt_shift;
  wire  [CNTR_WIDTH-1:0]  clk_shift_total = ((CLK_FREQUENCY_HZ / SHIFTER_CLK) - 1);
  
  //Variables for 100 Hz Update clock
  reg                     tick_update;
  reg   [CNTR_WIDTH-1:0]  clk_cnt_update;
  wire  [CNTR_WIDTH-1:0]  clk_update_total = ((CLK_FREQUENCY_HZ / UPDATE_CLK) - 1);
  
  // generate update clock enable
	always @(posedge clk)
	begin
		if (reset == 1'b0)	//Reset count to 0
		begin
			clk_cnt_shift <= {CNTR_WIDTH{1'b0}};
      tick_shift <= 1'b0;
		end
		else if (clk_cnt_shift == clk_shift_total)	//If max reached set tick and reset count to zero
		begin
			clk_cnt_shift <= {CNTR_WIDTH{1'b0}};
      tick_shift <= 1'b1;
		end
		else	//Else increment count and reset tick to zero
		begin
			clk_cnt_shift <= clk_cnt_shift + 1'b1;
			tick_shift <= 1'b0;
		end
	end

  // generate update clock enable
	always @(posedge clk)
	begin
		if (reset == 1'b0)	//Reset count to 0
		begin
			clk_cnt_update <= {CNTR_WIDTH{1'b0}};
      tick_update <= 1'b0;
		end
		else if (clk_cnt_update == clk_update_total)	//If max reached set tick and reset count to zero
		begin
			tick_update <= 1'b1;
			clk_cnt_update <= {CNTR_WIDTH{1'b0}};
		end
		else	//Else increment count and reset tick to zero
		begin
			clk_cnt_update <= clk_cnt_update + 1'b1;
			tick_update <= 1'b0;
		end
	end
  //==========================
  //    END OF CLOCK GEN
  //==========================
  
  
  //==========================
  //   DATA STATE MACHINE
  //==========================
  //State definitions
  localparam DATA_IDLE  = 2'b00;
  localparam WRITE      = 2'b01;
  localparam UPDATE     = 2'b10;
  
  
  //State variables
  reg [1:0] data_state = 2'b00;
  reg [1:0] data_nextstate = 2'b00;
    
  //Memory arrays
  reg [2:0] mem_array1 [511:0];
  reg [2:0] mem_array2 [511:0];
  
  //Memory array index variable to clear array
  integer i;
  
  //Memory array select bit
  reg mem_select;
  
  //Update edge detect variable
  reg prev_update;
  
  //Sequential logic
  always @ (posedge clk)
  begin
    if(reset == 1'b0 )
      data_state <= DATA_IDLE;
    else 
      data_state <= data_nextstate;
  end
  
  //Next state logic
  always @ (*)
  begin
    case (data_state)
      DATA_IDLE    : data_nextstate = (!prev_update && update_panel) ? UPDATE : (new_data ? WRITE : DATA_IDLE);
      WRITE   : data_nextstate = DATA_IDLE;
      UPDATE  : data_nextstate = DATA_IDLE;
      default : data_nextstate = DATA_IDLE;
    endcase
  end
  
  //Output Logic
  always @ (posedge clk)
  begin
    case (data_state)
      DATA_IDLE:   //Save current update into previous update variable for edge detection
      begin
        prev_update <= update_panel;
      end
      WRITE:  //If new data is available, write to selected memory array using inputs
      begin
        if (mem_select)
          mem_array1[{y_address, x_address}] <= color;
        else
          mem_array2[{y_address, x_address}] <= color;
      end
      UPDATE: //If panel update is requested, flip read/write memory arrays and clear new write array
      begin
        if (mem_select)
        begin
          mem_select <= 1'b0;
          for(i = 0; i < 512; i = i + 1)
            mem_array2[i] <= 3'b000;
        end
        else
        begin
          mem_select <= 1'b1;
          for(i = 0; i < 512; i = i + 1)
            mem_array1[i] <= 3'b000;
        end
      end
    endcase
  end
  //===========================
  // END OF DATA STATE MACHINE
  //===========================
  
  
  //==========================
  //   PANEL STATE MACHINE
  //==========================
  //State definitions
  localparam IDLE     = 3'b000;
  localparam LINE     = 3'b001;
  localparam ADDRESS  = 3'b010;
  localparam DATA     = 3'b011;
  localparam CLOCK    = 3'b100;
  localparam LATCH    = 3'b101;
  
  //Counter boundary
  localparam TOTAL_SHIFTS = 8'd32;
  localparam TOTAL_LINES = 3'b111;
  
  //LED Half Select Parameters
  localparam TOP_SELECT     = 1'b0;
  localparam BOTTOM_SELECT  = 1'b1;
  
  //State variables
  reg [2:0] panel_state;
  reg [2:0] panel_nextstate;
  
  //Counter variable
  reg [7:0] x_count = 8'h00;
  
  //y_address variable
  reg [2:0] y_decode = 3'b000;
 
  //Sequential logic
  always @ (posedge clk)
  begin
    if(reset == 1'b0)
      panel_state <= IDLE;
    else if (tick_shift)
      panel_state <= panel_nextstate;
    else 
      panel_state <= panel_state;
  end
  
  //Next state logic
  always @ (*)
  begin
    case (panel_state)
      IDLE    : panel_nextstate = tick_update ? ADDRESS : IDLE;
      LINE    : panel_nextstate = ADDRESS;
      ADDRESS : panel_nextstate = DATA;
      DATA    : panel_nextstate = CLOCK;
      CLOCK   : panel_nextstate = (x_count == TOTAL_SHIFTS) ? LATCH : DATA;
      LATCH   : panel_nextstate = (led_abc == TOTAL_LINES) ? IDLE : LINE;
      default : panel_nextstate = IDLE;
    endcase
  end
  
  //Output logic
  always @ (posedge tick_shift)
  begin
    //Default Output Assignments
    led_rgb1  <= 3'b000;
    led_rgb2  <= 3'b000;
    led_abc   <= led_abc;
    led_clk   <= 1'b0;
    led_latch <= 1'b0;
    led_oe    <= 1'b1;
    x_count   <= 8'h00;
    
    //Assign outputs based on current state
    case (panel_state)
      IDLE: //This state clears the ABC X address pins
      begin
        led_abc <= 3'b000;
      end
      LINE: //This state increments the ABC X address pins
      begin
        led_abc <= led_abc + 1'b1;
      end
      ADDRESS:  //This state decodes current ABC for array indexing
      begin
        y_decode <= decode_abc(led_abc);
      end
      DATA: //This state retrieves new RGB data from selected memory array.  
      begin
        if (mem_select)
        begin                     //MSB is toggled for RGB1 and RGB2
          led_rgb1  <= mem_array2[{TOP_SELECT, y_decode, x_count[4:0]}];
          led_rgb2  <= mem_array2[{BOTTOM_SELECT, y_decode, x_count[4:0]}];
        end
        else
        begin
          led_rgb1  <= mem_array1[{TOP_SELECT, y_decode, x_count[4:0]}];
          led_rgb2  <= mem_array1[{BOTTOM_SELECT, y_decode, x_count[4:0]}];
        end
        //Enable LED panel
        led_oe    <= 1'b0;
        //Increment x count which tracks number of RGB values shifted in
        x_count   <= x_count + 1'b1;
      end
      CLOCK:  //This state clocks in current RGB values and enables output
      begin
        led_rgb1  <= led_rgb1;
        led_rgb2  <= led_rgb2;
        led_clk   <= 1'b1;
        led_oe    <= 1'b0;
        x_count   <= x_count;
      end
      LATCH:  //This state latches currently shifted in RGB values
      begin
        led_latch <= 1'b1;
      end
    endcase
  end
  //===========================
  // END OF PANEL STATE MACHINE
  //===========================
endmodule