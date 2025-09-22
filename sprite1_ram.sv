  // This module is for Sprite RAM.
// Using RAM module from the ARM SoC ASIC Example with Software Customisation
// lab walkthrough.
//
// Number of addressable locations : 2**MEMWIDTH
// Size of each addressable location : 8 bits
// Supported transfer sizes : Word
// Alignment of base address : Word aligned

module sprite1_ram #(
  parameter MEMWIDTH = 11
)(
  //AHBLITE INTERFACE

    //Slave Select Signal
    input HSEL,
    //Global Signals
    input HCLK,
    input HRESETn,
    //Address, Control & Write Data
    input HREADY,
    input [31:0] HADDR,
    input [1:0] HTRANS,
    input HWRITE,
		input [2:0] HSIZE,
    input [31:0] HWDATA,
    // Transfer Response & Read Data
    output HREADYOUT,
    output [31:0] HRDATA,
	
	//Sprite Controller Interface (Read-only)
		input in_sprite,
		input [3:0] rel_x, rel_y,
		output [31:0] sprite_pixel,
		output [31:0] sprite_data

);

timeunit 1ns;
timeprecision 100ps;

  localparam No_Transfer = 2'b0;

// Memory Array, creates number of words stored in ram (size of ram)
  logic [31:0] memory[0:(2**(MEMWIDTH-2)-1)];

	//control signals are stored in registers
  logic write_enable;
  logic [MEMWIDTH-3:0] word_address;

//Generate the control signals in the address phase
  always_ff @(posedge HCLK, negedge HRESETn)
    if (! HRESETn )
      begin
        write_enable <= '0;
        word_address <= '0;
      end
    else if ( HREADY && HSEL && (HTRANS != No_Transfer) )
      begin
        write_enable <= HWRITE;
        word_address <= HADDR[MEMWIDTH-1:2];
     end
    else
      begin
        write_enable <= '0;
        word_address <= '0;
     end

//Act on control signals in the data phase

  // write
  always_ff @(posedge HCLK)
    if ( write_enable )
			memory[word_address] <= HWDATA;
	
  // ahb read 
	//assign	HRDATA = memory[word_address];
	logic [31:0] read_data;
	assign HRDATA = read_data;

	always_ff @(posedge HCLK, negedge HRESETn)
		if (!HRESETn)
			read_data <= '0;
		else
			read_data <= memory[word_address];
	
	
  // VGA reads
	logic [8:0] mem_location;
	
	always_comb
		begin
			mem_location = in_sprite ? (rel_y * 16) + rel_x + 1 : '0;
		end    
		
  assign sprite_pixel = in_sprite ? memory[mem_location] : '0;
	assign sprite_data = memory[0];

//Transfer Response
  assign HREADYOUT = '1; //Single cycle Write & Read. Zero Wait state operations

endmodule
