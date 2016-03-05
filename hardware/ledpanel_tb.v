//Test bench for ledpanel


module ledpanel_tb();


  reg reset;
  
  reg clock = 0;
  
  wire [2:0] led_rgb1, led_rgb2, led_abc;
  wire led_clk, led_latch, led_oe;
            
            
  // Generate 100MHz clock
  always #5 clock = ~clock;

  ledpanel
  #(
    //.UPDATE_CLK(500000)
  ) LED
  (
    .clk(clock),
    .reset(reset),
    .led_rgb1(led_rgb1),
    .led_rgb2(led_rgb2),
    .led_abc(led_abc),
    .led_clk(led_clk),
    .led_latch(led_latch),
    .led_oe(led_oe)
  );
  
  initial
  begin
  #10
  reset = 1;
  repeat (100) @(posedge clock);
  reset = 0;
  repeat (2000000) @(posedge clock);
  reset = 1;
  repeat (100) @(posedge clock);
  reset = 0;
  $stop;
  end
endmodule