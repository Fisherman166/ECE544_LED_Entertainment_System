// nescontrol.v - Module to control the NES controller inputs
//
// ECE 544 - FInal Project - Winter 2016
//
// Nathan Morelli, Sean Koppenhafer, Jessica Bare
//
// Last Modified:	5/17/2015
//
// Description:
// ------------
// Module that converts NES controller serial output into signals
// for each button used for this project
//
///////////////////////////////////////////////////////////////////////////

module nescontrol
#(
  //Parameters
  parameter integer	CLK_FREQUENCY_HZ		= 100000000, 
  parameter integer	SHIFTER_CLK   			= 200000,
  parameter integer	UPDATE_100_HZ 		  = 100,
  parameter integer CNTR_WIDTH 		  		= 32
)

(
  //Ports
  //Inputs from board
  input clk,
  input reset,
  
  //Input from NES
  input nes_data,
  
  //Outputs to NES
  output reg nes_latch,
  output reg nes_pulse,
  
  output reg [7:0] nes_btns
);

  //========================
  //       CLOCK GEN
  //========================
  //Variables for 200 KHz clock
  reg                     tick_200;
  reg   [CNTR_WIDTH-1:0]  clk_cnt_200;
  wire  [CNTR_WIDTH-1:0]  clk_200_total = ((CLK_FREQUENCY_HZ / SHIFTER_CLK) - 1);
  
  //Variables for 100 Hz clock
  reg                     tick_100;
  reg   [CNTR_WIDTH-1:0]  clk_cnt_100;
  wire  [CNTR_WIDTH-1:0]  clk_100_total = ((CLK_FREQUENCY_HZ / UPDATE_100_HZ) - 1);
  
  
  // generate update clock enable for 200 KHz
	always @(posedge clk)
	begin
		if (reset)	//Reset count to 0
		begin
			clk_cnt_200 <= {CNTR_WIDTH{1'b0}};
      tick_200 <= 1'b0;
		end
		else if (clk_cnt_200 == clk_200_total)	//If max reached set tick and reset count to zero
		begin
			clk_cnt_200 <= {CNTR_WIDTH{1'b0}};
      tick_200 <= 1'b1;
		end
		else	//Else increment count and reset tick to zero
		begin
			clk_cnt_200 <= clk_cnt_200 + 1'b1;
			tick_200 <= 1'b0;
		end
	end

  // generate update clock enable for 100 Hz
	always @(posedge clk)
	begin
		if (reset)	//Reset count to 0
		begin
			clk_cnt_100 <= {CNTR_WIDTH{1'b0}};
      tick_100 <= 1'b0;
		end
		else if (clk_cnt_100 == clk_100_total)	//If max reached set tick and reset count to zero
		begin
			tick_100 <= 1'b1;
			clk_cnt_100 <= {CNTR_WIDTH{1'b0}};
		end
		else	//Else increment count and reset tick to zero
		begin
			clk_cnt_100 <= clk_cnt_100 + 1'b1;
			tick_100 <= 1'b0;
		end
	end
  //==========================
  //    END OF CLOCK GEN
  //==========================
  
  
  //==========================
  //     STATE MACHINE
  //==========================
  //State definitions
  localparam RESET= 5'b11111;
  localparam IDLE = 5'b00000;
  localparam L1   = 5'b00001;
  localparam L2   = 5'b00010;
  localparam L3   = 5'b00011;
  localparam L4   = 5'b00100;
  localparam P1   = 5'b00101;
  localparam P2   = 5'b00110;
  localparam P3   = 5'b00111;
  localparam P4   = 5'b01000;
  localparam P5   = 5'b01001;
  localparam P6   = 5'b01010;
  localparam P7   = 5'b01011;
  localparam P8   = 5'b01100;
  localparam P9   = 5'b01101;
  localparam P10  = 5'b01110;
  localparam P11  = 5'b01111;
  localparam P12  = 5'b10000;
  localparam P13  = 5'b10001;
  localparam P14  = 5'b10010;
  
  //State variables
  reg [4:0] state;
  reg [4:0] nextstate;
 
  //Sequential logic
  always @ (posedge clk)
  begin
    if(reset)
      state <= RESET;
    else if (tick_200)
      state <= nextstate;
    else 
      state <= state;
  end
  
  //Next state logic
  always @ (*)
  begin
    case (state)
      RESET:nextstate = IDLE;
      IDLE: nextstate = tick_100 ? L1 : IDLE; //Wait in idle for 100Hz tick
      L1:   nextstate = L2;
      L2:   nextstate = L3;
      L3:   nextstate = L4;
      L4:   nextstate = P1;
      P1:   nextstate = P2;
      P2:   nextstate = P3;
      P3:   nextstate = P4;
      P4:   nextstate = P5;
      P5:   nextstate = P6;
      P6:   nextstate = P7;
      P7:   nextstate = P8;
      P8:   nextstate = P9;
      P9:   nextstate = P10;
      P10:  nextstate = P11;
      P11:  nextstate = P12;
      P12:  nextstate = P13;
      P13:  nextstate = P14;
      P14:  nextstate = IDLE;
      default: nextstate = IDLE;
    endcase
  end
  
  //Output logic
  always @(posedge tick_200)
  begin
    //nes_btns = 8'hff;
    case (state)
    RESET:
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= 8'hFF;
    end
    IDLE: //Waiting for 100 hz tick.  
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= nes_btns;
    end
    L1: //After 100 Hz send Latch high
    begin
      nes_latch  <= 1'b1;
      nes_pulse  <= 1'b0;
      nes_btns  <= nes_btns;
    end
    L2: //Leave latch high, store A button value
    begin
      nes_latch  <= 1'b1;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_data, nes_btns[6:0]}; //Set new value of A.  Retain other button values
    end
    L3: //Leave latch high
    begin
      nes_latch  <= 1'b1;
      nes_pulse  <= 1'b0;
      nes_btns  <= nes_btns;
    end
    L4: //Set latch low
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= nes_btns;
    end
    P1: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P2: //Set Pulse Low and save B button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7], nes_data, nes_btns[5:0]}; //Set new value of B.  Retain other button values
    end
    P3: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P4: //Set Pulse Low and save Select button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:6], nes_data, nes_btns[4:0]}; //Set new value of Select.  Retain other button values
    end
    P5: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P6: //Set Pulse Low and save Start button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:5], nes_data, nes_btns[3:0]}; //Set new value of Start.  Retain other button values
    end
    P7: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P8: //Set Pulse Low and save Up button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:4], nes_data, nes_btns[2:0]}; //Set new value of Up.  Retain other button values
    end
    P9: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P10: //Set Pulse Low and save Down button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:3], nes_data, nes_btns[1:0]}; //Set new value of Down.  Retain other button values
    end
    P11: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P12: //Set Pulse Low and save Left button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:2], nes_data, nes_btns[0]}; //Set new value of Left.  Retain other button values
    end
    P13: //Set Pulse High
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b1;
      nes_btns  <= nes_btns;
    end
    P14: //Set Pulse Low and save Right button value
    begin
      nes_latch  <= 1'b0;
      nes_pulse  <= 1'b0;
      nes_btns  <= {nes_btns[7:1], nes_data}; //Set new value of Right.  Retain other button values
    end
    default:
    begin
      nes_latch <= 1'b0;
      nes_pulse <= 1'b0;
      nes_btns <= nes_btns;
    end
    endcase
  end
  //===========================
  //   END OF STATE MACHINE
  //===========================
endmodule
    
