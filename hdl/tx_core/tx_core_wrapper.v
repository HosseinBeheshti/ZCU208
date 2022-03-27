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

module tx_core_wrapper
  (
    input clock,
    input resetn,
    input [7:0] mixer_gain,
    input [15:0] lo_dds_phase_inc,
    input enable_pre_distortion,
    input [3:0] scale_select,
    input [127:0] adc1_data,
    input [127:0] adc2_data,
    input [127:0] adc3_data,
    output [16-1:0] interval_max,
    output [127:0] dac_data
  );

  tx_core tx_core_inst
          (
            .clock(clock),
            .mixer_gain(mixer_gain),
            .lo_dds_phase_inc(lo_dds_phase_inc),
            .enable_pre_distortion(enable_pre_distortion),
            .scale_select(scale_select),
            .adc1_data(adc1_data),
            .adc2_data(adc2_data),
            .adc3_data(adc3_data),
            .interval_max(interval_max),
            .dac_data(dac_data)
          );

endmodule
