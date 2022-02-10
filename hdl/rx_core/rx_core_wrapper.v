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
    input resetn,
    input [4:0] output_select,
    input [15:0] ddc1_phase_inc,
    input [15:0] ddc2_phase_inc,
    input [15:0] ddc3_phase_inc,
    input [15:0] duc1_phase_inc,
    input [15:0] duc2_phase_inc,
    input [15:0] duc3_phase_inc,
    input [127:0] adc_data,
    output [127:0] dac1_data,
    output [127:0] dac2_data,
    output [127:0] dac3_data
  );

  rx_core rx_core_inst
          (
            .clock(clock),
            .resetn(resetn),
            .output_select(output_select),
            .ddc1_phase_inc(ddc1_phase_inc),
            .ddc2_phase_inc(ddc2_phase_inc),
            .ddc3_phase_inc(ddc3_phase_inc),
            .duc1_phase_inc(duc1_phase_inc),
            .duc2_phase_inc(duc2_phase_inc),
            .duc3_phase_inc(duc3_phase_inc),
            .adc_data(adc_data),
            .dac1_data(dac1_data),
            .dac2_data(dac2_data),
            .dac3_data(dac3_data)
          );

endmodule
