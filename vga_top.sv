
// VGA top-level module

module vga_top (

	input logic HCLK, HRESETn,
	output logic [7:0] VGA_R, VGA_G, VGA_B, 
	output logic VGA_HS, VGA_VS, VGA_CLK, VGA_BLANK_N,
	input logic [3:0] KEY,
	input logic [9:0] SW,
	input logic [3:0] joystick,
	
	output LOCKUP
		
		);
	
timeunit 1ns;
timeprecision 10ps;
	
	
  // Global & Master AHB Signals
  wire [31:0] HADDR, HWDATA, HRDATA;
  wire [1:0] HTRANS;
  wire [2:0] HSIZE, HBURST;
  wire [3:0] HPROT;
  wire HWRITE, HMASTLOCK, HRESP, HREADY;

  // Per-Slave AHB Signals
  wire HSEL_PacRAM, HSEL_JOYSTICK, HSEL_ROM, HSEL_RAM, HSEL_CHARROM, HSEL_VIDMEM , HSEL_GhostRAM, HSEL_TIMER;
  wire [31:0] HRDATA_PacRAM, HRDATA_JOYSTICK, HRDATA_ROM, HRDATA_RAM, HRDATA_CHARROM, HRDATA_VIDMEM, HRDATA_GhostRAM, HRDATA_TIMER;
  wire HREADYOUT_PacRAM, HREADYOUT_JOYSTICK, HREADYOUT_ROM, HREADYOUT_RAM, HREADYOUT_CHARROM, HREADYOUT_VIDMEM, HREADYOUT_GhostRAM, HREADYOUT_TIMER;

  // Non-AHB M0 Signals
  wire TXEV, RXEV, SLEEPING, SYSRESETREQ, NMI;
  wire [15:0] IRQ;
	
	// Outputs
	logic [10:0] H_count, V_count;
	logic [10:0] H_count_d1, V_count_d1;
	logic [7:0] sprite1_R, sprite1_G, sprite1_B;
	logic [10:0] H_count_d2, V_count_d2;
	logic [7:0] sprite2_R, sprite2_G, sprite2_B;
	logic [7:0] back_R, back_G, back_B;
	logic video_on, in_sprite1;
	wire [31:0] sprite1_pixel, sprite2_pixel;
	wire [31:0] sprite1_data, sprite2_data;
	wire [3:0] rel_x1, rel_y1;
	wire [3:0] rel_x2, rel_y2;
	wire button_irq;
	logic [5:0] row, col;
	logic [7:0] char;
	logic [4:0] square_x, square_y;
	logic pixel;
	logic [3:0] buttons_out;
	logic [3:0] joystick_out;
	logic [31:0] count;
	
  // Set this to zero because simple slaves do not generate errors
  assign HRESP = '0;

  // Set all interrupt and event inputs to zero
  assign NMI = '0;
  assign IRQ[15:1] = '0;
