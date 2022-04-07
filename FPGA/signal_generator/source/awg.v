//----------------------------------------------------------------------------------
//Module for generating the DAC signals

module awg
(
  //Input
  input i_main_clock,
  input i_dac_clock,
    
  input [47:0] i_negative_signal_step,
  input [47:0] i_positive_signal_step,  
  input [47:0] i_signal_phase_add,  
  input [47:0] i_signal_control,
    
  input [12:0] i_sin_data,    

  input i_reset_signal_phase_registers,  
  input i_load_step_registers,  
  input i_add_signal_phase,

  //Output
  output wire o_dac_clk,
  output wire o_dac_wrt,  

  output wire [13:0] o_dac_data,
  
  output reg [11:0] o_sin_address
);

parameter INVERT_CLK = 1'b0 ; // Set to 1 to invert the clock output.

  //--------------------------------------------------------------------------------
  //Registers
  
  reg [47:0] signal_phase = 0;
  reg [47:0] negative_signal_step = 48'h200000000;
  reg [47:0] positive_signal_step = 48'h200000000;
  
  reg [13:0] dac_signal_data = 0;  
  
  reg [1:0] sin_quarter_data;  
  
  reg [13:0] sin_data[0:1];

  //--------------------------------------------------------------------------------
  //Logic
  
  always@(posedge i_main_clock)
    begin  
      //Check if the load step registers signal is active
      if(i_load_step_registers == 1'b1)    
        begin      
          //Update the step registers if so
          negative_signal_step <= i_negative_signal_step;
          positive_signal_step <= i_positive_signal_step;
        end          

      //Check if the reset the signals phase registers signal is active
      if(i_reset_signal_phase_registers == 1'b1)      
        //Clear the signal phase register if so
        signal_phase <= 48'h0;        
      //Check if the add to the phase signal is active
      else if(i_add_signal_phase == 1'b1)        
        //Add the signal phase step to the signal phase register      
        signal_phase <= signal_phase + i_signal_phase_add;
      else
        begin      
          //Check if the signal is in the negative output range
          if(signal_phase[47] == 1'b0)          
            //If so add the negative step to the phase
            signal_phase <= signal_phase + negative_signal_step;      
          else            
            //Else add the positive step to the phase
            signal_phase <= signal_phase + positive_signal_step;                
        end
    end
    
  //--------------------------------------------------------------------------------
  //Signal waveform select  
  
  always@(posedge i_main_clock)
    begin
      case(i_signal_control[2:0])    
        default:
          //Sine wave
          dac_signal_data <= sin_data[1];

        3'h1:
          //Square wave
          dac_signal_data <= signal_phase[47] ? 14'h3FFF : 14'h0;
          
        3'h2:
          //Ramp up saw tooth
          dac_signal_data <= signal_phase[47:34];
          
        3'h3:
          //Ramp down sawtooth
          dac_signal_data <= ~signal_phase[47:34];

        3'h4:          
          //Triangle wave
          begin
            //For triangle wave the top two bits are used for quarter detection
            case(signal_phase[47:46])
              2'h0:
                //For the first quarter go down from mid level
                dac_signal_data <= 14'd8192 - signal_phase[45:33];

              2'h1:
                //In the second quarter go back up to mid level            
                dac_signal_data <= signal_phase[45:33];

              2'h2:
                //In the third quarter go up from mid level
                dac_signal_data <= 14'd8192 + signal_phase[45:33];

              2'h3:
                //In the fourth quarter go back down to mid level
                dac_signal_data <= 14'd16383 - signal_phase[45:33];
            endcase
          end

        3'h5:
          //Saw tooth with adjustable ramp        
          //On 50% setting it is a triangle wave
          dac_signal_data <= signal_phase[47] ? signal_phase[46:33] : ~signal_phase[46:33]; 
      endcase
    end
    
  //--------------------------------------------------------------------------------
  //Sine generation
    
  always@(posedge i_main_clock)
    begin   
      //Save the current quarter info for usage at the end of the pipeline
      sin_quarter_data[0] <= signal_phase[47];
     
      //Set the index for the next table output. For q1 and q3 forward, for q2 and q4 backward
      o_sin_address <= signal_phase[46] ? ~signal_phase[45:34] : signal_phase[45:34];

      //Load the previous quarter through the pipeline
      sin_quarter_data[1] <= sin_quarter_data[0];       
       
      //Latch the data from the lookup table       
      sin_data[0] <= i_sin_data;       
       
      //Process the previous data for output based on the belonging half       
      sin_data[1] <= sin_quarter_data[1] ? (14'd8192 + sin_data[0]) : (14'd8191 - sin_data[0]);
    end
    
  //--------------------------------------------------------------------------------
  //Output  
  //DDR logic is used to allow for proper synchronous switching of the signals  

  //Output the data to the DAC output registers
  output_ddr dac_data[13:0]    
  (  
    .clk   (i_main_clock),  
    .rst   (1'b0),
    .d1    (dac_signal_data),
    .d2    (dac_signal_data),    
    .q     (o_dac_data)
  );    
  
  //Clock the DAC write signal to load the DAC latch
  output_ddr dac_wrt
  (  
    .clk   (i_dac_clock),  
    .rst   (1'b0),
    .d1    (INVERT_CLK),
    .d2    (!INVERT_CLK),    
    .q     (o_dac_wrt)
  );  
    
  //Clock the DAC clock signal with a 180 degree phase shift to load the DAC with the latched data
  output_ddr dac_clk
  (  
    .clk   (i_dac_clock),  
    .rst   (1'b0),
    .d1    (!INVERT_CLK),
    .d2    (INVERT_CLK),    
    .q     (o_dac_clk)
  );  

endmodule

//----------------------------------------------------------------------------------

