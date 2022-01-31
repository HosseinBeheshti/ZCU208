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
     input adc1_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc1_tdata,
     output adc1_tready,
     input [15:0] dds_phase_inc1,
     input adc2_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc2_tdata,
     output adc2_tready,
     input [15:0] dds_phase_inc2,
     input adc3_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc3_tdata,
     output adc3_tready,
     input [15:0] dds_phase_inc3,
     input [2:0] output_select,
     input dac_tready,
     output logic [2*16*NUMBER_OF_LINE-1:0] dac_tdata,
     output dac_tvalid,
     output logic [2*16*NUMBER_OF_LINE-1:0] dbg_output_tdata
   );

  genvar i;
  logic [13:0] adc1_data_array[NUMBER_OF_LINE];
  logic [16*NUMBER_OF_LINE-1:0] adc1_data_i_shift;
  logic [16*NUMBER_OF_LINE-1:0] adc1_data_q_shift;
  logic [16*NUMBER_OF_LINE-1:0] adc2_data_i_shift;
  logic [16*NUMBER_OF_LINE-1:0] adc2_data_q_shift;
  logic [16*NUMBER_OF_LINE-1:0] adc3_data_i_shift;
  logic [16*NUMBER_OF_LINE-1:0] adc3_data_q_shift;
  logic [2*16*NUMBER_OF_LINE-1:0] output_data_temp;

  // rearrange input adc signal
  assign adc1_tready = 1;
  assign adc2_tready = 1;
  assign adc3_tready = 1;

  iq_freq_shift
    #(NUMBER_OF_LINE)
    iq_freq_shift_inst1
    (
      .clock(clock),
      .resetn(adc1_tvalid && resetn),
      .data_in_i(adc1_tdata),
      .data_in_q(),
      .dds_phase_inc(dds_phase_inc1),
      .data_out_i(adc1_data_i_shift),
      .data_out_q(adc1_data_q_shift)
    );

  iq_freq_shift
    #(NUMBER_OF_LINE)
    iq_freq_shift_inst2
    (
      .clock(clock),
      .resetn(adc3_tvalid && resetn),
      .data_in_i(adc2_tdata),
      .data_in_q(),
      .dds_phase_inc(dds_phase_inc2),
      .data_out_i(adc2_data_i_shift),
      .data_out_q(adc2_data_q_shift)
    );

  iq_freq_shift
    #(NUMBER_OF_LINE)
    iq_freq_shift_inst3
    (
      .clock(clock),
      .resetn(adc3_tvalid && resetn),
      .data_in_i(adc3_tdata),
      .data_in_q(),
      .dds_phase_inc(dds_phase_inc3),
      .data_out_i(adc3_data_i_shift),
      .data_out_q(adc3_data_q_shift)
    );

  always @(posedge clock)
  begin
    case (output_select)
      1:
        output_data_temp <= {adc1_data_q_shift,adc1_data_i_shift};
      2:
        output_data_temp <= {adc2_data_q_shift,adc2_data_i_shift};
      3:
        output_data_temp <= {adc3_data_q_shift,adc3_data_i_shift};
      default:
        output_data_temp <= {adc1_data_q_shift,adc1_data_i_shift};
    endcase
  end
  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      always @(posedge clock)
      begin
        dac_tdata[16*(2*i+1)-1:16*(2*i)] <= output_data_temp[16*(i+1)-1:16*i];
        dac_tdata[16*(2*i+2)-1:16*(2*i+1)]<= output_data_temp[16*(i+1)-1+128:16*i+128];
      end
    end
  endgenerate
  assign dbg_output_tdata = dac_tdata;
  assign dac_tvalid = 1;

endmodule
