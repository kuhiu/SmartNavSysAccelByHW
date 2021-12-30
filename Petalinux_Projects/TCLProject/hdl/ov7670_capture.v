`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/03/2021 03:19:29 PM
// Design Name: 
// Module Name: ov7670_capture
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


module ov7670_capture(
    input               pclk,           // clk de la camara ov7670
    input               vsync,          // sincronizacion vertical, termino todo el frame
    input               href,           // sincronizacion horizontal, termino la linea de pixeles
    input  [8-1 :0]     d,              // cada dos bytes un pixel es recibido, modo de trabajo a utilizar => RGB444
    output reg [12-1:0] dout,           // pixel
    output reg          pixel_valid,    // indica que el pixel es valido 
    output reg          frame_done      // termino un frame
    );
    
    // Estados
    reg                 state, next_state; 
    localparam          WAIT_FRAME  = 1'b0;
    localparam          CAPTURE     = 1'b1;

    reg                 byte_arrived;

    always @(posedge pclk) begin
        state <= next_state;
    end

    always @(posedge pclk) begin
        case (state) 
        
            WAIT_FRAME: begin   // Espero por vsync
                if (vsync == 1'b1)
                    next_state <= WAIT_FRAME;
                else 
                    next_state <= CAPTURE;
                frame_done <= 1'b0;
                
            end
            
            CAPTURE: begin
                if (vsync == 1'b1) begin        // Si llega un vsync mientras estoy caputando
                    next_state <= WAIT_FRAME;   // significa que ya termino el frame.
                    frame_done <= 1'b1;
                end else begin 
                    next_state <= CAPTURE;
                    frame_done <= 1'b0;
                end
                
                if (href && byte_arrived) 
                    pixel_valid <= 1'b1;
                else 
                    pixel_valid <= 1'b0;
                
                if (href) begin
                    byte_arrived <= ~ byte_arrived;
                    if (byte_arrived == 1'b1) 
                        dout <= d[3:0];
                    else 
                        dout[11:4] <= d; 
                end
            end
            
            default: 
                next_state <= WAIT_FRAME;
            
        endcase
    end

endmodule
