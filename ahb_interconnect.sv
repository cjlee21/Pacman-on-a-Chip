module ahb_interconnect #(
  parameter num_slaves = 8
)(
  // global signals
  input HCLK,
  input HRESETn,
   
  // input signals from master
  input [31:0] HADDR,
  
  // output signals to slaves
  output logic [num_slaves-1:0] HSEL_SIGNALS,


  // input signals from slaves
  input [num_slaves-1:0] HREADYOUT_SIGNALS,
  input [num_slaves-1:0][31:0] HRDATA_SIGNALS,

  // output signals to master
  output logic HREADY,
  output logic [31:0] HRDATA

);

timeunit 1ns;
timeprecision 100ps;

  logic [num_slaves-1:0] mux_sel;
  int i;


  //-------------------------
  // to customize this module for a different address map, change only this decoder part and the "num_slaves" parameter
  
  // this example decoder uses minimal decoding
  // (each slave has multiple images in the address map and there are no unmapped regions )
  always_comb
    if ( HADDR < 32'h1000_0000 )
      HSEL_SIGNALS = 1 << 0; //ROM 0000_0000 - 1000_3FFF
		else if ( HADDR < 32'h3000_0000 )
			HSEL_SIGNALS = 1 << 1; //RAM 2000_0000 - 2000_3FFF
		else if ( HADDR < 32'h4000_0000 )
			HSEL_SIGNALS = 1 << 2; //Pacman RAM 3000_0000 - 3000_3FFF
    else if ( HADDR < 32'h5000_0000 )
      HSEL_SIGNALS = 1 << 3;//Ghost RAM 4000_0000 - 4000__3FFF
		else if ( HADDR < 32'h6000_0000 )
			HSEL_SIGNALS = 1 << 4; //Joystick 5000_0000 - 5000__3FFF
		else if ( HADDR < 32'h7000_0000 )
			HSEL_SIGNALS = 1 << 5; //Char ROM 6000_0000 - 6000__3FFF
		else if ( HADDR < 32'h8000_0000 )                          
			HSEL_SIGNALS = 1 << 6; //VID_MEM 7000_0000 - 7000__3FFF	
		else //if ( HADDR < 32'h9000_0000 )                          
			HSEL_SIGNALS = 1 << 7; //TIMER 8000_0000 - 8000__3FFF

  
  //-------------------------
  // the code below should work for any number of slaves

  always_ff @(posedge HCLK, negedge HRESETn)
    if( ! HRESETn )
      mux_sel <= '0;
    else if( HREADY )
      mux_sel <= HSEL_SIGNALS;

  always_comb
    begin
      // default values
      HREADY = 1;
      HRDATA = 32'hDEADBEEF; // "hexspeak" to indicate an error has occured
      
      // since num_slaves is a parameter all of this should be unrolled at compile time
      
      for ( i = 0; i < num_slaves; i++ )
        if ( mux_sel == (1 << i) )
          begin
            HREADY = HREADYOUT_SIGNALS[i];
            HRDATA = HRDATA_SIGNALS[i];
          end

    end

endmodule