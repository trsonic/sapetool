#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscTransceiver.h"
#include "MushraComponent.h"
#include "SampleRegion.h"


class MainComponent   : public Component,
						public OSCReceiver,
						public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>,
                        public Timer
{
public:
    MainComponent();
    ~MainComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
	// GUI
	Label dawTxIpLabel, dawTxPortLabel, dawRxPortLabel;
	TextButton getTracksAndMarkersFromDawButton, initGuiButton;
	OwnedArray<ToggleButton> testTypeSelectionButtons;
	TextButton saveResultsButton;
	String dawIp;
	int dawTxPort, dawRxPort;

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

	bool dawReady = false;

	// METHODS
	void oscMessageReceived(const OSCMessage& message) override;
    virtual void timerCallback() override;
	void getTracksAndMarkersFromDaw();
	void launchTest();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
