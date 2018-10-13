/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
#if JUCE_ANDROID || JUCE_IOS
    auto screenBounds = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    setSize (screenBounds.getWidth(), screenBounds.getHeight());
#else
    setSize (1024, 768);
#endif

	hostRx.connectReceiver(8282);
	hostRx.addListener(this);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colours::gainsboro);

	g.setFont(Font(16.0f));
	g.setColour(Colours::black);
	g.drawText("Press Start Mobile GUI in the main app window.", getLocalBounds(), Justification::centred, true);
    g.drawText(String(getWidth()) + " x " + String(getHeight()), getLocalBounds(), Justification::bottomRight, true);
}

void MainComponent::resized()
{
	mcc.setBounds(getLocalBounds());
}

void MainComponent::oscMessageReceived(const OSCMessage& message)
{
	if (message.size() == 1 && message.getAddressPattern() == "/client/hostip" && message[0].isString())
	{
		mcc.ipToSendTo = message[0].getString();
	}
	else if (message.size() == 1 && message.getAddressPattern() == "/client/hostport" && message[0].isInt32())
	{
		mcc.portToSendTo = message[0].getInt32();
	}
	else if (message.size() == 1 && message.getAddressPattern() == "/client/nofsamples" && message[0].isInt32())
	{
		mcc.numberOfSamplesPerRegion = message[0].getInt32();
	}
	else if (message.size() == 2 && message.getAddressPattern() == "/client/currenttrial" && message[0].isInt32() && message[1].isInt32())
	{
		mcc.currentTrialIndex = message[0].getInt32();
		mcc.numberOfTrials = message[1].getInt32();
	}
	else if (message.size() == 1 && message.getAddressPattern() == "/client/isrefbutton" && message[0].isInt32())
	{
		if(message[0].getInt32() == 1) mcc.isReferenceButtonVisible = true;
		else mcc.isReferenceButtonVisible = false;
	}
	else if (message.size() == 1 && message.getAddressPattern() == "/client/gui" && message[0].isString())
	{
		if (message[0].getString() == "create")
		{
			hostRx.disconnectReceiver();
			
			mcc.connectOsc();
			mcc.createGui();
			addAndMakeVisible(mcc);
			resized();
		}
	}

}
