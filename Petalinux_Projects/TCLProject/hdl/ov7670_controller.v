`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03.08.2021 18:54:34
// Design Name: 
// Module Name: ov7670_controller
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


module ov7670_controller;

    reg clk;
    reg start;
    wire sioc;
    wire siod;
    wire done;
    
    // Unit under test (UUT)
    ov7670 uut(
        .clk(clk),            
        .start(start),          
        .sioc(sioc),            
        .siod(siod),                  
		.done(done)
    );

    always #5 clk = ~clk;
    
    initial
    begin

    clk = 1'b0;
    start = 1'b0;
    #500;
    start = 1'b1;
    end
endmodule

