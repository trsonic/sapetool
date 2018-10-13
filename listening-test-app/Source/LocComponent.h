#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Bridge.h"
#include "OscTransceiver.h"
#include "SampleRegion.h"
#include <vector>
#include <random>

using std::vector;

struct LocTrial
{
	int sampleIndex, regionIndex;
	float azimuthResult = 0, elevationResult = 0;
	int timeTaken = 0;
	String timeStamp = "not set";
};


class LocComponent :	public Component,
						public Button::Listener,
						public ComboBox::Listener,
						public KeyListener,
						public Timer,
						public OSCReceiver,
						public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	//==============================================================================
	LocComponent();
	~LocComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void buttonClicked(Button* buttonThatWasClicked) override;
	bool keyPressed(const KeyPress &k, Component *c) override;
	void timerCallback() override;
	void oscMessageReceived(const OSCMessage& message) override;

	void mouseDrag(const MouseEvent& e) override;
	void mouseDown(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;

	int mouseX, mouseY;

	OwnedArray<SampleRegion> regionArray;
	OwnedArray<LocTrial> trialArray;
	OwnedArray<LocTrial> trainingTrialArray;
	vector<int> trialRandomizationArray;
	Array<String> trackNameArray;
	String hostIp;
	int clientRxPortAtHost;

	void createTrialArray(int numberOfSamplesPerRegion, int numberOfRegions);
	void connectOsc(String dawIp, String clientIp, int dawTxPort, int dawRxPort, int clientTxPort, int clientRxPort);
	void sendAzElOSC();
	void saveResults();



private:
	StringArray CBox_portlist;
	ComboBox cb_portlist;
	TextButton AzEl_reset, Az_inc, Az_dec, El_inc, El_dec;
	TextButton Play_reference, Play_test, Go_next, Go_previous, Resume;
	TextButton Sp_refresh, Sp_connect;

	oscTransceiver dawTx, dawRx;
	oscTransceiver clientTx, clientRx;

	// APP STATE
	int currentRegion = 0, currentTrack = 0;
	int currentTrainingTrialIndex = 0;
	int currentTrialIndex = 0;

	float currentPosition;
	bool trainingMode = true;
	bool timeBreak = false;
	bool testFinished = false;
	bool playback = false;
	bool loop = false;

	int startTime;
	bool timerOn = false;

	bool adjRectClicked = false;
	float mouseDownAz, mouseDownEl;
	float Azimuth = 0, Elevation = 0;
	float AzimuthOSC = 0, ElevationOSC = 0;
	float AzimuthOSCsent = 0, ElevationOSCsent = 0;

	Bridge bridge;

	void playSample(int track, int region);
	void playSampleLoop();
	void stopPlayback();

	//OSCSender sender;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LocComponent)
};
