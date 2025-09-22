`timescale 1ns/1ps

module tb_timer;

  // Clock and reset
  logic HCLK = 0;
  logic HRESETn;

  // AHB inputs
  logic [31:0] HADDR;
  logic [31:0] HWDATA;
  logic [2:0]  HSIZE;
  logic [1:0]  HTRANS;
  logic        HWRITE;
  logic        HREADY;
  logic        HSEL;

  // AHB outputs
  logic [31:0] HRDATA;
  logic        HREADYOUT;

  // output
  logic done;

  // Clock generation: 50MHz = 20ns period
  always #10 HCLK = ~HCLK;

  // DUT instantiation
  timer dut (
    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS,
    .HWRITE, .HREADY, .HSEL, .HRDATA, .HREADYOUT, .done
  );

  initial begin
    // Initialize inputs
    HADDR   = 0;
    HWDATA  = 0;
    HSIZE   = 3'b000;
    HTRANS  = 2'b00;
    HWRITE  = 0;
    HREADY  = 1;
    HSEL    = 0;
    HRESETn = 0;

    // Apply reset
		HRESETn = 0;
    #50;
    HRESETn = 1;

    #(20_000_000);

    #(20_000_000);
		
    $stop;
  end

endmodule
