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

module adc_dac_monitor
  #(
     parameter NUMBER_OF_LINE = 8
   )
   (
     input clock,
     // adcx: tile xxx
     input adc0_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc0_in_tdata,
     output adc0_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc0_data,
     // adcx: tile xxx
     input adc1_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc1_in_tdata,
     output adc1_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc1_data,
     // adcx: tile xxx
     input adc2_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc2_in_tdata,
     output adc2_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc2_data,
     // adcx: tile xxx
     input adc3_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc3_in_tdata,
     output adc3_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc3_data,
     // adcx: tile xxx
     input adc4_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc4_in_tdata,
     output adc4_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc4_data,
     // adcx: tile xxx
     input adc5_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc5_in_tdata,
     output adc5_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc5_data,
     // adcx: tile xxx
     input adc6_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc6_in_tdata,
     output adc6_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc6_data,
     // adcx: tile xxx
     input adc7_in_tvalid,
     input [16*NUMBER_OF_LINE-1:0] adc7_in_tdata,
     output adc7_in_tready,
     output reg [16*NUMBER_OF_LINE-1:0] adc7_data,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac0_data,
     output reg dac0_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac0_out_tdata,
     input dac0_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac1_data,
     output reg dac1_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac1_out_tdata,
     input dac1_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac2_data,
     output reg dac2_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac2_out_tdata,
     input dac2_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac3_data,
     output reg dac3_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac3_out_tdata,
     input dac3_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac4_data,
     output reg dac4_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac4_out_tdata,
     input dac4_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac5_data,
     output reg dac5_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac5_out_tdata,
     input dac5_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac6_data,
     output reg dac6_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac6_out_tdata,
     input dac6_out_tready,
     // dacx: tile xxx
     input [16*NUMBER_OF_LINE-1:0] dac7_data,
     output reg dac7_out_tvalid,
     output reg [16*NUMBER_OF_LINE-1:0] dac7_out_tdata,
     input dac7_out_tready,
     input [2:0] adc_monitor_select,
     output [15:0] adc_line_0,
     output [15:0] adc_line_1,
     output [15:0] adc_line_2,
     output [15:0] adc_line_3,
     output [15:0] adc_line_4,
     output [15:0] adc_line_5,
     output [15:0] adc_line_6,
     output [15:0] adc_line_7,
     input [2:0] dac_monitor_select,
     output [15:0] dac_line_0,
     output [15:0] dac_line_1,
     output [15:0] dac_line_2,
     output [15:0] dac_line_3,
     output [15:0] dac_line_4,
     output [15:0] dac_line_5,
     output [15:0] dac_line_6,
     output [15:0] dac_line_7
   );

  reg [16*NUMBER_OF_LINE-1:0] adc0_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc1_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc2_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc3_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc4_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc5_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc6_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc7_data_temp;
  reg [16*NUMBER_OF_LINE-1:0] adc_monitor_data;
  reg [16*NUMBER_OF_LINE-1:0] dac_monitor_data;

  rf_axis_data_fifo rf_axis_data_fifo_adc0
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc0_in_tvalid),
                      .s_axis_tready(adc0_in_tready),
                      .s_axis_tdata(adc0_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc0_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc1
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc1_in_tvalid),
                      .s_axis_tready(adc1_in_tready),
                      .s_axis_tdata(adc1_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc1_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc2
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc2_in_tvalid),
                      .s_axis_tready(adc2_in_tready),
                      .s_axis_tdata(adc2_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc2_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc3
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc3_in_tvalid),
                      .s_axis_tready(adc3_in_tready),
                      .s_axis_tdata(adc3_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc3_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc4
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc4_in_tvalid),
                      .s_axis_tready(adc4_in_tready),
                      .s_axis_tdata(adc4_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc4_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc5
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc5_in_tvalid),
                      .s_axis_tready(adc5_in_tready),
                      .s_axis_tdata(adc5_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc5_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc6
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc6_in_tvalid),
                      .s_axis_tready(adc6_in_tready),
                      .s_axis_tdata(adc6_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc6_data_temp)
                    );

  rf_axis_data_fifo rf_axis_data_fifo_adc7
                    (
                      .s_axis_aresetn(1),
                      .s_axis_aclk(clock),
                      .s_axis_tvalid(adc7_in_tvalid),
                      .s_axis_tready(adc7_in_tready),
                      .s_axis_tdata(adc7_in_tdata),
                      .m_axis_tvalid(),
                      .m_axis_tready(1),
                      .m_axis_tdata(adc7_data_temp)
                    );

  always @(posedge clock)
  begin
    adc0_data <= adc0_data_temp;
    adc1_data <= adc1_data_temp;
    adc2_data <= adc2_data_temp;
    adc3_data <= adc3_data_temp;
    adc4_data <= adc4_data_temp;
    adc5_data <= adc5_data_temp;
    adc6_data <= adc6_data_temp;
    adc7_data <= adc7_data_temp;
    case (adc_monitor_select)
      0:
        adc_monitor_data <= adc0_data_temp;
      1:
        adc_monitor_data <= adc1_data_temp;
      2:
        adc_monitor_data <= adc2_data_temp;
      3:
        adc_monitor_data <= adc3_data_temp;
      4:
        adc_monitor_data <= adc4_data_temp;
      5:
        adc_monitor_data <= adc5_data_temp;
      6:
        adc_monitor_data <= adc6_data_temp;
      7:
        adc_monitor_data <= adc7_data_temp;
      default:
        adc_monitor_data <= adc0_data_temp;
    endcase
  end

  assign adc_line_0 = adc_monitor_data[16*(1)-1:16*(0)];
  assign adc_line_1 = adc_monitor_data[16*(2)-1:16*(1)];
  assign adc_line_2 = adc_monitor_data[16*(3)-1:16*(2)];
  assign adc_line_3 = adc_monitor_data[16*(4)-1:16*(3)];
  assign adc_line_4 = adc_monitor_data[16*(5)-1:16*(4)];
  assign adc_line_5 = adc_monitor_data[16*(6)-1:16*(5)];
  assign adc_line_6 = adc_monitor_data[16*(7)-1:16*(6)];
  assign adc_line_7 = adc_monitor_data[16*(8)-1:16*(7)];

  always @(posedge clock)
  begin
    dac0_out_tdata <= dac0_data;
    dac1_out_tdata <= dac1_data;
    dac2_out_tdata <= dac2_data;
    dac3_out_tdata <= dac3_data;
    dac4_out_tdata <= dac4_data;
    dac5_out_tdata <= dac5_data;
    dac6_out_tdata <= dac6_data;
    dac7_out_tdata <= dac7_data;
    dac0_out_tvalid <= 1;
    dac1_out_tvalid <= 1;
    dac2_out_tvalid <= 1;
    dac3_out_tvalid <= 1;
    dac4_out_tvalid <= 1;
    dac5_out_tvalid <= 1;
    dac6_out_tvalid <= 1;
    dac7_out_tvalid <= 1;
    case (dac_monitor_select)
      0:
        dac_monitor_data <= dac0_data;
      1:
        dac_monitor_data <= dac1_data;
      2:
        dac_monitor_data <= dac2_data;
      3:
        dac_monitor_data <= dac3_data;
      4:
        dac_monitor_data <= dac4_data;
      5:
        dac_monitor_data <= dac5_data;
      6:
        dac_monitor_data <= dac6_data;
      7:
        dac_monitor_data <= dac7_data;
      default:
        dac_monitor_data <= dac0_data;
    endcase
  end

  assign dac_line_0 = dac_monitor_data[16*(1)-1:16*(0)];
  assign dac_line_1 = dac_monitor_data[16*(2)-1:16*(1)];
  assign dac_line_2 = dac_monitor_data[16*(3)-1:16*(2)];
  assign dac_line_3 = dac_monitor_data[16*(4)-1:16*(3)];
  assign dac_line_4 = dac_monitor_data[16*(5)-1:16*(4)];
  assign dac_line_5 = dac_monitor_data[16*(6)-1:16*(5)];
  assign dac_line_6 = dac_monitor_data[16*(7)-1:16*(6)];
  assign dac_line_7 = dac_monitor_data[16*(8)-1:16*(7)];

endmodule
