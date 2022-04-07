create_clock -name core_clock -period 8 -waveform {0 4} [get_nets core_clock]
create_clock -name dac_clock -period 8 -waveform {0 4} [get_nets dac_clock]
create_clock -name i_xtal -period 20 -waveform {0 10}  [get_ports i_xtal]



set_clock_latency  -source 1 [get_clocks {i_xtal}]



