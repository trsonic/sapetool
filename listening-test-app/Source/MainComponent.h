#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscTransceiver.h"
#include "MushraComponent.h"
#include "SampleRegion.h"
#include "LocComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
						public Button::Listener,
						public OSCReceiver,
						public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>,
                        public Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


private:
	// GUI
	Label hostIpLabel;
	Label dawTxIpLabel, dawTxPortLabel, dawRxPortLabel;
	Label clientTxIpLabel, clientTxPortLabel, clientRxPortLabel;
	ToggleButton mushraToggleButton, localizationToggleButton;
	TextButton talkToDawButton, initGuiButton;
	TextButton saveResultsButton;
	TextButton initClientGuiButton;
	String dawIp, clientIp, hostIp;
	int dawTxPort, dawRxPort, clientTxPort, clientRxPort;

    oscTransceiver dawTx, dawRx;
    
    // GET MARKER POSITIONS
    bool countMarkers = false;
    int passiveLoopCounter = 3;
    int markerIndex;
    int lastMarkerIndex;
    float currentPosition;
    Array<float> markerTimeArray;
    int numberOfRegions = 0;

	// GET TRACK NAMES
	bool countTracks = false;
	int trackIndex;
	int lastTrackIndex;
	int trackArrayIndex;
	Array<String> trackNameArray;
    int numberOfSamplesPerRegion = 0;
	
    // MUSHRA COMPONENT
    MushraComponent mc;
    bool mushraComponentConnected = false;

	// Method of Adjustment COMPONENT
	LocComponent lc;
	bool locComponentConnected = false;

	bool dawReady = false;

	// METHODS
	void buttonClicked(Button* buttonThatWasClicked) override;
	void oscMessageReceived(const OSCMessage& message) override;
    virtual void timerCallback() override;
	void getTracksAndMarkersFromDaw();
	void configureMushra(), configureLoc();
	void removeGuiInitButton();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
