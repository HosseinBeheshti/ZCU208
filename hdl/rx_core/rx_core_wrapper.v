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

module rx_core_wrapper
  (

    input clock,
    input clock_down4,
    input [16*8-1:0] adc_data,
    input [16-1:0] ddc_phase_inc,
    input [16-1:0] demix_phase_inc,
    input [16-1:0] duc_phase_inc,
    input [16-1:0] lpf1_gain,
    input [16-1:0] lpf2_gain,
    input [16-1:0] lpf3_gain,
    input [16-1:0] lpf4_gain,
    output [16*8-1:0] dac1_data,
    output [16*8-1:0] dac2_data,
    output [16*8-1:0] dac3_data
  );

  rx_core rx_core_inst
          (
            .clock(clock),
            .clock_down4(clock_down4),
            .adc_data(adc_data),
            .ddc_phase_inc(ddc_phase_inc),
            .demix_phase_inc(demix_phase_inc),
            .duc1_phase_inc(duc_phase_inc),
            .duc2_phase_inc(duc_phase_inc),
            .duc3_phase_inc(duc_phase_inc),
            .lpf1_gain(lpf1_gain),
            .lpf2_gain(lpf2_gain),
            .lpf3_gain(lpf3_gain),
            .lpf4_gain(lpf4_gain),
            .dac1_data(dac1_data),
            .dac2_data(dac2_data),
            .dac3_data(dac3_data)
          );

endmodule
