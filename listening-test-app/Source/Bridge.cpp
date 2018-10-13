/*
  ==============================================================================

    Bridge.cpp
    Created: 12 May 2017 12:26:25pm
    Author:  Jacek

  ==============================================================================
*/

#include "Bridge.h"

Bridge::Bridge()
{
    BaudR = 9600;

    //startTimer(10);
    
}

Bridge::~Bridge()
{
    if (connected == true)
        Disconnect();
        
    //stopTimer();
}

StringArray Bridge::GetPortInfo()
{
    
    port_number = comEnumerate();
    for(port_index=0; port_index < port_number; port_index++)
        portlist.set(comGetInternalName(port_index),comGetPortName(port_index));
    return portlist.getAllValues();
}

bool Bridge::Connect()
{
    port_state=comOpen(PortN,BaudR);
	if(port_state == 1)
		{
            memset(&read_buffer[0], 0, sizeof(read_buffer));
            connected = true;
         	return true;
	}
	else
    {
            return false;
    }		
}


void Bridge::Disconnect()
{
    comClose(PortN);
    connected = false;
}

void Bridge::Read()
{
	if (connected) {
		comRead(PortN, read_buffer, 1);
		buffer_string = String(read_buffer, 1);
		memset(&read_buffer[0], 0, sizeof(read_buffer));

		if (buffer_string == "l") Az_inc_state = true;
		if (buffer_string == "r") Az_dec_state = true;
		if (buffer_string == "d") El_dec_state = true;
		if (buffer_string == "u") El_inc_state = true;
		if (buffer_string == "1") Play_reference = true;
		if (buffer_string == "2") Play_test = true;
		if (buffer_string == "3") AzEl_reset = true;
		if (buffer_string == "4") Go_next = true;
		buffer_string = "0";
	}
}

//void Bridge::hiResTimerCallback()
//{
//	if(connected) {
//        
//        comRead(PortN, read_buffer, 1);
//		Az_dec_state = true;
//		if (read_buffer == "l") Az_dec_state = true;
//		//else Az_dec_state = false;
//		if (read_buffer == "r") Az_inc_state = true;
//		//else Az_inc_state = false;
//	}
//}


