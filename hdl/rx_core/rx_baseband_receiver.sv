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
     input [15:0] ddc_phase_inc,
     output [15:0] data_out
   );

  genvar i;
  logic [16*NUMBER_OF_LINE-1:0] dac1_data_i_temp1;
  logic [16*NUMBER_OF_LINE-1:0] dac1_data_q_temp1;
  logic [16*NUMBER_OF_LINE-1:0] dac2_data_i_temp1;
  logic [16*NUMBER_OF_LINE-1:0] dac2_data_q_temp1;
  logic [16*NUMBER_OF_LINE-1:0] dac3_data_i_temp1;
  logic [16*NUMBER_OF_LINE-1:0] dac3_data_q_temp1;
  always @(posedge clock)
  begin
    resetn_reg <= resetn;
  end

  iq_freq_shift	iq_freq_shift_dac1
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(dac1_data_i_temp1),
                  .data_in_q(dac1_data_q_temp1),
                  .dds_phase_inc(duc1_phase_inc),
                  .data_out_i(dac1_data),
                  .data_out_q()
                );

  iq_freq_shift	iq_freq_shift_dac2
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(dac2_data_i_temp1),
                  .data_in_q(dac2_data_q_temp1),
                  .dds_phase_inc(duc2_phase_inc),
                  .data_out_i(dac2_data),
                  .data_out_q()
                );

  iq_freq_shift	iq_freq_shift_dac3
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(dac3_data_i_temp1),
                  .data_in_q(dac3_data_q_temp1),
                  .dds_phase_inc(duc3_phase_inc),
                  .data_out_i(dac3_data),
                  .data_out_q()
                );

endmodule
