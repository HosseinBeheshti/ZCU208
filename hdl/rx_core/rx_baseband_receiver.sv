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

module rx_baseband_receiver
  (
    input clock,
    input resetn,
    input [15:0] data_in,
    input [15:0] cmplx_phase_inc,
    input [15:0] real_phase_inc,
    output [15:0] data_out
  );

  genvar i;
  logic [15:0] data_in_array[4];
  logic [16*4-1:0] data_in_down4;
  logic [15:0] data_in_reg3;
  logic clock_down4;

  always @(posedge clock)
  begin
    data_in_array[0] <= data_in;
    data_in_array[1] <= data_in_array[0];
    data_in_array[2] <= data_in_array[1];
    data_in_array[3] <= data_in_array[2];
  end

  always @(posedge clock_down4)
  begin
    data_in_down4 <= {data_in_array[3],data_in_array[2],data_in_array[1],data_in_array[0]};
  end

  iq_freq_shift	
    #(.NUMBER_OF_LINE(4)
     )
  iq_freq_shift_dac1
                (
                  .clock(clock_down4),
                  .resetn(resetn),
                  .data_in_i(data_in_down4),
                  .data_in_q(0),
                  .dds_phase_inc(cmplx_phase_inc),
                  .data_out_i(),
                  .data_out_q()
                );


endmodule
