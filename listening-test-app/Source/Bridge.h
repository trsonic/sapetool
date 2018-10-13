/*
  ==============================================================================

    Bridge.h
    Created: 12 May 2017 12:26:25pm
    Author:  Jacek

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "rs232.h"

class Bridge //: private HighResolutionTimer
{
public:
    
    Bridge();    
    ~Bridge();
        
    int BaudR;
    int PortN;
    
    int port_number, port_index, port_state;
    
    char read_buffer[1];
	String buffer_string = "0";

    bool connected = false;
	bool Az_dec_state = false, Az_inc_state = false, El_dec_state = false, El_inc_state = false, AzEl_reset = false;
	bool Play_reference = false, Play_test = false, Go_previous = false, Go_next = false;
   
    StringPairArray portlist;
    StringArray GetPortInfo();
    bool Connect();
    void Disconnect();
	void Read();

    
    //void hiResTimerCallback() override;
       
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bridge)
};        
