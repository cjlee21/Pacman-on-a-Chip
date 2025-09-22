
// Sprite Engine
// Pixel data stored one pixel per memory location
// Requires 256 memory locations

module sprite_engine (

		input logic VGA_CLK,
		input logic HRESETn,
		input logic [9:0] sprite_x, sprite_y,
		input logic [10:0] screen_x, screen_y,
		output logic [3:0] rel_x, rel_y,
		output logic in_sprite
		
  );
				
timeunit 1ns;
timeprecision 10ps;
	 
	logic [3:0] next_rel_x, next_rel_y;
	 
  always_comb
		begin
			next_rel_x = screen_x - sprite_x;
			next_rel_y = screen_y - sprite_y;
		end    
	
  always_ff @(posedge VGA_CLK, negedge HRESETn)
    if ( ! HRESETn)
      begin
				in_sprite <= '0;
				rel_x <= '0;
				rel_y <= '0;
      end
    else
      begin
				if ((screen_x >= sprite_x) && (screen_x <= sprite_x + 15) && 
				(screen_y >= sprite_y) && (screen_y <= sprite_y + 15))
					begin
						in_sprite <= 1;
						rel_x <= next_rel_x;
						rel_y <= next_rel_y;
					end
				else
					in_sprite <= 0;
		  end

endmodule
 
 

 
