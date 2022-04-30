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

module tx_core
  #(
     parameter NUMBER_OF_LINE = 8
   )
   (
     input clock,
     input [7:0] mixer_gain,
     input [15:0] lo_dds_phase_inc,
     input enable_pre_distortion,
     input [3:0] scale_select,
     input [16*NUMBER_OF_LINE-1:0] adc1_data,
     input [16*NUMBER_OF_LINE-1:0] adc2_data,
     input [16*NUMBER_OF_LINE-1:0] adc3_data,
     output [16-1:0] interval_max,
     output logic [16*NUMBER_OF_LINE-1:0] dac_data
   );

  centeral_tx_dsp_core centeral_tx_dsp_core_inst (
                         .adc1_data0(adc1_data[16*1-1:16*0]),
                         .adc1_data1(adc1_data[16*2-1:16*1]),
                         .adc1_data2(adc1_data[16*3-1:16*2]),
                         .adc1_data3(adc1_data[16*4-1:16*3]),
                         .adc1_data4(adc1_data[16*5-1:16*4]),
                         .adc1_data5(adc1_data[16*6-1:16*5]),
                         .adc1_data6(adc1_data[16*7-1:16*6]),
                         .adc1_data7(adc1_data[16*8-1:16*7]),
                         .adc2_data0(adc2_data[16*1-1:16*0]),
                         .adc2_data1(adc2_data[16*2-1:16*1]),
                         .adc2_data2(adc2_data[16*3-1:16*2]),
                         .adc2_data3(adc2_data[16*4-1:16*3]),
                         .adc2_data4(adc2_data[16*5-1:16*4]),
                         .adc2_data5(adc2_data[16*6-1:16*5]),
                         .adc2_data6(adc2_data[16*7-1:16*6]),
                         .adc2_data7(adc2_data[16*8-1:16*7]),
                         .adc3_data0(adc3_data[16*1-1:16*0]),
                         .adc3_data1(adc3_data[16*2-1:16*1]),
                         .adc3_data2(adc3_data[16*3-1:16*2]),
                         .adc3_data3(adc3_data[16*4-1:16*3]),
                         .adc3_data4(adc3_data[16*5-1:16*4]),
                         .adc3_data5(adc3_data[16*6-1:16*5]),
                         .adc3_data6(adc3_data[16*7-1:16*6]),
                         .adc3_data7(adc3_data[16*8-1:16*7]),
                         .clk(clock),
						 .mixer_gain(mixer_gain),
						 .hilbert_asr_delay(42),
						 .hilbert_mux_select(0),
                         .dds_phase_inc(lo_dds_phase_inc),
                         .enable_pre_distortion(enable_pre_distortion),
                         .scale_select(scale_select),
                         .dac_data0(dac_data[16*1-1:16*0]),
                         .dac_data1(dac_data[16*2-1:16*1]),
                         .dac_data2(dac_data[16*3-1:16*2]),
                         .dac_data3(dac_data[16*4-1:16*3]),
                         .dac_data4(dac_data[16*5-1:16*4]),
                         .dac_data5(dac_data[16*6-1:16*5]),
                         .dac_data6(dac_data[16*7-1:16*6]),
                         .dac_data7(dac_data[16*8-1:16*7])
                       );

endmodule
