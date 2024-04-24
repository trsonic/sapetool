#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

#include <random>
#include <vector>
#include "OscTransceiver.h"
#include "SampleRegion.h"

using std::vector;

class MushraComponent : public juce::Component,
                        public juce::Button::Listener,
                        public juce::Slider::Listener,
                        public juce::OSCReceiver,
                        public juce::OSCReceiver::Listener<
                            juce::OSCReceiver::MessageLoopCallback> {
 public:
  //==============================================================================
  MushraComponent();
  ~MushraComponent();

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  enum TestType {
    standardMushra,
    externalisation,
    degreeOfLiking,
    naturalness
  };

  void setupRatingLabels(TestType type) {
    testType = type;
    switch (type) {
      case MushraComponent::standardMushra:
        testTypeText = "Standard MUSHRA Test";
        ratingScaleLabels = {"Excellent", "Good", "Fair", "Poor", "Bad"};
        break;
      case MushraComponent::externalisation:
        testTypeText =
            "Externalisation: Rate how you perceive the sound sources, whether "
            "they seem to be coming from within your head or outside of it.";
        ratingScaleLabels = {"More Externalized", "Externalized", "Indistinct",
                             "Internalized", "More Internalized"};
        break;
      case MushraComponent::degreeOfLiking:
        testTypeText =
            "Degree of Liking: Rate how much you like or dislike the scene "
            "based on its overall pleasantness or unpleasantness.";
        ratingScaleLabels = {"Strongly Like", "Like", "Neutral", "Dislike",
                             "Strongly Dislike"};
        break;
      case MushraComponent::naturalness:
        testTypeText = "naturalness";
        ratingScaleLabels = {"Strongly Natural", "Natural", "Neutral",
                             "Dislike", "Strongly Dislike"};
        break;
      default:
        break;
    }
    repaint();
  }

  juce::OwnedArray<SampleRegion> regionArray;
  vector<vector<float>> scoresArray;
  vector<int> trialRandomizationArray;
  vector<vector<int>> sampleRandomizationArray;
  juce::Array<juce::String> trackNameArray;
  bool randomizeSamples = true;
  int numberOfSamplesPerRegion = 0;

  juce::String hostIp;

  void createGui();
  void connectOsc(OscTransceiver* oscTxRx, juce::String dawIp, int dawTxPort, int dawRxPort);
  void saveResults();

 private:
  OscTransceiver* dawTxRx;

  // APP CONFIG
  TestType testType = standardMushra;
  bool isReferenceButtonVisible = true;

  // APP STATE
  int currentRegion = 0, currentTrack = 0;
  int currentTrialIndex = 0;
  float currentPosition = 0.0f;
  bool playback = false;
  bool loop = true;
  juce::Label dawTimeLabel, sampleTimeLabel;
  bool showInfo = false;

  // test type specific text
  juce::String testTypeText;
  juce::StringArray ratingScaleLabels;

  // MUSHRA CONTROLS
  juce::TextButton playReference;
  juce::TextButton stopPlaybackB;
  juce::TextButton goToPrevious, goToNext;
  juce::OwnedArray<juce::Slider> rateSampleSliderArray;
  juce::OwnedArray<juce::Label> rateSampleSliderLabelArray;
  juce::OwnedArray<juce::Button> playSampleButtonArray;
  juce::Slider loopSlider;
  juce::ToggleButton loopTB;
  juce::ToggleButton showInfoTB;

  // METHODS
  void playSample(int track, bool randomize);
  void playSampleLoop();
  void stopPlayback();
  void buttonClicked(juce::Button* buttonThatWasClicked) override;
  void sliderValueChanged(juce::Slider* sliderThatWasChanged) override;
  void oscBundleReceived(const juce::OSCBundle& bundle) override;
  void oscMessageReceived(const juce::OSCMessage& message) override;
  void updateTransportSlider(bool updateLoop);
  void updateRatingSliders();
  void updatePrevNextButtons();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MushraComponent)
};
