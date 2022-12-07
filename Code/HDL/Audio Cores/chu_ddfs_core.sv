module chu_ddfs_core
   #(parameter PW=30)   // # DDFS bits
   (
    input  logic clk,
    input  logic reset,
    // slot interface
    input  logic cs,
    input  logic read,
    input  logic write,
    input  logic [4:0] addr,
    input  logic [31:0] wr_data,
    output logic [31:0] rd_data,
    // external signals 
    input  logic [PW-1:0] focw_ext, pha_ext,
    input  logic [15:0] env_ext,
    output logic digital_out,
    output logic pdm_out,
    output logic [15:0] pcm_out
   );

   // declaration
   logic [PW-1:0] pha_reg, fccw_reg, focw_reg;
   logic [PW-1:0] focw, pha;
   logic [15:0] env_reg;
   logic [15:0] env;
   logic [2:0] ctrl_reg;
   logic wr_en, wr_fccw, wr_focw, wr_pha, wr_env, wr_ctrl;
   logic [15:0] pcm, pcm1, pcm2, pcm3, pcm4, pcm_reg; 
   logic out1, out2, out3, out4, out_reg;
   
   logic [2:0] active_reg;
   
   // instantiate ddfs
   ddfs #(.PW(PW), .FILE_NAME("sin_table.txt")) ddfs_sin
      (.*, .fccw(fccw_reg), .pcm_out(pcm1), .pulse_out(out1));
   
   ddfs #(.PW(PW), .FILE_NAME("saw_table.txt")) ddfs_saw
      (.*, .fccw(fccw_reg), .pcm_out(pcm2), .pulse_out(out2));
          
   ddfs #(.PW(PW), .FILE_NAME("random_table.txt")) ddfs_random
      (.*, .fccw(fccw_reg), .pcm_out(pcm3), .pulse_out(out3));
   
   ddfs #(.PW(PW), .FILE_NAME("square_table.txt")) ddfs_square
      (.*, .fccw(fccw_reg), .pcm_out(pcm4), .pulse_out(out4));
      
   // instantiate 1-bit dac
   ds_1bit_dac #(.W(16)) dac_unit 
      (.*, .pcm_in(pcm));
   assign pcm_out = pcm;
   
   // registers
   always_ff @(posedge clk, posedge reset)
      if (reset) begin
         fccw_reg <= 0;
         focw_reg <= 0;
         pha_reg <= 0;
         env_reg <= 16'h4000;    // 1.00
         ctrl_reg <= 3'b000;
      end 
      else begin
         if (wr_fccw)
            fccw_reg <= wr_data[PW-1:0];
         if (wr_focw)
            focw_reg <= wr_data[PW-1:0];
         if (wr_pha)
            pha_reg <= wr_data[PW-1:0];
         if (wr_env)
            env_reg <= wr_data[15:0];
         if (wr_ctrl)
            ctrl_reg <= wr_data[2:0];
         if (wr_unit)
            active_reg <= wr_data[2:0];
      end
      
   always_comb
    begin
         case(active_reg)
         3'b001: 
            begin
                pcm_reg = pcm2;
                out_reg = out2;
             end
         3'b010: 
            begin
                pcm_reg = pcm3;
                out_reg = out3;
             end 
         3'b011: 
            begin
                pcm_reg = pcm4;
                out_reg = out4;
             end 
         default:
            begin
                pcm_reg = pcm1;
                out_reg = out1;
            end
        endcase
    end
   
   // decoding
   assign wr_en = write & cs;
   assign wr_fccw = (addr[2:0]==3'b000) & wr_en;
   assign wr_focw = (addr[2:0]==3'b001) & wr_en;
   assign wr_pha  = (addr[2:0]==3'b010) & wr_en;
   assign wr_env  = (addr[2:0]==3'b011) & wr_en;
   assign wr_ctrl = (addr[2:0]==3'b100) & wr_en;
   assign wr_unit = (addr[2:0]==3'b101) & wr_en;
   // input signal routing 
   assign env  =(ctrl_reg[0]) ? env_ext : env_reg;
   assign focw =(ctrl_reg[1]) ? focw_ext : focw_reg;
   assign pha  =(ctrl_reg[2]) ? pha_ext : pha_reg;
   assign digital_out = out_reg;
   assign pcm = pcm_reg;
   // read out
   assign rd_data = {16'h0000, pcm};
endmodule     


