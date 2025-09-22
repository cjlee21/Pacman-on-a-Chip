// Timer taken from Lab Walkthrough
//
// Address map :
//   Base addess + 0 : 
//     Read-only Status register
//     LSB = 1 when countdown is in effect. LSB = 0 when countdown is complete
//   Base addess + 4 : 
//     Read-write Count output register
//     Writing to count should start countdown. Countdown decrements every 1ms from write.

module timer (

  // AHB Global Signals
  input HCLK,
  input HRESETn,

  // AHB Signals from Master to Slave
  input [31:0] HADDR, // With this interface only HADDR[3:2] is used (other bits are ignored)
  input [31:0] HWDATA,
  input [2:0] HSIZE,
  input [1:0] HTRANS, // describes type of transfer (idle, sequential, non-sequential, busy)
  input HWRITE,
  input HREADY,
  input HSEL,

  // AHB Signals from Slave to Master
  output logic [31:0] HRDATA,
  output HREADYOUT,

  //Non-AHB Signals
  output logic [31:0] count
	
);

timeunit 1ns;
timeprecision 100ps;

  // AHB transfer codes needed in this module
  localparam No_Transfer = 2'b0;

  //control signals are stored in registers
  logic write_enable, read_enable;
  logic word_address;
  logic [31:0] status;
  
  //clock speed is 50MHz, 20ns per cycle. 1ms = 50000 which needs 16 bits to store.
  logic [15:0] clockcounter;
 
  //Generate the control signals in the address phase
  always_ff @(posedge HCLK, negedge HRESETn)
  	if (!HRESETn)				//if reset is hit
	   begin
			 write_enable <= '0;
			 read_enable <= '0;
			 word_address <= '0;
	   end
	 else if (HREADY && HSEL && (HTRANS != No_Transfer))
	   begin
			 write_enable <= HWRITE;
			 read_enable <= !HWRITE;
			 word_address <= HADDR[2];
	   end
	 else
	   begin
			 write_enable <= '0;
			 read_enable <= '0;
			 word_address <= HADDR[2];
	   end
  
	   
  //Act on control signals in the data phase

  // write
  always_ff @(posedge HCLK, negedge HRESETn)
    if (!HRESETn)
       begin
				 count <= '0;
				 status <= '0;
				 clockcounter <= '0;
       end
    else if (write_enable && !status[0]) //if we want to write and we aren't already counting
       begin
				 count <= HWDATA;
				 status <= {31'd0, 1'd1};
				 clockcounter <= 49999; //decreases every 1ms
       end
    else if (status[0]) //if we are counting
      begin
				if (clockcounter == 0)
				begin
					count <= count - 1;
					if (count == '0)
						status <= '0;
					else
						clockcounter <= 49999;
				end
				else
					clockcounter <= clockcounter - 1;
			end


  // read
  always_comb
  	if (!read_enable)
	   HRDATA = '0;
		else
			begin
				case(word_address)
				0: HRDATA = status;
				1: HRDATA = count;
				endcase
			end


  //Transfer Response
  assign HREADYOUT = '1; //Single cycle Write & Read. Zero Wait state operations

endmodule


