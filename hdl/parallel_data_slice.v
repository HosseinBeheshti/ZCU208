`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//------------------
//-___           ___
//|   |         |   |
//|   |         |   |
//|   |         |   |
//|   |_________|   |
//|                 |
//|    H128B717     |
//|    _________    |
//|   |         |   |
//|   |         |   |
//|   |         |   |
//|___|         |___|
//-------------------
// Engineer: HosseinBehshti
//////////////////////////////////////////////////////////////////////////////////

module parallel_data_slice
  (
    input clock,
    input [255:0] data,
    output [15:0] data_line_r1,
    output [15:0] data_line_q1,
    output [15:0] data_line_r2,
    output [15:0] data_line_q2,
    output [15:0] data_line_r3,
    output [15:0] data_line_q3,
    output [15:0] data_line_r4,
    output [15:0] data_line_q4,
    output [15:0] data_line_r5,
    output [15:0] data_line_q5,
    output [15:0] data_line_r6,
    output [15:0] data_line_q6,
    output [15:0] data_line_r7,
    output [15:0] data_line_q7,
    output [15:0] data_line_r8,
    output [15:0] data_line_q8
 
  );

  assign data_line_r1 = data[16*(1)-1:16*(0)];
  assign data_line_q1 = data[16*(2)-1:16*(1)];
  assign data_line_r2 = data[16*(3)-1:16*(2)];
  assign data_line_q2 = data[16*(4)-1:16*(3)];
  assign data_line_r3 = data[16*(5)-1:16*(4)];
  assign data_line_q3 = data[16*(6)-1:16*(5)];
  assign data_line_r4 = data[16*(7)-1:16*(6)];
  assign data_line_q4 = data[16*(8)-1:16*(7)];
  assign data_line_r5 = data[16*(9)-1:16*(8)];
  assign data_line_q5 = data[16*(10)-1:16*(9)];
  assign data_line_r6 = data[16*(11)-1:16*(10)];
  assign data_line_q6 = data[16*(12)-1:16*(11)];
  assign data_line_r7 = data[16*(13)-1:16*(12)];
  assign data_line_q7 = data[16*(14)-1:16*(13)];
  assign data_line_r8 = data[16*(15)-1:16*(14)];
  assign data_line_q8 = data[16*(16)-1:16*(15)];

endmodule
