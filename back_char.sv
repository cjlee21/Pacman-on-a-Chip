
	// Character ROM
	
	module back_char#(
  parameter MEMWIDTH = 14
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
		
		// Other Signals
		input [7:0] char,
		input [4:0] square_x, square_y,
		output logic pixel
	);
	
	logic [7:0] memory [0:2047]; //256 characters, 8 rows of 8 pixels per
															
																
	initial begin
	
	  int i;
    for (i = 0; i < 2048; i++) begin
      memory[i] = '0;
		end
		
		
		//'A'
		memory[(65 * 8) + 0 ]  = 8'b00011000;
    memory[(65 * 8) + 1 ]  = 8'b00111100;
    memory[(65 * 8) + 2 ]  = 8'b01100110;
    memory[(65 * 8) + 3 ]  = 8'b11000011;
    memory[(65 * 8) + 4 ]  = 8'b11111111;
    memory[(65 * 8) + 5 ]  = 8'b11000011;
    memory[(65 * 8) + 6 ]  = 8'b11000011;
    memory[(65 * 8) + 7 ]  = 8'b11000011;

		//'0'
		memory[(48 * 8) + 0 ]  = 8'b00111100;
    memory[(48 * 8) + 1 ]  = 8'b01000010;
    memory[(48 * 8) + 2 ]  = 8'b10000101;
    memory[(48 * 8) + 3 ]  = 8'b10001001;
    memory[(48 * 8) + 4 ]  = 8'b10010001;
    memory[(48 * 8) + 5 ]  = 8'b10100001;
    memory[(48 * 8) + 6 ]  = 8'b01000010;
    memory[(48 * 8) + 7 ]  = 8'b00111100;
		
		//'1'
		memory[(49 * 8) + 0 ]  = 8'b00110000;
    memory[(49 * 8) + 1 ]  = 8'b01010000;
    memory[(49 * 8) + 2 ]  = 8'b00010000;
    memory[(49 * 8) + 3 ]  = 8'b00010000;
    memory[(49 * 8) + 4 ]  = 8'b00010000;
    memory[(49 * 8) + 5 ]  = 8'b00010000;
    memory[(49 * 8) + 6 ]  = 8'b00010000;
    memory[(49 * 8) + 7 ]  = 8'b11111111;
		
		//'2'
		memory[(50 * 8) + 0 ]  = 8'b01111110;
    memory[(50 * 8) + 1 ]  = 8'b10000001;
    memory[(50 * 8) + 2 ]  = 8'b00000001;
    memory[(50 * 8) + 3 ]  = 8'b00000110;
    memory[(50 * 8) + 4 ]  = 8'b00011000;
    memory[(50 * 8) + 5 ]  = 8'b01100000;
    memory[(50 * 8) + 6 ]  = 8'b10000001;
    memory[(50 * 8) + 7 ]  = 8'b11111111;

		//'3'
		memory[(51 * 8) + 0 ]  = 8'b01111110;
    memory[(51 * 8) + 1 ]  = 8'b10000001;
    memory[(51 * 8) + 2 ]  = 8'b10000001;
    memory[(51 * 8) + 3 ]  = 8'b00001110;
    memory[(51 * 8) + 4 ]  = 8'b00000001;
    memory[(51 * 8) + 5 ]  = 8'b10000001;
    memory[(51 * 8) + 6 ]  = 8'b10000001;
    memory[(51 * 8) + 7 ]  = 8'b01111110;
		
		//'4'
		memory[(52 * 8) + 0 ]  = 8'b00000110;
    memory[(52 * 8) + 1 ]  = 8'b00001010;
    memory[(52 * 8) + 2 ]  = 8'b00010010;
    memory[(52 * 8) + 3 ]  = 8'b00100010;
    memory[(52 * 8) + 4 ]  = 8'b01000010;
    memory[(52 * 8) + 5 ]  = 8'b11111111;
    memory[(52 * 8) + 6 ]  = 8'b00000010;
    memory[(52 * 8) + 7 ]  = 8'b00000111;	
	
		//'5'
		memory[(53 * 8) + 0 ]  = 8'b11111111;
    memory[(53 * 8) + 1 ]  = 8'b10000001;
    memory[(53 * 8) + 2 ]  = 8'b10000000;
    memory[(53 * 8) + 3 ]  = 8'b11111110;
    memory[(53 * 8) + 4 ]  = 8'b00000001;
    memory[(53 * 8) + 5 ]  = 8'b10000001;
    memory[(53 * 8) + 6 ]  = 8'b10000001;
    memory[(53 * 8) + 7 ]  = 8'b01111110;
		
		//'6'
		memory[(54 * 8) + 0 ]  = 8'b01111110;
    memory[(54 * 8) + 1 ]  = 8'b10000001;
    memory[(54 * 8) + 2 ]  = 8'b10000000;
    memory[(54 * 8) + 3 ]  = 8'b11111110;
    memory[(54 * 8) + 4 ]  = 8'b10000001;
    memory[(54 * 8) + 5 ]  = 8'b10000001;
    memory[(54 * 8) + 6 ]  = 8'b10000001;
    memory[(54 * 8) + 7 ]  = 8'b01111110;			
		
		//'7'
		memory[(55 * 8) + 0 ]  = 8'b11111111;
    memory[(55 * 8) + 1 ]  = 8'b10000001;
    memory[(55 * 8) + 2 ]  = 8'b00000010;
    memory[(55 * 8) + 3 ]  = 8'b00000100;
    memory[(55 * 8) + 4 ]  = 8'b00001000;
    memory[(55 * 8) + 5 ]  = 8'b00010000;
    memory[(55 * 8) + 6 ]  = 8'b00010000;
    memory[(55 * 8) + 7 ]  = 8'b00111000;

		//'8'
		memory[(56 * 8) + 0 ]  = 8'b01111110;
    memory[(56 * 8) + 1 ]  = 8'b10000001;
    memory[(56 * 8) + 2 ]  = 8'b10000001;
    memory[(56 * 8) + 3 ]  = 8'b01111110;
    memory[(56 * 8) + 4 ]  = 8'b10000001;
    memory[(56 * 8) + 5 ]  = 8'b10000001;
    memory[(56 * 8) + 6 ]  = 8'b10000001;
    memory[(56 * 8) + 7 ]  = 8'b01111110;	

		//'9'
		memory[(57 * 8) + 0 ]  = 8'b01111110;
    memory[(57 * 8) + 1 ]  = 8'b10000001;
    memory[(57 * 8) + 2 ]  = 8'b10000001;
    memory[(57 * 8) + 3 ]  = 8'b10000001;
    memory[(57 * 8) + 4 ]  = 8'b01111111;
    memory[(57 * 8) + 5 ]  = 8'b00000001;
    memory[(57 * 8) + 6 ]  = 8'b10000001;
    memory[(57 * 8) + 7 ]  = 8'b01111110;				
		
		//Border Top Left Corner
		memory[(1 * 8) + 0 ]  = 8'b00000000;
    memory[(1 * 8) + 1 ]  = 8'b00000000;
    memory[(1 * 8) + 2 ]  = 8'b00000000;
    memory[(1 * 8) + 3 ]  = 8'b00000000;
    memory[(1 * 8) + 4 ]  = 8'b00000011;
    memory[(1 * 8) + 5 ]  = 8'b00000100;
    memory[(1 * 8) + 6 ]  = 8'b00001001;
    memory[(1 * 8) + 7 ]  = 8'b00001010;
		
		//Border Top Horizontal
		memory[(2 * 8) + 0 ]  = 8'b00000000;
    memory[(2 * 8) + 1 ]  = 8'b00000000;
    memory[(2 * 8) + 2 ]  = 8'b00000000;
    memory[(2 * 8) + 3 ]  = 8'b00000000;
    memory[(2 * 8) + 4 ]  = 8'b11111111;
    memory[(2 * 8) + 5 ]  = 8'b00000000;
    memory[(2 * 8) + 6 ]  = 8'b11111111;
    memory[(2 * 8) + 7 ]  = 8'b00000000;
		
		//Border Top Right Corner
		memory[(3 * 8) + 0 ]  = 8'b00000000;
    memory[(3 * 8) + 1 ]  = 8'b00000000;
    memory[(3 * 8) + 2 ]  = 8'b00000000;
    memory[(3 * 8) + 3 ]  = 8'b00000000;
    memory[(3 * 8) + 4 ]  = 8'b11000000;
    memory[(3 * 8) + 5 ]  = 8'b00100000;
    memory[(3 * 8) + 6 ]  = 8'b10010000;
    memory[(3 * 8) + 7 ]  = 8'b01010000;		
		
		//Border Left Vertical
		memory[(4 * 8) + 0 ]  = 8'b00001010;
    memory[(4 * 8) + 1 ]  = 8'b00001010;
    memory[(4 * 8) + 2 ]  = 8'b00001010;
    memory[(4 * 8) + 3 ]  = 8'b00001010;
    memory[(4 * 8) + 4 ]  = 8'b00001010;
    memory[(4 * 8) + 5 ]  = 8'b00001010;
    memory[(4 * 8) + 6 ]  = 8'b00001010;
    memory[(4 * 8) + 7 ]  = 8'b00001010;

		//Border Bottom Right Corner
		memory[(5 * 8) + 0 ]  = 8'b01010000;
    memory[(5 * 8) + 1 ]  = 8'b10010000;
    memory[(5 * 8) + 2 ]  = 8'b00100000;
    memory[(5 * 8) + 3 ]  = 8'b11000000;
    memory[(5 * 8) + 4 ]  = 8'b00000000;
    memory[(5 * 8) + 5 ]  = 8'b00000000;
    memory[(5 * 8) + 6 ]  = 8'b00000000;
    memory[(5 * 8) + 7 ]  = 8'b00000000;
		
		//Border Bottom Left Corner
		memory[(6 * 8) + 0 ]  = 8'b00001010;
    memory[(6 * 8) + 1 ]  = 8'b00001001;
    memory[(6 * 8) + 2 ]  = 8'b00000100;
    memory[(6 * 8) + 3 ]  = 8'b00000011;
    memory[(6 * 8) + 4 ]  = 8'b00000000;
    memory[(6 * 8) + 5 ]  = 8'b00000000;
    memory[(6 * 8) + 6 ]  = 8'b00000000;
    memory[(6 * 8) + 7 ]  = 8'b00000000;

		//Border Bottom Horizontal
		memory[(7 * 8) + 0 ]  = 8'b00000000;
    memory[(7 * 8) + 1 ]  = 8'b11111111;
    memory[(7 * 8) + 2 ]  = 8'b00000000;
    memory[(7 * 8) + 3 ]  = 8'b11111111;
    memory[(7 * 8) + 4 ]  = 8'b00000000;
    memory[(7 * 8) + 5 ]  = 8'b00000000;
    memory[(7 * 8) + 6 ]  = 8'b00000000;
    memory[(7 * 8) + 7 ]  = 8'b00000000;

		//Border Right Vertical
		memory[(8 * 8) + 0 ]  = 8'b01010000;
    memory[(8 * 8) + 1 ]  = 8'b01010000;
    memory[(8 * 8) + 2 ]  = 8'b01010000;
    memory[(8 * 8) + 3 ]  = 8'b01010000;
    memory[(8 * 8) + 4 ]  = 8'b01010000;
    memory[(8 * 8) + 5 ]  = 8'b01010000;
    memory[(8 * 8) + 6 ]  = 8'b01010000;
    memory[(8 * 8) + 7 ]  = 8'b01010000;
		
		//Border Bottom Centre
		memory[(9 * 8) + 0 ]  = 8'b01000010;
    memory[(9 * 8) + 1 ]  = 8'b01000010;
    memory[(9 * 8) + 2 ]  = 8'b01000010;
    memory[(9 * 8) + 3 ]  = 8'b01000010;
    memory[(9 * 8) + 4 ]  = 8'b11000011;
    memory[(9 * 8) + 5 ]  = 8'b00000000;
    memory[(9 * 8) + 6 ]  = 8'b11111111;
    memory[(9 * 8) + 7 ]  = 8'b00000000;

		//Border Top Centre
		memory[(10 * 8) + 0 ]  = 8'b00000000;
    memory[(10 * 8) + 1 ]  = 8'b00000000;
    memory[(10 * 8) + 2 ]  = 8'b00000000;
    memory[(10 * 8) + 3 ]  = 8'b00000000;
    memory[(10 * 8) + 4 ]  = 8'b11111111;
    memory[(10 * 8) + 5 ]  = 8'b00000000;
    memory[(10 * 8) + 6 ]  = 8'b11000011;
    memory[(10 * 8) + 7 ]  = 8'b01000010;
		
		//Wall Top Left
		memory[(11 * 8) + 0 ]  = 8'b00000000;
    memory[(11 * 8) + 1 ]  = 8'b00111111;
    memory[(11 * 8) + 2 ]  = 8'b01100000;
    memory[(11 * 8) + 3 ]  = 8'b01000000;
    memory[(11 * 8) + 4 ]  = 8'b01000000;
    memory[(11 * 8) + 5 ]  = 8'b01000000;
    memory[(11 * 8) + 6 ]  = 8'b01000000;
    memory[(11 * 8) + 7 ]  = 8'b01000000;

		//Wall Top Horizontal
		memory[(12 * 8) + 0 ]  = 8'b00000000;
    memory[(12 * 8) + 1 ]  = 8'b11111111;
    memory[(12 * 8) + 2 ]  = 8'b00000000;
    memory[(12 * 8) + 3 ]  = 8'b00000000;
    memory[(12 * 8) + 4 ]  = 8'b00000000;
    memory[(12 * 8) + 5 ]  = 8'b00000000;
    memory[(12 * 8) + 6 ]  = 8'b00000000;
    memory[(12 * 8) + 7 ]  = 8'b00000000;

		//Wall Top Right
		memory[(13 * 8) + 0 ]  = 8'b00000000;
    memory[(13 * 8) + 1 ]  = 8'b11111100;
    memory[(13 * 8) + 2 ]  = 8'b00000110;
    memory[(13 * 8) + 3 ]  = 8'b00000010;
    memory[(13 * 8) + 4 ]  = 8'b00000010;
    memory[(13 * 8) + 5 ]  = 8'b00000010;
    memory[(13 * 8) + 6 ]  = 8'b00000010;
    memory[(13 * 8) + 7 ]  = 8'b00000010;
		
		//Wall Right Vertical
		memory[(14 * 8) + 0 ]  = 8'b00000010;
    memory[(14 * 8) + 1 ]  = 8'b00000010;
    memory[(14 * 8) + 2 ]  = 8'b00000010;
    memory[(14 * 8) + 3 ]  = 8'b00000010;
    memory[(14 * 8) + 4 ]  = 8'b00000010;
    memory[(14 * 8) + 5 ]  = 8'b00000010;
    memory[(14 * 8) + 6 ]  = 8'b00000010;
    memory[(14 * 8) + 7 ]  = 8'b00000010;
		
		//Wall Bottom Right
		memory[(15 * 8) + 0 ]  = 8'b00000010;
    memory[(15 * 8) + 1 ]  = 8'b00000010;
    memory[(15 * 8) + 2 ]  = 8'b00000010;
    memory[(15 * 8) + 3 ]  = 8'b00000010;
    memory[(15 * 8) + 4 ]  = 8'b00000010;
    memory[(15 * 8) + 5 ]  = 8'b00000110;
    memory[(15 * 8) + 6 ]  = 8'b11111100;
    memory[(15 * 8) + 7 ]  = 8'b00000000;

		//Wall Bottom Horizontal
		memory[(16 * 8) + 0 ]  = 8'b00000000;
    memory[(16 * 8) + 1 ]  = 8'b00000000;
    memory[(16 * 8) + 2 ]  = 8'b00000000;
    memory[(16 * 8) + 3 ]  = 8'b00000000;
    memory[(16 * 8) + 4 ]  = 8'b00000000;
    memory[(16 * 8) + 5 ]  = 8'b00000000;
    memory[(16 * 8) + 6 ]  = 8'b11111111;
    memory[(16 * 8) + 7 ]  = 8'b00000000;
		
		// Wall Left Vertical
		memory[(17 * 8) + 0 ]  = 8'b01000000;
    memory[(17 * 8) + 1 ]  = 8'b01000000;
    memory[(17 * 8) + 2 ]  = 8'b01000000;
    memory[(17 * 8) + 3 ]  = 8'b01000000;
    memory[(17 * 8) + 4 ]  = 8'b01000000;
    memory[(17 * 8) + 5 ]  = 8'b01000000;
    memory[(17 * 8) + 6 ]  = 8'b01000000;
    memory[(17 * 8) + 7 ]  = 8'b01000000;
		
		// Wall Thin-Horizontal Left
		memory[(18 * 8) + 0 ]  = 8'b00000000;
    memory[(18 * 8) + 1 ]  = 8'b00111111;
    memory[(18 * 8) + 2 ]  = 8'b01100000;
    memory[(18 * 8) + 3 ]  = 8'b01000000;
    memory[(18 * 8) + 4 ]  = 8'b01000000;
    memory[(18 * 8) + 5 ]  = 8'b01100000;
    memory[(18 * 8) + 6 ]  = 8'b00111111;
    memory[(18 * 8) + 7 ]  = 8'b00000000;

		// Wall Thin-Horizontal
		memory[(27 * 8) + 0 ]  = 8'b00000000;
    memory[(27 * 8) + 1 ]  = 8'b11111111;
    memory[(27 * 8) + 2 ]  = 8'b00000000;
    memory[(27 * 8) + 3 ]  = 8'b00000000;
    memory[(27 * 8) + 4 ]  = 8'b00000000;
    memory[(27 * 8) + 5 ]  = 8'b00000000;
    memory[(27 * 8) + 6 ]  = 8'b11111111;
    memory[(27 * 8) + 7 ]  = 8'b00000000;

		// Wall Thin-Horizontal Right
		memory[(19 * 8) + 0 ]  = 8'b00000000;
    memory[(19 * 8) + 1 ]  = 8'b11111100;
    memory[(19 * 8) + 2 ]  = 8'b00000110;
    memory[(19 * 8) + 3 ]  = 8'b00000010;
    memory[(19 * 8) + 4 ]  = 8'b00000010;
    memory[(19 * 8) + 5 ]  = 8'b00000110;
    memory[(19 * 8) + 6 ]  = 8'b11111100;
    memory[(19 * 8) + 7 ]  = 8'b00000000;
		
		// Wall Thin-Vertical Top
		memory[(20 * 8) + 0 ]  = 8'b00000000;
    memory[(20 * 8) + 1 ]  = 8'b00111100;
    memory[(20 * 8) + 2 ]  = 8'b01100110;
    memory[(20 * 8) + 3 ]  = 8'b01000010;
    memory[(20 * 8) + 4 ]  = 8'b01000010;
    memory[(20 * 8) + 5 ]  = 8'b01000010;
    memory[(20 * 8) + 6 ]  = 8'b01000010;
    memory[(20 * 8) + 7 ]  = 8'b01000010;
		
		// Wall Thin-Vertical
		memory[(21 * 8) + 0 ]  = 8'b01000010;
    memory[(21 * 8) + 1 ]  = 8'b01000010;
    memory[(21 * 8) + 2 ]  = 8'b01000010;
    memory[(21 * 8) + 3 ]  = 8'b01000010;
    memory[(21 * 8) + 4 ]  = 8'b01000010;
    memory[(21 * 8) + 5 ]  = 8'b01000010;
    memory[(21 * 8) + 6 ]  = 8'b01000010;
    memory[(21 * 8) + 7 ]  = 8'b01000010;
		
		// Wall Thin-Vertical Bottom
		memory[(22 * 8) + 0 ]  = 8'b01000010;
    memory[(22 * 8) + 1 ]  = 8'b01000010;
    memory[(22 * 8) + 2 ]  = 8'b01000010;
    memory[(22 * 8) + 3 ]  = 8'b01000010;
    memory[(22 * 8) + 4 ]  = 8'b01000010;
    memory[(22 * 8) + 5 ]  = 8'b01100110;
    memory[(22 * 8) + 6 ]  = 8'b00111100;
    memory[(22 * 8) + 7 ]  = 8'b00000000;
		
		// Wall Thin-Vertical Right-Aiming Split
		memory[(23 * 8) + 0 ]  = 8'b01000010;
    memory[(23 * 8) + 1 ]  = 8'b01000011;
    memory[(23 * 8) + 2 ]  = 8'b01000000;
    memory[(23 * 8) + 3 ]  = 8'b01000000;
    memory[(23 * 8) + 4 ]  = 8'b01000000;
    memory[(23 * 8) + 5 ]  = 8'b01000000;
    memory[(23 * 8) + 6 ]  = 8'b01000011;
    memory[(23 * 8) + 7 ]  = 8'b01000010;
		
		// Wall Thin-Vertical Left-Aiming Split
		memory[(24 * 8) + 0 ]  = 8'b01000010;
    memory[(24 * 8) + 1 ]  = 8'b11000010;
    memory[(24 * 8) + 2 ]  = 8'b00000010;
    memory[(24 * 8) + 3 ]  = 8'b00000010;
    memory[(24 * 8) + 4 ]  = 8'b00000010;
    memory[(24 * 8) + 5 ]  = 8'b00000010;
    memory[(24 * 8) + 6 ]  = 8'b11000010;
    memory[(24 * 8) + 7 ]  = 8'b01000010;
		
		// Wall Thin-Horizontal Down-Aiming Split
		memory[(25 * 8) + 0 ]  = 8'b00000000;
    memory[(25 * 8) + 1 ]  = 8'b11111111;
    memory[(25 * 8) + 2 ]  = 8'b00000000;
    memory[(25 * 8) + 3 ]  = 8'b00000000;
    memory[(25 * 8) + 4 ]  = 8'b00000000;
    memory[(25 * 8) + 5 ]  = 8'b00000000;
    memory[(25 * 8) + 6 ]  = 8'b11000011;
    memory[(25 * 8) + 7 ]  = 8'b01000010;
		
		// Wall Thin-Vertical Up-Aiming Split
		memory[(26 * 8) + 0 ]  = 8'b01000010;
    memory[(26 * 8) + 1 ]  = 8'b11000011;
    memory[(26 * 8) + 2 ]  = 8'b00000000;
    memory[(26 * 8) + 3 ]  = 8'b00000000;
    memory[(26 * 8) + 4 ]  = 8'b00000000;
    memory[(26 * 8) + 5 ]  = 8'b00000000;
    memory[(26 * 8) + 6 ]  = 8'b11111111;
    memory[(26 * 8) + 7 ]  = 8'b00000000;
		
		// Border Right Side T-Shape
		memory[(28 * 8) + 0 ]  = 8'b01010000;
    memory[(28 * 8) + 1 ]  = 8'b11010000;
    memory[(28 * 8) + 2 ]  = 8'b00010000;
    memory[(28 * 8) + 3 ]  = 8'b00010000;
    memory[(28 * 8) + 4 ]  = 8'b00010000;
    memory[(28 * 8) + 5 ]  = 8'b00010000;
    memory[(28 * 8) + 6 ]  = 8'b11010000;
    memory[(28 * 8) + 7 ]  = 8'b01010000;	
		
		// Border Left Side T-Shape
		memory[(29 * 8) + 0 ]  = 8'b00001010;
    memory[(29 * 8) + 1 ]  = 8'b00001011;
    memory[(29 * 8) + 2 ]  = 8'b00001000;
    memory[(29 * 8) + 3 ]  = 8'b00001000;
    memory[(29 * 8) + 4 ]  = 8'b00001000;
    memory[(29 * 8) + 5 ]  = 8'b00001000;
    memory[(29 * 8) + 6 ]  = 8'b00001011;
    memory[(29 * 8) + 7 ]  = 8'b00001010;

		// Wall Thin LD Corner
		memory[(30 * 8) + 0 ]  = 8'b00000000;
    memory[(30 * 8) + 1 ]  = 8'b11111100;
    memory[(30 * 8) + 2 ]  = 8'b00000110;
    memory[(30 * 8) + 3 ]  = 8'b00000010;
    memory[(30 * 8) + 4 ]  = 8'b00000010;
    memory[(30 * 8) + 5 ]  = 8'b00000010;
    memory[(30 * 8) + 6 ]  = 8'b11000010;
    memory[(30 * 8) + 7 ]  = 8'b01000010;		
		
		// Wall Thin RD Corner
		memory[(31 * 8) + 0 ]  = 8'b00000000;
    memory[(31 * 8) + 1 ]  = 8'b00111111;
    memory[(31 * 8) + 2 ]  = 8'b01100000;
    memory[(31 * 8) + 3 ]  = 8'b01000000;
    memory[(31 * 8) + 4 ]  = 8'b01000000;
    memory[(31 * 8) + 5 ]  = 8'b01000000;
    memory[(31 * 8) + 6 ]  = 8'b01000011;
    memory[(31 * 8) + 7 ]  = 8'b01000010;
		
		//Wall Bottom Left
		memory[(32 * 8) + 0 ]  = 8'b01000000;
    memory[(32 * 8) + 1 ]  = 8'b01000000;
    memory[(32 * 8) + 2 ]  = 8'b01000000;
    memory[(32 * 8) + 3 ]  = 8'b01000000;
    memory[(32 * 8) + 4 ]  = 8'b01000000;
    memory[(32 * 8) + 5 ]  = 8'b01100000;
    memory[(32 * 8) + 6 ]  = 8'b00111111;
    memory[(32 * 8) + 7 ]  = 8'b00000000;
		
		//Border Top Right Corner Type 2
		memory[(33 * 8) + 0 ]  = 8'b00000000;
    memory[(33 * 8) + 1 ]  = 8'b11111000;
    memory[(33 * 8) + 2 ]  = 8'b00000100;
    memory[(33 * 8) + 3 ]  = 8'b11110010;
    memory[(33 * 8) + 4 ]  = 8'b00001010;
    memory[(33 * 8) + 5 ]  = 8'b00001010;
    memory[(33 * 8) + 6 ]  = 8'b00001010;
    memory[(33 * 8) + 7 ]  = 8'b00001010;

		//Border Bottom Right Corner Type 2
		memory[(34 * 8) + 0 ]  = 8'b00001010;
    memory[(34 * 8) + 1 ]  = 8'b00001010;
    memory[(34 * 8) + 2 ]  = 8'b00001010;
    memory[(34 * 8) + 3 ]  = 8'b00001010;
    memory[(34 * 8) + 4 ]  = 8'b11110010;
    memory[(34 * 8) + 5 ]  = 8'b00000100;
    memory[(34 * 8) + 6 ]  = 8'b11111000;
    memory[(34 * 8) + 7 ]  = 8'b00000000;

		//Border Top Left Corner Type 2
		memory[(35 * 8) + 0 ]  = 8'b00000000;
    memory[(35 * 8) + 1 ]  = 8'b00011111;
    memory[(35 * 8) + 2 ]  = 8'b00100000;
    memory[(35 * 8) + 3 ]  = 8'b01001111;
    memory[(35 * 8) + 4 ]  = 8'b01010000;
    memory[(35 * 8) + 5 ]  = 8'b01010000;
    memory[(35 * 8) + 6 ]  = 8'b01010000;
    memory[(35 * 8) + 7 ]  = 8'b01010000;

		//Border Bottom Left Corner Type 2
		memory[(36 * 8) + 0 ]  = 8'b01010000;
    memory[(36 * 8) + 1 ]  = 8'b01010000;
    memory[(36 * 8) + 2 ]  = 8'b01010000;
    memory[(36 * 8) + 3 ]  = 8'b01010000;
    memory[(36 * 8) + 4 ]  = 8'b01001111;
    memory[(36 * 8) + 5 ]  = 8'b00100000;
    memory[(36 * 8) + 6 ]  = 8'b00011111;
    memory[(36 * 8) + 7 ]  = 8'b00000000;
		
		//Cherry 0x50
		memory[(80 * 8) + 0 ]  = 8'b00000000;
    memory[(80 * 8) + 1 ]  = 8'b00000000;
    memory[(80 * 8) + 2 ]  = 8'b00000000;
    memory[(80 * 8) + 3 ]  = 8'b00011000;
    memory[(80 * 8) + 4 ]  = 8'b00011000;
    memory[(80 * 8) + 5 ]  = 8'b00000000;
    memory[(80 * 8) + 6 ]  = 8'b00000000;
    memory[(80 * 8) + 7 ]  = 8'b00000000;		
		
		//Cherry 0x51
		memory[(81 * 8) + 0 ]  = 8'b00000000;
    memory[(81 * 8) + 1 ]  = 8'b10100101;
    memory[(81 * 8) + 2 ]  = 8'b00000000;
    memory[(81 * 8) + 3 ]  = 8'b00000000;
    memory[(81 * 8) + 4 ]  = 8'b00000000;
    memory[(81 * 8) + 5 ]  = 8'b00000000;
    memory[(81 * 8) + 6 ]  = 8'b00000000;
    memory[(81 * 8) + 7 ]  = 8'b00000000;
		
	end

timeunit 1ns;
timeprecision 100ps;

  localparam No_Transfer = 2'b0;

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
			
	//Transfer Response
  assign HREADYOUT = '1; //Single cycle Write & Read. Zero Wait state operations
	
	// no read
	
	// pixel assignments
		
		logic [11:0] char_index;
		logic [4:0] char_x, char_y;
		
		assign char_x = square_x / 2;						// floor division
		assign char_y = square_y / 2;
		assign char_index = char * 8 + char_y;  // locate char in memory

		always_ff @(posedge HCLK, negedge HRESETn)
			if (!HRESETn) begin
				pixel <= '0;
			end
			else begin
				pixel <= memory[char_index][7 - char_x];  // Same bit select
			end

endmodule