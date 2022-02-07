

module pl_sysref(
    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_sysref CLK_P" *) input wire pl_sysref_p,
    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_sysref CLK_N" *) input wire pl_sysref_n,
    
    input  wire pl_clk_adc,  
    input  wire pl_clk_dac,  
    
    input wire  pl_clk_buf,
    output reg  user_sysref_adc,          
    output reg  user_sysref_adc_1,          
    output reg  user_sysref_dac          
);       

    wire pl_sysref_i;
    reg pl_sysref_r;
    reg pl_sysref_r_1;

    IBUFDS IBUFDS_sysref_inst (
        .O(pl_sysref_i),   // 1-bit output: Buffer output
        .I(pl_sysref_p),   // 1-bit input: Diff_p buffer input (connect directly to top-level port)
        .IB(pl_sysref_n)  // 1-bit input: Diff_n buffer input (connect directly to top-level port)
    );

    always @(posedge pl_clk_buf) begin
        pl_sysref_r <= pl_sysref_i;
        pl_sysref_r_1 <= pl_sysref_i;
    end

    always @(posedge pl_clk_adc) begin
        user_sysref_adc <= pl_sysref_r;
        user_sysref_adc_1 <= pl_sysref_r_1;
    end
    
    always @(posedge pl_clk_dac) begin
        user_sysref_dac <= pl_sysref_r;
    end
    
endmodule

//module pl_sysref(
//    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_clk CLK_P" *) input wire pl_clk_p,
//    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_clk CLK_N" *) input wire pl_clk_n,
      
//    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_sysref CLK_P" *) input wire pl_sysref_p,
//    (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pl_sysref CLK_N" *) input wire pl_sysref_n,
    
//    input  wire pl_clk_adc,  
//    input  wire pl_clk_dac,  
    
//    output wire  pl_clk_buf,
//    output reg  user_sysref_adc,          
//    output reg  user_sysref_adc_1,          
//    output reg  user_sysref_dac          
//);       

//    wire pl_sysref_i;
//    reg pl_sysref_r;
//    reg pl_sysref_r_1;

//    IBUFDS IBUFDS_sysref_inst (
//        .O(pl_sysref_i),   // 1-bit output: Buffer output
//        .I(pl_sysref_p),   // 1-bit input: Diff_p buffer input (connect directly to top-level port)
//        .IB(pl_sysref_n)  // 1-bit input: Diff_n buffer input (connect directly to top-level port)
//    );

//    IBUFDS IBUFDS_pl_clk_inst (
//        .O(pl_clk_buf),   // 1-bit output: Buffer output
//        .I(pl_clk_p),   // 1-bit input: Diff_p buffer input (connect directly to top-level port)
//        .IB(pl_clk_n)  // 1-bit input: Diff_n buffer input (connect directly to top-level port)
//    );

//    always @(posedge pl_clk_buf) begin
//        pl_sysref_r <= pl_sysref_i;
//        pl_sysref_r_1 <= pl_sysref_i;
//    end

//    always @(posedge pl_clk_adc) begin
//        user_sysref_adc <= pl_sysref_r;
//        user_sysref_adc_1 <= pl_sysref_r_1;
//    end
    
//    always @(posedge pl_clk_dac) begin
//        user_sysref_dac <= pl_sysref_r;
//    end
    
//endmodule
