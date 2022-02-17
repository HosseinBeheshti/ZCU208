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
    input [127:0] data,
    output [15:0] data_line_1,
    output [15:0] data_line_2,
    output [15:0] data_line_3,
    output [15:0] data_line_4,
    output [15:0] data_line_5,
    output [15:0] data_line_6,
    output [15:0] data_line_7,
    output [15:0] data_line_8
  );

  assign data_line_1 = data[16*(1)-1:16*(0)];
  assign data_line_2 = data[16*(2)-1:16*(1)];
  assign data_line_3 = data[16*(3)-1:16*(2)];
  assign data_line_4 = data[16*(4)-1:16*(3)];
  assign data_line_5 = data[16*(5)-1:16*(4)];
  assign data_line_6 = data[16*(6)-1:16*(5)];
  assign data_line_7 = data[16*(7)-1:16*(6)];
  assign data_line_8 = data[16*(8)-1:16*(7)];

endmodule
