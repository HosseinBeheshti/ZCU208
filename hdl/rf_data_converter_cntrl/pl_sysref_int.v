

module pl_sysref_int(
    input  wire pl_sysref_captured,
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

    always @(posedge pl_clk_buf) begin
        pl_sysref_r <= pl_sysref_captured;
        pl_sysref_r_1 <= pl_sysref_captured;
    end

    always @(posedge pl_clk_adc) begin
        user_sysref_adc <= pl_sysref_r;
        user_sysref_adc_1 <= pl_sysref_r_1;
    end
    
    always @(posedge pl_clk_dac) begin
        user_sysref_dac <= pl_sysref_r;
    end
    
endmodule


