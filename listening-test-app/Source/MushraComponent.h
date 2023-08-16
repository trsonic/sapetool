#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscTransceiver.h"
#include "SampleRegion.h"
#include <vector>
#include <random>

using std::vector;

class MushraComponent : public Component,
						public Button::Listener,
						public Slider::Listener,
						public OSCReceiver,
						public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
	//==============================================================================
	MushraComponent();
	~MushraComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

	enum TestType {
		standardMushra, externalisation, degreeOfLiking
	};

	void setupRatingLabels(TestType type)
	{
		testType = type;
		switch (type)
		{
		case MushraComponent::standardMushra:
			testTypeText = "Standard MUSHRA Test";
			ratingScaleLabels = { "Excellent", "Good", "Fair", "Poor", "Bad" };
			break;
		case MushraComponent::externalisation:
			testTypeText = "Externalisation: Rate how you perceive the sound sources, whether they seem to be coming from within your head or outside of it.";
			ratingScaleLabels = { "More Externalized", "Externalized", "Indistinct", "Internalized", "More Internalized" };
			break;
		case MushraComponent::degreeOfLiking:
			testTypeText = "Degree of Liking: Rate how much you like or dislike the scene based on its overall pleasantness or unpleasantness.";
			ratingScaleLabels = { "Strongly Like", "Like", "Neutral", "Dislike", "Strongly Dislike" };
			break;
		default:
			break;
		}
		repaint();
	};

	OwnedArray<SampleRegion> regionArray;
	vector<vector<float>> scoresArray;
	vector<int> trialRandomizationArray;
	vector<vector<int>> sampleRandomizationArray;
	Array<String> trackNameArray;
	bool randomizeSamples = true;
	int numberOfSamplesPerRegion;

	String hostIp;


	void createGui();
	void connectOsc(String dawIp, int dawTxPort, int dawRxPort);
	void saveResults();

private:
	oscTransceiver dawTx, dawRx;

	// APP CONFIG
	TestType testType = standardMushra;
	bool isReferenceButtonVisible = false;

	// APP STATE
	int currentRegion = 0, currentTrack = 0;
	int currentTrialIndex = 0;
	float currentPosition;
	bool playback = false;
	bool loop = true;
	Label dawTimeLabel, sampleTimeLabel;

	// test type specific text
	String testTypeText;
	StringArray ratingScaleLabels;

	// MUSHRA CONTROLS
	TextButton playReference;
	TextButton stopPlaybackB;
	TextButton goToPrevious, goToNext;
	OwnedArray<Slider> rateSampleSliderArray;
    OwnedArray<Label> rateSampleSliderLabelArray;
	OwnedArray<Button> playSampleButtonArray;
	Slider loopSlider;
	ToggleButton loopTB;
	
	// METHODS


	void playSample(int track, bool randomize);
	void playSampleLoop();
	void stopPlayback();
	void buttonClicked(Button* buttonThatWasClicked) override;
	void sliderValueChanged(Slider *sliderThatWasChanged) override;
	void oscMessageReceived(const OSCMessage& message) override;
	void updateTransportSlider(bool updateLoop);
	void updateRatingSliders();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MushraComponent)
};
