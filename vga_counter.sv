
// VGA counters updated for background

module vga_counter (

		input logic HCLK, 
		input logic HRESETn, 
		output logic VGA_HS, VGA_VS, VGA_CLK, VGA_BLANK_N, video_on,
		output logic [10:0] H_count_d1, V_count_d1,
		output logic [10:0] H_count, V_count,
		output logic [4:0] square_x, square_y,
		output logic [5:0] row, col

); 
		
timeunit 1ns;
timeprecision 10ps;

// Video Display Signals    
logic video_on_H, video_on_V, clock_enable; 

  // Map internal signals to external busses
  
  assign VGA_CLK = clock_enable;
  assign VGA_BLANK_N = video_on; //no output (blank) when outside video range

  // video_on turns off pixel color data when not in the pixel view area 
  assign video_on = video_on_H && video_on_V;

  //Generate Horizontal and Vertical Timing Signals for Video Signal 
  //For details see Rapid Prototyping of Digital Systems Chapter 9 
  //VIDEO_DISPLAY 
	
	logic [10:0] H_count_d2, V_count_d2, H_count_d3, V_count_d3;
  
  always_ff @(posedge HCLK, negedge HRESETn)
    if ( ! HRESETn) 
      begin
        clock_enable <= '0; 
        H_count <= '0; 
        V_count <= '0; 
        VGA_HS <= 1; 
        VGA_VS <= 1; 
				
        video_on_H <= '0; 
        video_on_V <= '0;
				
				H_count_d1 <= '0;
				V_count_d1 <= '0;				
				H_count_d2 <= '0;
				V_count_d2 <= '0;
				H_count_d3 <= '0;
				V_count_d3 <= '0;
				
				row <= '0;
				col <= '0;
				
				square_y <= '0;
				square_x <= '0;
      end
    else
      begin

        // H_count counts pixels (640 + extra time for sync signals) 
        // 
        //   <-Clock out RGB Pixel Row Data ->   <-H Sync-> 
        //   ------------------------------------__________-------- 
        //   0                           640   659       755    799 
        // 
        //V_count counts rows of pixels (480 + extra time for sync signals) 
        // 
        //  <---- 480 Horizontal Syncs (pixel rows) -->  ->V Sync<- 
        //  -----------------------------------------------_______------------ 
        //  0                                       480    493-494          524 
        // 

        if ( clock_enable )
          begin 

            //Generate Active-low Horizontal Sync Signal 
            VGA_HS <= ! ((H_count <= 755) && (H_count >= 659)); 

            // Generate Active-low Vertical Sync Signal 
            VGA_VS <= ! ((V_count <= 494) && (V_count >= 493));


            // Generate Video on Screen Signals for Pixel Data 
            video_on_H <= (H_count <= 639); 

            video_on_V <= (V_count <= 479); 

            // Update the counters

            if ((V_count_d3 >= 524) && (H_count_d3 >= 699))
              V_count_d3 <= 0; 
            else if (H_count == 699)						// picks arbitrary number late into the row
              V_count_d3 <= V_count_d2 + 1; 		// incremenets pixel row number at this point

            if (H_count_d3 >= 799)
              H_count_d3 <= 0; 
            else 
              H_count_d3 <= H_count_d2 + 1; 

          end
					
		H_count_d2 <= H_count_d3;
		V_count_d2 <= V_count_d3;
		  
		H_count_d1 <= H_count_d2;
		V_count_d1 <= V_count_d2;
		
		H_count <= H_count_d1;
		V_count <= V_count_d1;

		square_y <= V_count_d2 % 16;
		square_x <= H_count_d2 % 16;
		
		row <= V_count_d3 / 16;   // integer division
		col <= H_count_d3 / 16;

        // Clock enable used for a 25Mhz video clock rate 
        // 640 by 480 display mode expects 25.175Mhz pixel clock 
        // 25Mhz should work on most new monitors 

        clock_enable <= ! clock_enable; 

      end
	
endmodule