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
     input resetn,
     input bypass_enable,
     input [127:0] adc_data,
     input [15:0] dds_phase_inc,
     input [7:0] output_gain,
     output logic [127:0] output_data,
     // debug probes
     output [15:0] dbg_dds_sin,
     output [15:0] dbg_dds_cosin,
     output [61:0] dbg_cm_data,
     output [47:0] dbg_output_data
   );

  genvar i;
  logic [13:0] adc_data_array[NUMBER_OF_LINE];
  logic [15:0] phase_acc_value = 0;
  logic [31:0] dds_data_array[NUMBER_OF_LINE];
  logic dds_data_valid_array[NUMBER_OF_LINE];
  logic [15:0] dds_sin_array[NUMBER_OF_LINE];
  logic [15:0] dds_cosin_array[NUMBER_OF_LINE];
  logic [31:0] cm_dds_data[NUMBER_OF_LINE];
  logic cm_data_valid_array[NUMBER_OF_LINE];
  logic [63:0] cm_data_array[NUMBER_OF_LINE];
  logic [47:0] cm_output_data_array[NUMBER_OF_LINE];

  // rearrange input adc signal
  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      always @(posedge clock)
      begin
        adc_data_array[i] <= adc_data[16*(i+1)-1:16*i+1];
      end
    end
  endgenerate

  // generate NUMBER_OF_LINE-line 4GSPS DDS signal
  always @(posedge clock)
  begin
    phase_acc_value <= phase_acc_value + dds_phase_inc;
  end
  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      dds_compiler_core	dds_compiler_core_inst
                        (
                          .aclk(clock),
                          .s_axis_phase_tvalid(resetn),
                          .s_axis_phase_tdata(phase_acc_value + i*dds_phase_inc),
                          .m_axis_data_tvalid(dds_data_valid_array[i]),
                          .m_axis_data_tdata(dds_data_array[i])
                        );
      always @(posedge clock)
      begin
        dds_sin_array[i] <= dds_data_array[i][15:0];
        dds_cosin_array[i] <= dds_data_array[i][31:16];
      end
      assign cm_dds_data[i] = bypass_enable ? {16'd0, 16'd1} : {dds_sin_array[i],dds_cosin_array[i]};

  // CM for NUMBER_OF_LINE  line
      ddc_cmpy_core ddc_cmpy_core_inst
                    (
                      .aclk(clock),
                      .s_axis_a_tvalid(resetn),
                      .s_axis_a_tdata({14'd0,adc_data_array[i]}),
                      .s_axis_b_tvalid(resetn),
                      .s_axis_b_tdata(cm_dds_data[i]),
                      .m_axis_dout_tvalid(cm_data_valid_array[i]),
                      .m_axis_dout_tdata(cm_data_array[i])
                    );
      ddc_xbip_multadd ddc_xbip_multadd_inst(
                         .CLK(clock),
                         .CE(1),
                         .SCLR(!resetn),
                         .A(output_gain),
                         .B(cm_data_array[i][30:16]),
                         .C(cm_data_array[i][62:47]),
                         .SUBTRACT(0),
                         .P(cm_output_data_array[i]),
                         .PCOUT()
                       );
      always @(posedge clock)
      begin
        output_data[16*(i+1)-1:16*i] <= {cm_output_data_array[i][13:0], 2'd0};
      end
    end
  endgenerate
  assign dbg_dds_sin = dds_sin_array[0];
  assign dbg_dds_cosin = dds_cosin_array[0];
  assign dbg_cm_data = cm_data_array[0];
  assign dbg_output_data = cm_output_data_array[0];

endmodule