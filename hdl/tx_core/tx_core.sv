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
     input [4:0] output_select,
     input [15:0] lo_dds_phase_inc,
     input [16*NUMBER_OF_LINE-1:0] adc1_data,
     input [16*NUMBER_OF_LINE-1:0] adc2_data,
     input [16*NUMBER_OF_LINE-1:0] adc3_data,
     output logic [16*NUMBER_OF_LINE-1:0] dac_data
   );

  genvar i;
  logic [15:0] adc1_data_array[NUMBER_OF_LINE];
  logic [15:0] adc2_data_array[NUMBER_OF_LINE];
  logic [15:0] adc3_data_array[NUMBER_OF_LINE];
  logic [15:0] phase_acc_value = 0;
  logic [31:0] dds_data_array[NUMBER_OF_LINE];
  logic [15:0] dds_sin_array[NUMBER_OF_LINE];
  logic [15:0] dds_cosin_array[NUMBER_OF_LINE];
  logic [15:0] sin_mult_data_array[NUMBER_OF_LINE];
  logic [15:0] cosin_mult_data_array[NUMBER_OF_LINE];
  logic [15:0] adder1_data_array[NUMBER_OF_LINE];
  logic [15:0] adder2_data_array[NUMBER_OF_LINE];
  logic [15:0] data_out_array[NUMBER_OF_LINE];

  // generate NUMBER_OF_LINE-line 4GSPS DDS signal
  always @(posedge clock)
  begin
    phase_acc_value <= phase_acc_value + 8*lo_dds_phase_inc;
  end

  generate
    for (i = 0; i < NUMBER_OF_LINE; i = i + 1)
    begin
      // rearrange input adc signal
      always @(posedge clock)
      begin
        adc1_data_array[i] <= adc1_data[16*(i+1)-1:16*i];
        adc2_data_array[i] <= adc2_data[16*(i+1)-1:16*i];
        adc3_data_array[i] <= adc3_data[16*(i+1)-1:16*i];
      end

      dds_compiler_core	dds_compiler_core_inst
                        (
                          .aclk(clock),
                          .s_axis_phase_tvalid(resetn),
                          .s_axis_phase_tdata(phase_acc_value + i*lo_dds_phase_inc),
                          .m_axis_data_tvalid(),
                          .m_axis_data_tdata(dds_data_array[i])
                        );

      always @(posedge clock)
      begin
        dds_sin_array[i] <= dds_data_array[i][15:0];
        dds_cosin_array[i] <= dds_data_array[i][31:16];
      end

      tx_lo_mult_gen  tx_lo_mult_gen_sin_inst
                      (
                        .CLK(clock),
                        .A(dds_sin_array[i]),
                        .B(adc1_data_array[i]),
                        .P(sin_mult_data_array[i])
                      );

      tx_lo_mult_gen  tx_lo_mult_gen_cosin_inst
                      (
                        .CLK(clock),
                        .A(dds_cosin_array[i]),
                        .B(adc2_data_array[i]),
                        .P(cosin_mult_data_array[i])
                      );

      tx_sum_addsub  tx_sum_addsub_inst1
                     (
                       A(sin_mult_data_array[i]),
                       B(cosin_mult_data_array[i]),
                       CLK(clock),
                       S(adder1_data_array[i])
                     );

      tx_sum_addsub  tx_sum_addsub_inst2
                     (
                       A(adder1_data_array[i]),
                       B(adc3_data_array[i]),
                       CLK(clock),
                       S(adder2_data_array[i])
                     );

      always @(posedge clock)
      begin
        case (output_select)
          1:
            data_out_array[i] <= adc1_data_array[i];
          2:
            data_out_array[i] <= adc2_data_array[i];
          3:
            data_out_array[i] <= adc3_data_array[i];
          4:
            data_out_array[i] <= sin_mult_data_array[i];
          5:
            data_out_array[i] <= cosin_mult_data_array[i];
          6:
            data_out_array[i] <= adder1_data_array[i];
          7:
            data_out_array[i] <= adder2_data_array[i];
          default:
            data_out_array[i] <= adder2_data_array[i];
        endcase
      end

      always @(posedge clock)
      begin
        dac_data[16*(i+1)-1:16*i] <= data_out_array[i];
      end

    end
  endgenerate

endmodule
