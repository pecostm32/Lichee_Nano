//---------------------------------------------------------------------------
//MCU interface for passing data in and out the FPGA based on a command set


module mcu_interface
(
  //Main clock input
  input i_main_clk,            //125MHz

  //Control input
  input i_data_strobe,         //Data strobe signal:            Pulse from the mcu to clock data
  input i_control_strobe,      //Control strobe signal:         Pulse from the mcu to strobe the data into the addressed control register

  //External data bus
  input [7:0] i_data,
  
  //Output to other modules
  output [47:0] o_channel1_negative_signal_step,
  output [47:0] o_channel1_positive_signal_step,  
  output [47:0] o_channel1_signal_phase_add,  
  output [47:0] o_channel1_signal_control,

  output [47:0] o_channel2_negative_signal_step,
  output [47:0] o_channel2_positive_signal_step,  
  output [47:0] o_channel2_signal_phase_add,
  output [47:0] o_channel2_signal_control,
  
  output o_reset_signal_phase_registers,  

  output o_load_channel1_step_registers,  
  output o_add_channel1_signal_phase,  

  output o_load_channel2_step_registers,
  output o_add_channel2_signal_phase
);

  //---------------------------------------------------------------------------
  //Control data latch addresses
  //
  //Channel 1
  //0x00 ==> Step value for negative signal part
  //0x01 ==> Step value for positive signal part  
  //0x02 ==> Signal phase add value  
  //0x03 ==> Signal control (Wavform select)
  //
  //Channel 2
  //0x04 ==> Step value for negative signal part
  //0x05 ==> Step value for positive signal part  
  //0x06 ==> Signal phase add value  
  //0x07 ==> Signal control (Wavform select)  
  //  
  //Control action addresses  
  //
  //Channel 1
  //0x08 ==> Load the step registers with the new values
  //0x09 ==> Add the signal phase add value to the signal phase register
  //
  //Channel 2
  //0x0C ==> Load the step registers with the new values
  //0x0D ==> Add the signal phase add value to the signal phase register
  //
  //Global
  //0x0F ==> Reset both the signal phase registers

  //---------------------------------------------------------------------------
  //Internal registers

  reg ireg_i_data_strobe    = 1;
  reg ireg_i_control_strobe = 1;

  reg dly_i_data_strobe    = 1;
  reg dly_i_control_strobe = 1;

  reg [7:0] ireg_i_data = 0;

  reg [47:0] long_data_reg = 0;

  reg [47:0] control_data_reg[0:7];
  
  reg [7:0] control_strobe;
  
  //---------------------------------------------------------------------------
  //Synchronize to fpga clock system

  //---------------------------------------------------------------------------
  //Create the enable signals

  //Every time the dly_i_data_strobe transitions low load and shift in the new 8 bit data.
  wire shift_8_48_data = ( !ireg_i_data_strobe && dly_i_data_strobe );

  //Every time the dly_i_control_strobe transitions low and the control register address is in range, load the data into it's target register
  wire load_into_control_reg = ( !ireg_i_control_strobe && dly_i_control_strobe ) && ( ireg_i_data < 8 );
  
  //Every time the dly_i_control_strobe transitions low and the control address is in range set a control strobe
  wire set_control_strobe = ( !ireg_i_control_strobe && dly_i_control_strobe ) && ( ireg_i_data >= 8 ) && ( ireg_i_data < 16 );

  //---------------------------------------------------------------------------
  //Sample the mcu signals on the up going edge of the main clock  

  always@(posedge i_main_clk)
    begin
      //Load the MCU signals into the first stage
      ireg_i_data_strobe       <= i_data_strobe;
      ireg_i_control_strobe    <= i_control_strobe;
      ireg_i_data              <= i_data;

      //Load the signals into the second stage
      dly_i_data_strobe    <= ireg_i_data_strobe;
      dly_i_control_strobe <= ireg_i_control_strobe;
            
      //When enabled shift the MCU data into the temporary register
      if(shift_8_48_data) long_data_reg <= { long_data_reg[39:0], ireg_i_data[7:0] };      
      
      //When enabled load the received data into the intended control register
      if(load_into_control_reg) control_data_reg[ireg_i_data[2:0]] <= long_data_reg;      
      
      //When enables set the intended control strobe
      if(set_control_strobe)            
        control_strobe[ireg_i_data[2:0]] <= 1'b1;      
      else
        //Make sure the strobes are only valid vor a single clock pulse
        control_strobe <= 8'h0;
    end

  //---------------------------------------------------------------------------
  //Connect to the other modules
  
  //Channel 1 data
  assign o_channel1_negative_signal_step = control_data_reg[0];
  assign o_channel1_positive_signal_step = control_data_reg[1];
  assign o_channel1_signal_phase_add     = control_data_reg[2];
  assign o_channel1_signal_control       = control_data_reg[3];
  
  //Channel 2 data
  assign o_channel2_negative_signal_step = control_data_reg[4];
  assign o_channel2_positive_signal_step = control_data_reg[5];  
  assign o_channel2_signal_phase_add     = control_data_reg[6];
  assign o_channel2_signal_control       = control_data_reg[7];
  
  //Channel 1 control
  assign o_load_channel1_step_registers = control_strobe[0];
  assign o_add_channel1_signal_phase    = control_strobe[1];

  //Channel 2 control
  assign o_load_channel2_step_registers = control_strobe[4];
  assign o_add_channel2_signal_phase    = control_strobe[5];
    
  //Global control
  assign o_reset_signal_phase_registers = control_strobe[7];  

endmodule

//---------------------------------------------------------------------------

