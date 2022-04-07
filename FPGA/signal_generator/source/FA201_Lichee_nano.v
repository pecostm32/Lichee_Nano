//---------------------------------------------------------------------------
//Main module for connections with the outside world

module FA201_Lichee_nano
(
  //Input signals
  input wire i_xtal,                     //50 MHz clock
  input wire i_mcu_data_strobe,          //Active low going pulse from the mcu to strobe the data
  input wire i_mcu_control_srobe,        //Active low going pulse from the mcu to strobe the control

  //Parallel data bus from the mcu        
  input wire [7:0] i_mcu_data,

  //Output signals
  output o_dac1_clk,
  output o_dac1_wrt,
  output o_dac2_clk,
  output o_dac2_wrt,

  output wire [13:0] o_dac1_data,
  output wire [13:0] o_dac2_data
);

  //---------------------------------------------------------------------------
  //Internal wires

  wire core_clock;
  wire dac_clock;
    
  wire [47:0] channel1_negative_signal_step;
  wire [47:0] channel1_positive_signal_step;  
  wire [47:0] channel1_signal_phase_add;  
  wire [47:0] channel1_signal_control;  
  
  wire [12:0] channel1_sin_data;
  wire [11:0] channel1_sin_address;

  wire [47:0] channel2_negative_signal_step;
  wire [47:0] channel2_positive_signal_step;  
  wire [47:0] channel2_signal_phase_add;
  wire [47:0] channel2_signal_control;
  
  wire [12:0] channel2_sin_data;
  wire [11:0] channel2_sin_address;
  
  wire reset_signal_phase_registers;

  wire load_channel1_step_registers;
  wire add_channel1_signal_phase;

  wire load_channel2_step_registers;
  wire add_channel2_signal_phase;

  //---------------------------------------------------------------------------
  //Connection with the sub modules
  
  pll_clock pll  
  (  
    .refclk   (i_xtal),
    .reset    (1'b0),
    .clk0_out (core_clock),
    .clk1_out (dac_clock)
  );
  
  mcu_interface mcu  
  (  
    .i_main_clk                      (core_clock),
    .i_data_strobe                   (i_mcu_data_strobe),
    .i_control_strobe                (i_mcu_control_srobe),    
    .i_data                          (i_mcu_data),    
    .o_channel1_negative_signal_step (channel1_negative_signal_step),
    .o_channel1_positive_signal_step (channel1_positive_signal_step),    
    .o_channel1_signal_phase_add     (channel1_signal_phase_add),    
    .o_channel1_signal_control       (channel1_signal_control),
    .o_channel2_negative_signal_step (channel2_negative_signal_step),
    .o_channel2_positive_signal_step (channel2_positive_signal_step),    
    .o_channel2_signal_phase_add     (channel2_signal_phase_add),
    .o_channel2_signal_control       (channel2_signal_control),
    .o_reset_signal_phase_registers  (reset_signal_phase_registers),    
    .o_load_channel1_step_registers  (load_channel1_step_registers),    
    .o_add_channel1_signal_phase     (add_channel1_signal_phase),
    .o_load_channel2_step_registers  (load_channel2_step_registers),    
    .o_add_channel2_signal_phase     (add_channel2_signal_phase)
  );  
  
  awg dac1
  (
    .i_main_clock                   (core_clock),  
    .i_dac_clock                    (dac_clock),
    .i_negative_signal_step         (channel1_negative_signal_step),
    .i_positive_signal_step         (channel1_positive_signal_step),    
    .i_signal_phase_add             (channel1_signal_phase_add),    
    .i_signal_control               (channel1_signal_control),    
    .i_sin_data                     (channel1_sin_data),
    .i_reset_signal_phase_registers (reset_signal_phase_registers),    
    .i_load_step_registers          (load_channel1_step_registers),
    .i_add_signal_phase             (add_channel1_signal_phase),
    .o_dac_clk                      (o_dac1_clk),
    .o_dac_wrt                      (o_dac1_wrt),
    .o_dac_data                     (o_dac1_data),    
    .o_sin_address                  (channel1_sin_address)
  );

  awg dac2
  (
    .i_main_clock                   (core_clock),
    .i_dac_clock                    (dac_clock),
    .i_negative_signal_step         (channel2_negative_signal_step),
    .i_positive_signal_step         (channel2_positive_signal_step),
    .i_signal_phase_add             (channel2_signal_phase_add),
    .i_signal_control               (channel2_signal_control),
    .i_sin_data                     (channel2_sin_data),
    .i_reset_signal_phase_registers (reset_signal_phase_registers),
    .i_load_step_registers          (load_channel2_step_registers),
    .i_add_signal_phase             (add_channel2_signal_phase),
    .o_dac_clk                      (o_dac2_clk),
    .o_dac_wrt                      (o_dac2_wrt),
    .o_dac_data                     (o_dac2_data),
    .o_sin_address                  (channel2_sin_address)
  );
    
  sin_rom sin_data    
  (
    .clka    (core_clock),  
    .rsta    (1'b0),    
    .addra   (channel1_sin_address),    
    .doa     (channel1_sin_data),
    .clkb    (core_clock),  
    .rstb    (1'b0),    
    .addrb   (channel2_sin_address),    
    .dob     (channel2_sin_data)
  );    

endmodule

//---------------------------------------------------------------------------

