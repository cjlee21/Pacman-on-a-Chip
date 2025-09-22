

module joystick_input(

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
  input [3:0] joystick,
	
	output logic [3:0] joystick_out

);

timeunit 1ns;
timeprecision 100ps;

  // AHB transfer codes needed in this module
  localparam No_Transfer = 2'b0;
	
	// Extra button signal checks for release (for a single press)
	logic [3:0] last_joystick;
	
	logic joystick_pressed;
	
  //control signals are stored in registers
  logic read_enable;
	// word_address
	
	always_ff @(posedge HCLK, negedge HRESETn)
		if (!HRESETn)
			begin
				joystick_pressed <= '0;
				last_joystick <= '0;
			end
		else
			begin
				if ( joystick && !last_joystick ) // Once button is released after pressed
					begin
						joystick_pressed <= 1;
					end
				if ( read_enable )
					begin
						joystick_pressed <= 0;
						joystick_out <= '0;
					end
				last_joystick <= joystick;
				joystick_out <= joystick; // ~

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
			HRDATA = joystick_out;

  //Transfer Response
  assign HREADYOUT = '1;
	
	//Output
//	assign joystick_out = ~joystick;
	
endmodule