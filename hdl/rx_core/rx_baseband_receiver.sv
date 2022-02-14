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

module rx_baseband_receiver
  (
    input clock,
    input resetn,
    input [15:0] data_in,
    input [15:0] compelex_phase_inc,
    input [15:0] real_phase_inc,
    (* ASYNC_REG="true" *)
    output logic [15:0] data_out1_i,
    (* ASYNC_REG="true" *)
    output logic [15:0] data_out1_q,
    (* ASYNC_REG="true" *)
    output logic [15:0] data_out2,
    (* ASYNC_REG="true" *)
    output logic [15:0] data_out3
  );

  genvar i;
  parameter NUMBER_OF_PATH = 4;
  logic process_clock;
  logic [7:0] down_counter;
  logic down_strobe;
  logic [15:0] data_in_array[NUMBER_OF_PATH];
  (* ASYNC_REG="true" *)
  logic [16*NUMBER_OF_PATH-1:0] data_in_down4;
  (* ASYNC_REG="true" *)
  logic [16*NUMBER_OF_PATH-1:0] data_in_down4_reg;
  logic [16*NUMBER_OF_PATH-1:0] dds_sin1;
  logic [16*NUMBER_OF_PATH-1:0] dds_cosin1;
  logic [16*NUMBER_OF_PATH-1:0] dds_sin2;
  logic [16*NUMBER_OF_PATH-1:0] dds_cosin2;
  logic [2*16*NUMBER_OF_PATH-1:0] cm1_data_array;
  logic [2*16*NUMBER_OF_PATH-1:0] cm2_data_array;
  logic [2*16*NUMBER_OF_PATH-1:0] cm3_data_array;
  logic [2*16*NUMBER_OF_PATH-1:0] lpf1_data1_array;
  logic [2*16*NUMBER_OF_PATH-1:0] lpf1_data2_array;
  logic [2*16*NUMBER_OF_PATH-1:0] lpf1_data3_array;
  (* ASYNC_REG="true" *)
  logic [15:0] data_out1_i_temp;
  (* ASYNC_REG="true" *)
  logic [15:0] data_out1_q_temp;
  (* ASYNC_REG="true" *)
  logic [15:0] data_out2_temp;
  (* ASYNC_REG="true" *)
  logic [15:0] data_out3_temp;

  always @(posedge clock)
  begin
    data_in_array[0] <= data_in;
    data_in_array[1] <= data_in_array[0];
    data_in_array[2] <= data_in_array[1];
    data_in_array[3] <= data_in_array[2];
    if (down_counter < NUMBER_OF_PATH -1)
    begin
      down_counter <= down_counter + 1;
      down_strobe <= 0;
    end
    else
    begin
      down_counter <= 0;
      down_strobe <= 1;
    end
  end

  BUFGCE_DIV #(
               .BUFGCE_DIVIDE(4),
               .IS_CE_INVERTED(1'b0),
               .IS_CLR_INVERTED(1'b0),
               .IS_I_INVERTED(1'b0),
               .SIM_DEVICE("ULTRASCALE_PLUS")
             )
             BUFGCE_DIV_inst (
               .O(process_clock),
               .CE(1'b1),
               .CLR(1'b0),
               .I(clock)
             );

  // 1 -> NUMBER_OF_PATH
  always @(posedge process_clock)
  begin
    data_in_down4 <= {data_in_array[3],data_in_array[2],data_in_array[1],data_in_array[0]};
    data_in_down4_reg <= data_in_down4;
  end

  dds_multipath
    #(.NUMBER_OF_LINE(NUMBER_OF_PATH)
     )
    dds_multipath_inst1
    (
      .clock(clock),
      .resetn(resetn),
      .dds_phase_inc(compelex_phase_inc),
      .sin_out_i(dds_sin1),
      .cosin_out_q(dds_cosin1)
    );

  dds_multipath
    #(.NUMBER_OF_LINE(NUMBER_OF_PATH)
     )
    dds_multipath_inst2
    (
      .clock(clock),
      .resetn(resetn),
      .dds_phase_inc(real_phase_inc),
      .sin_out_i(dds_sin2),
      .cosin_out_q(dds_cosin2)
    );

  generate
    for (i = 0; i < NUMBER_OF_PATH; i = i + 1)
    begin
      cmult_core cmult_core_inst1
                 (
                   .aclk(clock),
                   .s_axis_a_tvalid(resetn),
                   .s_axis_a_tdata({16'd0,data_in_down4_reg[16*(i+1)-1:16*i]}),
                   .s_axis_b_tvalid(resetn),
                   .s_axis_b_tdata({dds_sin1[16*(i+1)-1:16*i],dds_cosin1[16*(i+1)-1:16*i]}),
                   .m_axis_dout_tvalid(),
                   .m_axis_dout_tdata(cm1_data_array[32*(i+1)-1:32*i])
                 );
      cmult_core cmult_core_inst2
                 (
                   .aclk(clock),
                   .s_axis_a_tvalid(resetn),
                   .s_axis_a_tdata({16'd0,data_in_down4_reg[16*(i+1)-1:16*i]}),
                   .s_axis_b_tvalid(resetn),
                   .s_axis_b_tdata({16'd0,dds_sin2[16*(i+1)-1:16*i]}),
                   .m_axis_dout_tvalid(),
                   .m_axis_dout_tdata(cm2_data_array[32*(i+1)-1:32*i])
                 );
      cmult_core cmult_core_inst3
                 (
                   .aclk(clock),
                   .s_axis_a_tvalid(resetn),
                   .s_axis_a_tdata({16'd0,data_in_down4_reg[16*(i+1)-1:16*i]}),
                   .s_axis_b_tvalid(resetn),
                   .s_axis_b_tdata({16'd0,dds_cosin2[16*(i+1)-1:16*i]}),
                   .m_axis_dout_tvalid(),
                   .m_axis_dout_tdata(cm3_data_array[32*(i+1)-1:32*i])
                 );
    end
  endgenerate

  // 4 path fir
  fir_compiler_4path fir_compiler_4path_ch1_i
                     (
                       .clk(process_clock),
                       .concat_data_in({cm1_data_array[111:96],cm1_data_array[79:64],cm1_data_array[47:32],cm1_data_array[15:0]}),
                       .concat_data_out({lpf1_data1_array[111:96],lpf1_data1_array[79:64],lpf1_data1_array[47:32],lpf1_data1_array[15:0]})
                     );

  fir_compiler_4path fir_compiler_4path_ch1_q
                     (
                       .clk(process_clock),
                       .concat_data_in({cm1_data_array[111+16:96+16],cm1_data_array[79+16:64+16],cm1_data_array[47+16:32+16],cm1_data_array[15+16:0+16]}),
                       .concat_data_out({lpf1_data1_array[111+16:96+16],lpf1_data1_array[79+16:64+16],lpf1_data1_array[47+16:32+16],lpf1_data1_array[15+16:0+16]})
                     );

  fir_compiler_4path fir_compiler_4path_ch2
                     (
                       .clk(process_clock),
                       .concat_data_in({cm2_data_array[111:96],cm2_data_array[79:64],cm2_data_array[47:32],cm2_data_array[15:0]}),
                       .concat_data_out({lpf1_data2_array[111:96],lpf1_data2_array[79:64],lpf1_data2_array[47:32],lpf1_data2_array[15:0]})
                     );

  fir_compiler_4path fir_compiler_4path_ch3
                     (
                       .clk(process_clock),
                       .concat_data_in({cm3_data_array[111:96],cm3_data_array[79:64],cm3_data_array[47:32],cm3_data_array[15:0]}),
                       .concat_data_out({lpf1_data3_array[111:96],lpf1_data3_array[79:64],lpf1_data3_array[47:32],lpf1_data3_array[15:0]})
                     );

  // NUMBER_OF_PATH -> 1
  always @(posedge clock)
  begin
    case (down_counter)
      0:
        data_out1_i_temp <= lpf1_data1_array[15:0];
      1:
        data_out1_i_temp <= lpf1_data1_array[47:32];
      2:
        data_out1_i_temp <= lpf1_data1_array[79:64];
      3:
        data_out1_i_temp <= lpf1_data1_array[111:96];
    endcase
    case (down_counter)
      0:
        data_out1_q_temp <= lpf1_data1_array[15+16:0+16];
      1:
        data_out1_q_temp <= lpf1_data1_array[47+16:32+16];
      2:
        data_out1_q_temp <= lpf1_data1_array[79+16:64+16];
      3:
        data_out1_q_temp <= lpf1_data1_array[111+16:96+16];
    endcase
    case (down_counter)
      0:
        data_out2_temp <= lpf1_data2_array[15:0];
      1:
        data_out2_temp <= lpf1_data2_array[47:32];
      2:
        data_out2_temp <= lpf1_data2_array[79:64];
      3:
        data_out2_temp <= lpf1_data2_array[111:96];
    endcase
    case (down_counter)
      0:
        data_out3_temp <= lpf1_data3_array[15:0];
      1:
        data_out3_temp <= lpf1_data3_array[47:32];
      2:
        data_out3_temp <= lpf1_data3_array[79:64];
      3:
        data_out3_temp <= lpf1_data3_array[111:96];
    endcase
    data_out1_i <= data_out1_i_temp;
    data_out1_q <= data_out1_q_temp;
    data_out2 <= data_out2_temp;
    data_out3 <= data_out3_temp;
  end
endmodule
