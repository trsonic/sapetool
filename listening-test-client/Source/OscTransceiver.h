#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class oscTransceiver : public OSCReceiver
{
public:
	oscTransceiver();
	~oscTransceiver();

	void connectSender(String ipToSendTo, int portToSendTo);
	void disconnectSender();
	void connectReceiver(int portToReceiveAt);
	void disconnectReceiver();
	void sendOscMessage(const String& message);
	void sendOscMessage(const String& message, int value);
	void sendOscMessage(const String& message, float value);
	void sendOscMessage(const String& message, String value);
	void sendOscMessage(const String& message, int value, float value2);
    void sendOscMessage(const String& message, float value, float value2, float value3, float value4);
private:
	OSCSender sender;

	void showConnectionErrorMessage(const String& messageText);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (oscTransceiver)
};
