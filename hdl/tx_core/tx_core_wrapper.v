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
    input adc1_tvalid,
    input [127:0] adc1_tdata,
    output adc1_tready,
    input [15:0] dds_phase_inc1,
    input adc2_tvalid,
    input [127:0] adc2_tdata,
    output adc2_tready,
    input [15:0] dds_phase_inc2,
    input adc3_tvalid,
    input [127:0] adc3_tdata,
    output adc3_tready,
    input [15:0] dds_phase_inc3,
    input [2:0] dbg_select,
    input dac_tready,
    output [255:0] dac_tdata,
    output dac_tvalid,
    output [255:0] dbg_output_data
  );

  tx_core tx_core_inst
          (
            .clock(clock),
            .resetn(resetn),
            .adc1_tvalid(adc1_tvalid),
            .adc1_tdata(adc1_tdata),
            .adc1_tready(adc1_tready),
            .dds_phase_inc1(dds_phase_inc1),
            .adc2_tvalid(adc2_tvalid),
            .adc2_tdata(adc2_tdata),
            .adc2_tready(adc2_tready),
            .dds_phase_inc2(dds_phase_inc2),
            .adc3_tvalid(adc3_tvalid),
            .adc3_tdata(adc3_tdata),
            .adc3_tready(adc3_tready),
            .dds_phase_inc3(dds_phase_inc3),
            .output_select(dbg_select),
            .dac_tready(dac_tready),
            .dac_tdata(dac_tdata),
            .dac_tvalid(dac_tvalid),
            .dbg_output_data(dbg_output_data)
          );

endmodule
