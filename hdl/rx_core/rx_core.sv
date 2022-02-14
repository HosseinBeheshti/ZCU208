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

module rx_core
  #(
     parameter NUMBER_OF_LINE = 8
   )
   (
     input clock,
     input resetn,
     input [4:0] output_select,
     input [15:0] compelex_phase_inc,
     input [15:0] real_phase_inc,
     input [15:0] duc1_phase_inc,
     input [15:0] duc2_phase_inc,
     input [15:0] duc3_phase_inc,
     input [16*NUMBER_OF_LINE-1:0] adc_data,
     output [16*NUMBER_OF_LINE-1:0] dac1_data,
     output [16*NUMBER_OF_LINE-1:0] dac2_data,
     output [16*NUMBER_OF_LINE-1:0] dac3_data
   );

  genvar i;
  logic resetn_reg;
  logic [16*NUMBER_OF_LINE-1:0] baseband_data1_i;
  logic [16*NUMBER_OF_LINE-1:0] baseband_data1_q;
  logic [16*NUMBER_OF_LINE-1:0] baseband_data2;
  logic [16*NUMBER_OF_LINE-1:0] baseband_data3;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data1_i;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data1_q;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data2_i;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data2_q;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data3_i;
  logic [16*NUMBER_OF_LINE-1:0] lpf2_data3_q;

  always @(posedge clock)
  begin
    resetn_reg <= resetn;
  end

rx_baseband_receiver rx_baseband_receiver_inst 
(
     .clock(clock),
     .resetn(resetn_reg),
     .data_in(adc_data[15:0]),
     .compelex_phase_inc(compelex_phase_inc),
     .real_phase_inc(real_phase_inc),
     .data_out1_i(baseband_data1_i[15:0]),
     .data_out1_q(baseband_data1_q[15:0]),
     .data_out2(baseband_data2[15:0]),
     .data_out3(baseband_data3[15:0])
   );

// upsample fir filter
  fir_compiler_8path fir_compiler_8path_ch1_i
                     (
                       .clk(clock),
                       .concat_data_in(baseband_data1_i),
                       .concat_data_out(lpf2_data1_i)
                     );

  fir_compiler_8path fir_compiler_8path_ch1_q
                     (
                       .clk(clock),
                       .concat_data_in(baseband_data1_q),
                       .concat_data_out(lpf2_data1_q)
                     );
  fir_compiler_8path fir_compiler_8path_ch2
                     (
                       .clk(clock),
                       .concat_data_in(baseband_data2),
                       .concat_data_out(lpf2_data2_i)
                     );
  fir_compiler_8path fir_compiler_8path_ch3
                     (
                       .clk(clock),
                       .concat_data_in(baseband_data3),
                       .concat_data_out(lpf2_data3_i)
                     );

  iq_freq_shift	iq_freq_shift_dac1
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(lpf2_data1_i),
                  .data_in_q(lpf2_data1_q),
                  .dds_phase_inc(duc1_phase_inc),
                  .data_out_i(dac1_data),
                  .data_out_q()
                );

  iq_freq_shift	iq_freq_shift_dac2
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(lpf2_data2_i),
                  .data_in_q(lpf2_data2_q),
                  .dds_phase_inc(duc2_phase_inc),
                  .data_out_i(dac2_data),
                  .data_out_q()
                );

  iq_freq_shift	iq_freq_shift_dac3
                (
                  .clock(clock),
                  .resetn(resetn_reg),
                  .data_in_i(lpf2_data3_i),
                  .data_in_q(lpf2_data3_q),
                  .dds_phase_inc(duc3_phase_inc),
                  .data_out_i(dac3_data),
                  .data_out_q()
                );

endmodule
