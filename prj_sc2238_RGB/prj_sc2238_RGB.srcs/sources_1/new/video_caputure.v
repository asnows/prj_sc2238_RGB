`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/08/08 15:06:46
// Design Name: 
// Module Name: video_caputure
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


    module video_caputure 
    #(
        parameter DATA_WIDTH = 10
    )

    (
        input  wire          piexl_clk    ,
        input  wire          href         ,
        input  wire          vsync        ,
        input  wire [ DATA_WIDTH - 1 : 0] data_in      ,
        input  wire [12 : 0] img_width    ,
        output wire [DATA_WIDTH - 1 : 0 ] m_axis_tdata ,
        output wire          m_axis_tlast ,
        output wire          m_axis_tuser ,
        output wire          m_axis_tvalid    
    );
        //parameter    IMG_WIDTH = 640;
        reg[11:0]     row_pixels_count = 12'd0;
        reg           tlast_tmp;
        reg           tuser_tmp ;
        wire          tuser;
//        wire          s_axis_tready;
        reg  [12 : 0] reg_img_width = 13'd640;
        
        
        assign m_axis_tdata = data_in; 
        assign m_axis_tlast = tlast_tmp; 
        assign m_axis_tuser =(tuser_tmp & href); 
        assign m_axis_tvalid = href;   
        
        //generate tuser
        always @(posedge piexl_clk)
        begin
            if(vsync == 1)
                begin
                    tuser_tmp <= 1'b1;    
                end
            else if(href == 1)
                begin
                    tuser_tmp <= 1'b0;
                end
            else
                begin
                    tuser_tmp <= tuser_tmp;
                end
                 
        end
         
        //count row pixels
        always @(posedge piexl_clk)
        begin
            if(vsync == 1 )
                begin
                    row_pixels_count <= 12'd0;
                    reg_img_width <= img_width;
                end
            else if(href == 1)
                begin
                    row_pixels_count <= row_pixels_count + 1; 
                end
            else
                begin
                    row_pixels_count <= 12'd0;
                end
        end
        
        //genrate tlast
        always @(posedge piexl_clk)
        begin
            if(row_pixels_count == (reg_img_width - 2))//(reg_img_width - 2)
                begin
                    tlast_tmp <= 1'b1;
                end
            else
                begin
                    tlast_tmp <= 1'b0;
                end
        end
                      
endmodule