//	assign IRQ[0] = button_irq;
	assign IRQ[0] = '0;
  assign RXEV = '0;

  CORTEXM0DS m0_1 (

    // AHB Signals
    .HCLK, .HRESETn,
    .HADDR, .HBURST, .HMASTLOCK, .HPROT, .HSIZE, .HTRANS, .HWDATA, .HWRITE,
    .HRDATA, .HREADY, .HRESP,                                   

    // Non-AHB Signals
    .NMI, .IRQ, .TXEV, .RXEV, .LOCKUP, .SYSRESETREQ, .SLEEPING
  );

  // AHB interconnect including address decoder, register and multiplexer
  ahb_interconnect interconnect_1 (

    .HCLK, .HRESETn, .HADDR, .HRDATA, .HREADY,

    .HSEL_SIGNALS({HSEL_TIMER, HSEL_VIDMEM, HSEL_CHARROM, HSEL_JOYSTICK, HSEL_GhostRAM, HSEL_PacRAM, HSEL_RAM, HSEL_ROM}),
    .HRDATA_SIGNALS({HRDATA_TIMER, HRDATA_VIDMEM, HRDATA_CHARROM, HRDATA_JOYSTICK, HRDATA_GhostRAM, HRDATA_PacRAM, HRDATA_RAM, HRDATA_ROM}),
    .HREADYOUT_SIGNALS({HREADYOUT_TIMER, HREADYOUT_VIDMEM, HREADYOUT_CHARROM, HREADYOUT_JOYSTICK, HREADYOUT_GhostRAM, HREADYOUT_PacRAM, HREADYOUT_RAM, HREADYOUT_ROM})

  );
  
  ahb_rom rom_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_ROM),
    .HRDATA(HRDATA_ROM), .HREADYOUT(HREADYOUT_ROM)

  );

  ahb_ram ram_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_RAM), .HRDATA(HRDATA_RAM), .HREADYOUT(HREADYOUT_RAM)

  );
	
	joystick_input joystick_1 (
		
		.HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
		.HSEL(HSEL_JOYSTICK), .HRDATA(HRDATA_JOYSTICK), .HREADYOUT(HREADYOUT_JOYSTICK),
		.joystick, .joystick_out
		
	);
	
	timer timer_1 (
	
		.HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
		.HSEL(HSEL_TIMER), .HRDATA(HRDATA_TIMER), .HREADYOUT(HREADYOUT_TIMER),
		.count
	);
	
	sprite1_ram pacman_ram (
	
		.HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
		.HSEL(HSEL_PacRAM), .HRDATA(HRDATA_PacRAM), .HREADYOUT(HREADYOUT_PacRAM),
		.sprite_pixel(sprite1_pixel), .sprite_data(sprite1_data), 
		.rel_x(rel_x1), .rel_y(rel_y1), .in_sprite(in_sprite1)
		
	);
	
	sprite2_ram ghost_ram (
	
		.HCLK, .HRESETn, .HADDR, .HSIZE, .HTRANS, .HWRITE, .HWDATA, .HREADY,
		.HSEL(HSEL_GhostRAM), .HREADYOUT(HREADYOUT_GhostRAM), .HRDATA(HRDATA_GhostRAM),
		.sprite_pixel(sprite2_pixel), .sprite_data(sprite2_data), 
		.rel_x(rel_x2), .rel_y(rel_y2), .in_sprite(in_sprite2)
		
	);
	
	vga_counter counter (
	
		.HCLK, .HRESETn,
		.VGA_HS, .VGA_VS, .VGA_CLK, .VGA_BLANK_N, .video_on,
		.H_count, .V_count, .H_count_d1, .V_count_d1, 
		.square_x, .square_y, .row, .col
		
	);

	sprite_engine pacman (
	
		.sprite_x(sprite1_data[9:0]), .sprite_y(sprite1_data[19:10]), .HRESETn, 
		.VGA_CLK, .screen_x(H_count_d1), .screen_y(V_count_d1),
		.rel_x(rel_x1), .rel_y(rel_y1), .in_sprite(in_sprite1)
		
	);
	
	sprite_engine ghost (
	
		.sprite_x(sprite2_data[9:0]), .sprite_y(sprite2_data[19:10]), .HRESETn, 
		.VGA_CLK, .screen_x(H_count_d1), .screen_y(V_count_d1),
		.rel_x(rel_x2), .rel_y(rel_y2), .in_sprite(in_sprite2)
		
	);
	
	video_mem video_mem1 (
	
    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
		.HSEL(HSEL_VIDMEM), .HRDATA(HRDATA_VIDMEM), .HREADYOUT(HREADYOUT_VIDMEM),
		.row, .col, .char
		
	);
	
	back_char char_rom (
	
    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
		.HSEL(HSEL_CHARROM), .HRDATA(HRDATA_CHARROM), .HREADYOUT(HREADYOUT_CHARROM),
		.char, .square_x, .square_y, .pixel
		
	);

  
	 // if RGB is red, green, blue then set it to max
  assign sprite1_R = sprite1_pixel[2] ? 255 : 0;
  assign sprite1_G = sprite1_pixel[1] ? 255 : 0;
  assign sprite1_B = sprite1_pixel[0] ? 255 : 0;    
	
	assign sprite2_R = sprite2_pixel[2] ? 255 : 0;
  assign sprite2_G = sprite2_pixel[1] ? 255 : 0;
  assign sprite2_B = sprite2_pixel[0] ? 255 : 0;  
	
	assign back_R = '0;
  assign back_G = '0;
  assign back_B = pixel ? 255:0;
	
	// XOR colours for final output
  assign VGA_R = sprite1_R ^ back_R ^ sprite2_R;
  assign VGA_G = sprite1_G ^ back_G ^ sprite2_G;
  assign VGA_B = sprite1_B ^ back_B ^ sprite2_B;	
	
/* 	// output test
  assign VGA_R = 255;
  assign VGA_G = 0;
  assign VGA_B = 0;
   */

endmodule