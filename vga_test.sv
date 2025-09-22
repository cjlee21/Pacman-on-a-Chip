// testbench for VGA

module vga_test();

timeunit 1ns;
timeprecision 100ps;

    logic HCLK, HRESETn;
    wire [7:0] VGA_R,VGA_G,VGA_B;
    wire VGA_HS,VGA_VS, VGA_CLK, VGA_BLANK_N;
		logic button;
	
	vga_top dut(.HCLK, .HRESETn, .VGA_R, .VGA_G, .VGA_B, .button,
					.VGA_HS, .VGA_VS, .VGA_CLK, .VGA_BLANK_N);
	
	always
	  begin
					HCLK = 0;
		#5ns  HCLK = 1;
		#10ns HCLK = 0;
		#5ns  HCLK = 0;
	  end
	  
	initial
	  begin	
					HRESETn = 1;
		#10ns	HRESETn = 0;	//reset
		#10ns HRESETn = 1;
		
		#20ns button = 1;
		#2000ns button = 0;	//press
		#50ns button = 1;	//release
		#30000000
		$stop;
	  end	  

endmodule