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
// pg269-rf-data-converter.pdf figure 136
//////////////////////////////////////////////////////////////////////////////////

module clock_signaling
  (
    input sysref_in_n,
    input sysref_in_p,
    input pl_clk_n,
    input pl_clk_p,
    (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 master_clock CLK" *)
    (* X_INTERFACE_PARAMETER = "FREQ_HZ 500000000" *)
    output master_clock,
    (* dont_touch="true" *)
    output reg user_sysref_adc,
    (* dont_touch="true" *)
    output reg user_sysref_dac
  );

  wire pl_sysref_clk;
  wire pl_clk;

  IBUFDS #(
           .DIFF_TERM("FALSE"),
           .IBUF_LOW_PWR("FALSE"),
           .IOSTANDARD("DEFAULT")
         ) IBUFDS_inst1 (
           .O(pl_sysref_clk),
           .I(sysref_in_p),
           .IB(sysref_in_n)
         );

  IBUFDS #(
           .DIFF_TERM("FALSE"),
           .IBUF_LOW_PWR("FALSE"),
           .IOSTANDARD("DEFAULT")
         ) IBUFDS_inst2 (
           .O(pl_clk),
           .I(pl_clk_p),
           .IB(pl_clk_n)
         );

  BUFG BUFG_inst (
         .O(master_clock),
         .I(pl_clk)
       );

  always @(posedge master_clock)
  begin
    user_sysref_adc <=  pl_sysref_clk;
    user_sysref_dac <=  pl_sysref_clk;
  end

endmodule
