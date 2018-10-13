/*
  ==============================================================================

    MushraClientComponent.h
    Created: 16 Aug 2018 4:43:39pm
    Author:  tr837

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscTransceiver.h"

#pragma once

class MushraClientComponent :	public Component,
								public Button::Listener,
								public Slider::Listener,
								public OSCReceiver,
								public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	//==============================================================================
	MushraClientComponent();
	~MushraClientComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

	// OSC
	String ipToSendTo = "127.0.0.1";
	int portToSendTo = 8181;
	int portToReceiveAt = 8282;

	int numberOfSamplesPerRegion;
	int currentTrialIndex, numberOfTrials;
	bool isReferenceButtonVisible;
	void createGui();
	void connectOsc();

private:
	oscTransceiver hostTx, hostRx;

	// MUSHRA CONTROLS
	TextButton playReference;
	TextButton stopPlaybackB;
	TextButton goToPrevious, goToNext;
	OwnedArray<Slider> rateSampleSliderArray;
	OwnedArray<Label> rateSampleSliderLabelArray;
	OwnedArray<Button> playSampleButtonArray;
	Slider loopSlider;
    float lastLoopSliderMin, lastLoopSliderMax;
	ToggleButton loopTB;
	bool loop = true;



	void buttonClicked(Button* buttonThatWasClicked) override;
	void sliderValueChanged(Slider *sliderThatWasChanged) override;
	void oscMessageReceived(const OSCMessage& message) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MushraClientComponent)
};
