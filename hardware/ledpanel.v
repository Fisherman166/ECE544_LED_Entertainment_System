// ledpanel.v
//
// ECE 544 - FInal Project - Winter 2016
//
// Nathan Morelli, Sean Koppenhafer, Jessica Bare
//
// Last Modified:	3/4/2016
//
// Description:
// ------------
// Module controls the LED panel used as a display for our final project.
// Uses an FSM to control shift registers which drive the RGB leds.
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
 
  //Outputs to panel
  output reg  [2:0]   led_rgb1,
  output reg  [2:0]   led_rgb2,
  output reg  [2:0]   led_abc,
  output reg          led_clk,
  output reg          led_latch,
  output reg          led_oe
  
);

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
		if (reset)	//Reset count to 0
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
		if (reset)	//Reset count to 0
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
  //     STATE MACHINE
  //==========================
  //State definitions
  localparam RESET  = 3'b111;
  localparam IDLE   = 3'b000;
  localparam LINE   = 3'b001;
  localparam DATA   = 3'b010;
  localparam CLOCK  = 3'b011;
  localparam LATCH  = 3'b100;
  
  //Counter boundary
  localparam TOTAL_SHIFTS = 8'd32;
  localparam TOTAL_LINES = 3'b111;
  
  //State variables
  reg [2:0] state;
  reg [2:0] nextstate;
  
  //Counter variable
  reg [7:0] shift_count = 8'h00;
 
  //Sequential logic
  always @ (posedge clk)
  begin
    if(reset)
      state <= RESET;
    else if (tick_shift)
      state <= nextstate;
    else 
      state <= state;
  end
  
  //Next state logic
  always @ (*)
  begin
    case (state)
      RESET   : nextstate = IDLE;
      IDLE    : nextstate = DATA;//tick_update ? DATA : IDLE;
      LINE    : nextstate = DATA;
      DATA    : nextstate = CLOCK;
      CLOCK   : nextstate = (shift_count == TOTAL_SHIFTS) ? LATCH : DATA;
      LATCH   : nextstate = (led_abc == TOTAL_LINES) ? IDLE : LINE;
      default : nextstate = IDLE;
    endcase
  end
  
  //Output logic
  always @ (posedge tick_shift)
  begin
    case (state)
    RESET:
    begin
      led_rgb1  <= 3'b000;
      led_rgb2  <= 3'b000;
      led_abc   <= 3'b000;
      led_clk   <= 1'b0;
      led_latch <= 1'b0;
      led_oe    <= 1'b1;
      shift_count = 8'h00;
    end
    IDLE:
    begin
      led_rgb1  <= 3'b000;
      led_rgb2  <= 3'b000;
      led_abc   <= 3'b000;
      led_clk   <= 1'b0;
      led_latch <= 1'b0;
      led_oe    <= 1'b0;
      shift_count = 8'h00;
    end
    LINE:
    begin
      led_rgb1  <= 3'b000;
      led_rgb2  <= 3'b000;
      led_abc   <= led_abc + 1'b1;
      led_clk   <= 1'b0;
      led_latch <= 1'b0;
      led_oe    <= 1'b1;
      shift_count = 8'h00;
    end
    DATA:
    begin
      led_rgb1  <= led_abc;
      led_rgb2  <= led_abc;
      led_abc   <= led_abc;
      led_clk   <= 1'b0;
      led_latch <= 1'b0;
      led_oe    <= 1'b0;
      shift_count = shift_count + 1'b1;
    end
    CLOCK:
    begin
      led_rgb1  <= led_abc;
      led_rgb2  <= led_abc;
      led_abc   <= led_abc;
      led_clk   <= 1'b1;
      led_latch <= 1'b0;
      led_oe    <= 1'b0;
      shift_count = shift_count;
    end
    LATCH:
    begin
      led_rgb1  <= 3'b000;
      led_rgb2  <= 3'b000;
      led_abc   <= led_abc;
      led_clk   <= 1'b0;
      led_latch <= 1'b1;
      led_oe    <= 1'b1;
      shift_count = 8'h00;
    end
    default:
    begin
      led_rgb1  <= 3'b000;
      led_rgb2  <= 3'b000;
      led_abc   <= 3'b000;
      led_clk   <= 1'b0;
      led_latch <= 1'b0;
      led_oe    <= 1'b1;
      shift_count = 8'h00;
    end
    endcase
  end
  //===========================
  //   END OF STATE MACHINE
  //===========================
endmodule