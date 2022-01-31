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
    input bypass_enable,
    input [127:0] adc_data,
    input [15:0] dds_phase_inc,
    input [7:0] output_gain,
    output [127:0] output_data,
    // debug probes
    output [15:0] dbg_dds_sin,
    output [15:0] dbg_dds_cosin,
    output [61:0] dbg_cm_data,
    output [47:0] dbg_output_data
  );

  tx_core tx_core_inst
          (
            .clock(),
            .resetn(),
            .bypass_enable(),
            .adc_data(),
            .dds_phase_inc(),
            .output_gain(),
            .output_data(),
            .dbg_dds_sin(),
            .dbg_dds_cosin(),
            .dbg_cm_data(),
            .dbg_output_data()
          );

endmodule
