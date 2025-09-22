//
// Input interface for button press
// Button input for sprite movement
// Register button_pressed clears once accessed by processor
//
// Note: if the button is not 0 at a posedge of clock it wont get registered
// Right now it clears after one clock cycle, but probably better after a read

module button_input(

  // AHB Global Signals
  input HCLK,
  input HRESETn,

  // AHB Signals from Master to Slave
  input [31:0] HADDR, // With this interface HADDR is ignored
  input [31:0] HWDATA,
  input [2:0] HSIZE,
  input [1:0] HTRANS,
  input HWRITE,
  input HREADY,
  input HSEL,

  // AHB Signals from Slave to Master
  output logic [31:0] HRDATA,
  output HREADYOUT,

  // Non-AHB Signals
  input [3:0] buttons,
	
	output logic [3:0] buttons_out

);

timeunit 1ns;
timeprecision 100ps;

  // AHB transfer codes needed in this module
  localparam No_Transfer = 2'b0;
	
	// Extra button signal checks for release (for a single press)
	logic [3:0] last_buttons;
	
	logic button_pressed;
	
  //control signals are stored in registers
  logic read_enable;
	// word_address
	
	always_ff @(posedge HCLK, negedge HRESETn)
		if (!HRESETn)
			begin
				button_pressed <= '0;
				last_buttons <= '0;
			end
		else
			begin
				if ( buttons && !last_buttons ) // Once button is released after pressed
					begin
						button_pressed <= 1;
					end
				if ( read_enable )
					begin
						button_pressed <= 0;
						buttons_out <= '0;
					end
				last_buttons <= buttons;
				buttons_out <= buttons; // ~

			end
				
	
  //Generate the control signals in the address phase
  always_ff @(posedge HCLK, negedge HRESETn)
    if ( ! HRESETn )
      begin
        read_enable <= '0;
      end
    else if ( HREADY && HSEL && (HTRANS != No_Transfer) )
      begin
        read_enable <= ! HWRITE;
			end
    else
      begin
        read_enable <= '0;
     end
		 
  // read
  always_comb
    if ( ! read_enable )
      HRDATA = '0;
    else
			HRDATA = buttons_out;

  //Transfer Response
  assign HREADYOUT = '1;
	
	//Output
//	assign buttons_out = ~buttons;
	
endmodule