// testbench for wrapper file

module wrapper_test();

timeunit 1ns;
timeprecision 100ps;

  logic CLOCK_50;
  
  logic [9:0] SW;
  logic [3:0] KEY; // DE1-SoC keys are active low
  wire [9:0] LEDR;
  wire [6:0] HEX0;
  wire [6:0] HEX1;
  wire [6:0] HEX2;
  wire [6:0] HEX3;
  wire [6:0] HEX4;
  wire [6:0] HEX5;

	logic [35:0] GPIO_0;
	
	wire [7:0] VGA_R,VGA_G,VGA_B;
	wire VGA_HS, VGA_VS, VGA_CLK, VGA_BLANK_N;
	
	de1_soc_wrapper dut (
	
		.CLOCK_50, .KEY, .SW,
		.VGA_R, .VGA_G, .VGA_B, .VGA_HS, .VGA_VS, .VGA_CLK, .VGA_BLANK_N,
		.LEDR, .HEX0, .HEX1, .HEX2, .HEX3, .HEX4, .HEX5, .GPIO_0
		
	);
	
	always
	  begin
					CLOCK_50 = 0;
		#5ns  CLOCK_50 = 1;
		#10ns CLOCK_50 = 0;
		#5ns  CLOCK_50 = 0;
	  end
	
	initial
		begin
						KEY[0] = 1;
						KEY[1] = 1;
						KEY[2] = 1;
						KEY[3] = 1;
						
						GPIO_0[0] = 1;
						GPIO_0[1] = 1;
						GPIO_0[2] = 1;
						GPIO_0[3] = 1;

						KEY[0] = 0; //reset
		#10ns		KEY[0] = 1;
		
		#2ms
		
						GPIO_0[0] = 0;
		#1s			GPIO_0[0] = 1;
		
		#2ms 		GPIO_0[1] = 0;
		#1s		 	GPIO_0[1] = 1;
		
		
		$stop;
		end
		
endmodule