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
     input clock_down4,
     input [16*NUMBER_OF_LINE-1:0] adc_data,
     input [16-1:0] ddc_phase_inc,
     input [16-1:0] demix_phase_inc,
     input [16-1:0] duc1_phase_inc,
     input [16-1:0] duc2_phase_inc,
     input [16-1:0] duc3_phase_inc,
     input [8-1:0] gain_duc2,
     input [8-1:0] gain_duc3,
     input [8-1:0] gain_duc1,
     output [16*NUMBER_OF_LINE-1:0] dac1_data,
     output [16*NUMBER_OF_LINE-1:0] dac2_data,
     output [16*NUMBER_OF_LINE-1:0] dac3_data
   );

  genvar i;

  centeral_rx_dsp_core centeral_rx_dsp_core_inst (
                         .adc_data0(adc_data[16*(1)-1:16*(0)+1]),
                         .adc_data1(adc_data[16*(2)-1:16*(1)+1]),
                         .adc_data2(adc_data[16*(3)-1:16*(2)+1]),
                         .adc_data3(adc_data[16*(4)-1:16*(3)+1]),
                         .adc_data4(adc_data[16*(5)-1:16*(4)+1]),
                         .adc_data5(adc_data[16*(6)-1:16*(5)+1]),
                         .adc_data6(adc_data[16*(7)-1:16*(6)+1]),
                         .adc_data7(adc_data[16*(8)-1:16*(7)+1]),
                         .ddc_phase_inc(ddc_phase_inc),
                         .demix_phase_inc(demix_phase_inc),
                         .duc1_phase_inc(duc1_phase_inc),
                         .duc2_phase_inc(duc2_phase_inc),
                         .duc3_phase_inc(duc3_phase_inc),
                         .gain_duc1(gain_duc1),
                         .gain_duc2(gain_duc2),
                         .gain_duc3(gain_duc3),
                         .ddc_ss_clk(clock),
                         .duc_ss_clk(clock),
                         .demix_ss_clk(clock_down4),
                         .concat_imag_dac1(),
                         .concat_imag_dac2(),
                         .concat_imag_dac3(),
                         .concat_real_dac1(dac1_data),
                         .concat_real_dac2(dac2_data),
                         .concat_real_dac3(dac3_data)
                       );

endmodule
