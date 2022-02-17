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

module dds_multipath
  #(
     parameter NUMBER_OF_LINE = 8
   )
   (
     input clock,
     input resetn,
     input [15:0] dds_phase_inc,
     output logic [16*NUMBER_OF_LINE-1:0] sin_out_i,
     output logic [16*NUMBER_OF_LINE-1:0] cosin_out_q
   );

  genvar i;
  logic [15:0] phase_acc_value = 0;
  logic [31:0] dds_data_array[NUMBER_OF_LINE];

  // generate NUMBER_OF_LINE-line 4GSPS DDS signal
  always @(posedge clock)
  begin
    phase_acc_value <= phase_acc_value + NUMBER_OF_LINE*dds_phase_inc;
  end
  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      dds_compiler_core	dds_compiler_core_inst
                        (
                          .aclk(clock),
                          .s_axis_phase_tvalid(resetn),
                          .s_axis_phase_tdata(phase_acc_value + i*dds_phase_inc),
                          .m_axis_data_tvalid(),
                          .m_axis_data_tdata(dds_data_array[i])
                        );
      always @(posedge clock)
      begin
        sin_out_i[16*(i+1)-1:16*i] <= dds_data_array[i][15:0];
        cosin_out_q[16*(i+1)-1:16*i] <= dds_data_array[i][31:16];
      end
    end
  endgenerate

endmodule
