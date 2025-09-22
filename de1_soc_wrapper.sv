// Example code for an AHBLite System-on-Chip
//  Iain McNally
//  ECS, University of Soutampton
//
// This module is a wrapper allowing the system to be used on the DE1-SoC FPGA board
//

module de1_soc_wrapper(

  input CLOCK_50,
  
  input [3:0] KEY, // DE1-SoC keys are active low
	input [9:0] SW,
	
	input [35:0] GPIO_0,

  output [9:0] LEDR,
  output [6:0] HEX0,
  output [6:0] HEX1,
  output [6:0] HEX2,
  output [6:0] HEX3,
  output [6:0] HEX4,
  output [6:0] HEX5,
	
	output [7:0] VGA_R,VGA_G,VGA_B, 
	output VGA_HS, VGA_VS, VGA_CLK, VGA_BLANK_N

);

timeunit 1ns;
timeprecision 100ps;
	
	localparam seven_seg_H = ~7'b1110110;
  localparam seven_seg_L = ~7'b0111000; 
  localparam seven_seg_E = ~7'b1111001; 
  localparam seven_seg_o = ~7'b1011100; 
  localparam seven_seg_off = ~7'b0000000; 
  
  wire [3:0] buttons;
  wire [3:0] joystick;
	
	wire HCLK, HRESETn, LOCKUP;
	
	assign joystick = ~GPIO_0[3:0];
		
  // Driving with KEY[0] 
  assign HRESETn = KEY[0];
	
	// Flip buttons
	assign buttons = ~KEY;

  // Drive HCLK from 50MHz de0 board clock
  assign HCLK = CLOCK_50;
	
  vga_top vga_mod(.HCLK, .HRESETn, .VGA_R, .VGA_G, .VGA_B, .VGA_HS, .VGA_VS, .VGA_CLK,
		.VGA_BLANK_N, .LOCKUP, .KEY(buttons), .SW, .joystick);

  // these digits on the seven-segment display are not used here
  assign HEX0 = seven_seg_o;
  assign HEX1 = seven_seg_L;
  assign HEX2 = seven_seg_L;
  assign HEX3 = seven_seg_E;
  assign HEX4 = seven_seg_H;
	assign HEX5 = '1;
	
	
	// test with LEDs
	assign LEDR[0] = ~GPIO_0[0];
	assign LEDR[1] = ~GPIO_0[1];
	assign LEDR[2] = ~GPIO_0[2];
	assign LEDR[3] = ~GPIO_0[3];

endmodule