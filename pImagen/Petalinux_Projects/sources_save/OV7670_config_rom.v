`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/06/2015 02:41:55 PM
// Design Name: 
// Module Name: OV7670_config_rom
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


module OV7670_config_rom(
    input wire clk,
    input wire [7:0] addr,
    output reg [15:0] dout
    );
    //FFFF is end of rom, FFF0 is delay
    always @(posedge clk) begin
    case(addr) 
        8'h00: dout <= 16'h12_80; // COM7   Reset
	    8'h01: dout <= 16'h12_80; // COM7   Reset
	    8'h02: dout <= 16'h12_04; // COM7   Size & RGB output
	    8'h03: dout <= 16'h11_40; // CLKRC  Prescaler - Fin/(1+1)
	    8'h04: dout <= 16'h0C_00; // COM3   Lots of stuff, enable scaling, all others off
	    8'h05: dout <= 16'h3E_00; // COM14  PCLK scaling off
   		8'h06: dout <= 16'h8C_00; // RGB444 Set RGB format
   		8'h07: dout <= 16'h04_00; // COM1   no CCIR601
 		8'h08: dout <= 16'h40_10; // COM15  Full 0-255 output, RGB 565
		8'h09: dout <= 16'h3a_04; // TSLB   Set UV ordering,  do not auto-reset window
		8'h0A: dout <= 16'h14_38; // COM9  - AGC Celling
		8'h0B: dout <= 16'h4f_b3; // MTX1  - colour conversion matrix
		8'h0C: dout <= 16'h50_b3; // MTX2  - colour conversion matrix
		8'h0D: dout <= 16'h51_00; // MTX3  - colour conversion matrix
		8'h0E: dout <= 16'h52_3d; // MTX4  - colour conversion matrix
		8'h0F: dout <= 16'h53_a7; // MTX5  - colour conversion matrix
		8'h10: dout <= 16'h54_e4; // MTX6  - colour conversion matrix
		8'h11: dout <= 16'h58_9e; // MTXS  - Matrix sign and auto contrast
		8'h12: dout <= 16'h3d_c0; // COM13 - Turn on GAMMA and UV Auto adjust
		8'h13: dout <= 16'h11_00; // CLKRC  Prescaler - Fin/(1+1)
		8'h14: dout <= 16'h17_11; // HSTART HREF start (high 8 bits)
		8'h15: dout <= 16'h18_61; // HSTOP  HREF stop (high 8 bits)
		8'h16: dout <= 16'h32_A4; // HREF   Edge offset and low 3 bits of HSTART and HSTOP
		8'h17: dout <= 16'h19_03; // VSTART VSYNC start (high 8 bits)
		8'h18: dout <= 16'h1A_7b; // VSTOP  VSYNC stop (high 8 bits) 
		8'h19: dout <= 16'h03_0a; // VREF   VSYNC low two bits
        8'h1A: dout <= 16'h0e_61; // COM5(0x0E) 0x61
        8'h1B: dout <= 16'h0f_4b; // COM6(0x0F) 0x4B 
        8'h1C: dout <= 16'h16_02; //
        8'h1D: dout <= 16'h1e_37; // MVFP (0x1E) 0x07  -- FLIP AND MIRROR IMAGE 0x3x
        8'h1E: dout <= 16'h21_02;
        8'h1F: dout <= 16'h22_91;
        8'h20: dout <= 16'h29_07;
        8'h21: dout <= 16'h33_0b;   
        8'h22: dout <= 16'h35_0b;
        8'h23: dout <= 16'h37_1d;             
        8'h24: dout <= 16'h38_71;
        8'h25: dout <= 16'h39_2a;             
        8'h26: dout <= 16'h3c_78; // COM12 (0x3C) 0x78
        8'h27: dout <= 16'h4d_40;                   
        8'h28: dout <= 16'h4e_20;
        8'h29: dout <= 16'h69_00; // GFIX (0x69) 0x00                 
        8'h2A: dout <= 16'h6b_4a;
        8'h2B: dout <= 16'h74_10; 
        8'h2C: dout <= 16'h8d_4f;
        8'h2D: dout <= 16'h8e_00;                   
        8'h2E: dout <= 16'h8f_00;
        8'h2F: dout <= 16'h90_00;                      
        8'h30: dout <= 16'h91_00;
        8'h31: dout <= 16'h96_00;              
        8'h32: dout <= 16'h9a_00;
        8'h33: dout <= 16'hb0_84;          
        8'h34: dout <= 16'hb1_0c;
        8'h35: dout <= 16'hb2_0e;
        8'h36: dout <= 16'hb3_82;
        8'h37: dout <= 16'hb8_0a;
	    default: dout <= 16'hFF_FF;         //mark end of ROM
    endcase
    
    end
endmodule



//				when x"10" => sreg <= x"703a"; -- SCALING_XSC
//				when x"11" => sreg <= x"7135"; -- SCALING_YSC
//				when x"12" => sreg <= x"7200"; -- SCALING_DCWCTR  -- zzz was 11 
//				when x"13" => sreg <= x"7300"; -- SCALING_PCLK_DIV
//				when x"14" => sreg <= x"a200"; -- SCALING_PCLK_DELAY  must match COM14
//          when x"15" => sreg <= x"1500"; -- COM10 Use HREF not hSYNC
//				
//	when x"1D" => sreg <= x"B104"; -- ABLC1 - Turn on auto black level
//	when x"1F" => sreg <= x"138F"; -- COM8  - AGC, White balance
//	when x"21" => sreg <= x"FFFF"; -- spare
//	when x"22" => sreg <= x"FFFF"; -- spare
//	when x"23" => sreg <= x"0000"; -- spare
//	when x"24" => sreg <= x"0000"; -- spare
//	when x"25" => sreg <= x"138F"; -- COM8 - AGC, White balance
//	when x"26" => sreg <= x"0000"; -- spare
//	when x"27" => sreg <= x"1000"; -- AECH Exposure
//	when x"28" => sreg <= x"0D40"; -- COMM4 - Window Size
//	when x"29" => sreg <= x"0000"; -- spare
//	when x"2a" => sreg <= x"a505"; -- AECGMAX banding filter step
//	when x"2b" => sreg <= x"2495"; -- AEW AGC Stable upper limite
//	when x"2c" => sreg <= x"2533"; -- AEB AGC Stable lower limi
//	when x"2d" => sreg <= x"26e3"; -- VPT AGC fast mode limits
//	when x"2e" => sreg <= x"9f78"; -- HRL High reference level
//	when x"2f" => sreg <= x"A068"; -- LRL low reference level
//	when x"30" => sreg <= x"a103"; -- DSPC3 DSP control
//	when x"31" => sreg <= x"A6d8"; -- LPH Lower Prob High
//	when x"32" => sreg <= x"A7d8"; -- UPL Upper Prob Low
//	when x"33" => sreg <= x"A8f0"; -- TPL Total Prob Low
//	when x"34" => sreg <= x"A990"; -- TPH Total Prob High
//	when x"35" => sreg <= x"AA94"; -- NALG AEC Algo select
//	when x"36" => sreg <= x"13E5"; -- COM8 AGC Settings