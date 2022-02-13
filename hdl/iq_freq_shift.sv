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

module iq_freq_shift
  #(
     parameter NUMBER_OF_LINE = 8
   )
   (
     input clock,
     input resetn,
     input [16*NUMBER_OF_LINE-1:0] data_in_i,
     input [16*NUMBER_OF_LINE-1:0] data_in_q,
     input [15:0] dds_phase_inc,
     output logic [16*NUMBER_OF_LINE-1:0] data_out_i,
     output logic [16*NUMBER_OF_LINE-1:0] data_out_q
   );

  genvar i;
  logic [15:0] data_array_i[NUMBER_OF_LINE];
  logic [15:0] data_array_q[NUMBER_OF_LINE];
  logic [31:0] dds_data_array[NUMBER_OF_LINE];
  logic [16*NUMBER_OF_LINE-1:0] dds_sin;
  logic [16*NUMBER_OF_LINE-1:0] dds_cosin;
  logic [15:0] dds_sin_array[NUMBER_OF_LINE];
  logic [15:0] dds_cosin_array[NUMBER_OF_LINE];
  logic [79:0] cm_data_array[NUMBER_OF_LINE];

  // DDS signal generator
  dds_multipath
    #(.NUMBER_OF_LINE(NUMBER_OF_LINE)
     )
    dds_multipath_inst
    (
      .clock(clock),
      .resetn(resetn),
      .dds_phase_inc(dds_phase_inc),
      .sin_out_i(dds_sin),
      .cosin_out_q(dds_cosin)
    );

  // rearrange input adc signal
  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      always @(posedge clock)
      begin
        data_array_i[i] <= data_in_i[16*(i+1)-1:16*i];
        data_array_q[i] <= data_in_q[16*(i+1)-1:16*i];
        dds_sin_array[i] <= dds_sin[16*(i+1)-1:16*i];
        dds_cosin_array[i] <= dds_cosin[16*(i+1)-1:16*i];
      end
      // CM for NUMBER_OF_LINE  line
      cmult_core cmult_core_inst
                 (
                   .aclk(clock),
                   .s_axis_a_tvalid(resetn),
                   .s_axis_a_tdata({data_array_q[i],data_array_i[i]}),
                   .s_axis_b_tvalid(resetn),
                   .s_axis_b_tdata({dds_sin_array[i],dds_cosin_array[i]}),
                   .m_axis_dout_tvalid(),
                   .m_axis_dout_tdata(cm_data_array[i])
                 );
      always @(posedge clock)
      begin
        data_out_i[16*(i+1)-1:16*i] <= cm_data_array[i][32:17];
        data_out_q[16*(i+1)-1:16*i] <= cm_data_array[i][72:57];
      end
    end
  endgenerate

endmodule
