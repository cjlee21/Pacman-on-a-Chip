
// 8x8 characters

module video_mem #(
  parameter MEMWIDTH = 13
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
    output logic [31:0] HRDATA,
	
		//Functionality
		input [5:0] row, col,
		output logic [7:0] char
);

// if we get row and column from adapted counter module we output identify character
// character rom will then be accessed and (again using counter module) we will pop out the pixel
// data

// memory needs to store a 40x30 array with character encoding

timeunit 1ns;
timeprecision 100ps;

  localparam No_Transfer = 2'b0;

	logic [7:0] memory [0:1199];	// TOP LEFT - BOT RIGHT
	
/* 	initial begin
		$readmemh("vidmem_init.mem", memory);
	end */
	
	wire [5:0] visible_row;
	wire [5:0] visible_col;
	logic [10:0] index;
	
	// maybe include this check within the always_ff block so you can clear char
	assign visible_row = (row < 30) ? row : 45;	//this gives 1845 outside of full range
	assign visible_col = (col < 40) ? col : 45;	
	
	always_comb
		index = visible_col + ( 40 * visible_row );

	//control signals are stored in registers
  logic write_enable, read_enable;
  logic [MEMWIDTH-3:0] word_address;

//Generate the control signals in the address phase
  always_ff @(posedge HCLK, negedge HRESETn)
    if (! HRESETn )
      begin
        write_enable <= '0;
        read_enable <= '0;
        word_address <= '0;
      end
    else if ( HREADY && HSEL && (HTRANS != No_Transfer) )
      begin
        write_enable <= HWRITE;
				read_enable <= !HWRITE;
        word_address <= HADDR[MEMWIDTH-1:2];
     end
    else
      begin
        write_enable <= '0;
        read_enable <= '0;
        word_address <= '0;
     end

//Act on control signals in the data phase

  // write
  always_ff @(posedge HCLK)
    if ( write_enable )
			memory[word_address] <= HWDATA[7:0];
	
	// read
	always_comb
		if (!read_enable)
			HRDATA = '0;
		else
			HRDATA = memory[word_address];
	
	//Transfer Response
  assign HREADYOUT = '1; //Single cycle Write & Read. Zero Wait state operations
	
  // char assignments
  
  always_ff @(posedge HCLK, negedge HRESETn)
		if (!HRESETn)
			begin
				char <= '0;
			end
		else if ( index < 1200 ) //max value 1199
			begin
			char <= memory[index][7:0];
			end
		else if ( row > 29 | col > 39 )
			begin
				char <= '0;
			end

endmodule