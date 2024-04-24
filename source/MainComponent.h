#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

#include "MushraComponent.h"
#include "OscTransceiver.h"

class MainComponent : public juce::Component,
                      public juce::OSCReceiver,
                      public juce::OSCReceiver::Listener<
                          juce::OSCReceiver::MessageLoopCallback>,
                      public juce::Timer {
 public:
  MainComponent();
  ~MainComponent();

  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  // GUI
  juce::Label dawTxIpLabel, dawTxPortLabel, dawRxPortLabel;
  juce::TextButton getTracksAndMarkersFromDawButton, initGuiButton;
  juce::OwnedArray<juce::ToggleButton> testTypeSelectionButtons;
  juce::TextButton saveResultsButton;

  OscTransceiver dawTxRx;

  // GET MARKER POSITIONS
  bool countMarkers = false;
  int markerIndex;
  int lastMarkerIndex;
  float currentPosition;
  juce::Array<float> markerTimeArray;
  int numberOfTrials = 0;

  // GET TRACK NAMES
  bool countTracks = false;
  int trackIndex;
  int lastTrackIndex;
  int trackArrayIndex;
  juce::Array<juce::String> trackNameArray;
  int numberOfConditions = 0;
  bool dawReady = false;

  // MUSHRA COMPONENT
  MushraComponent mc;
  bool mushraComponentConnected = false;

  // METHODS
  void oscBundleReceived(const juce::OSCBundle& bundle) override;
  void oscMessageReceived(const juce::OSCMessage& message) override;
  void timerCallback() override;
  void getTracksAndMarkersFromDaw();
  void launchTest();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
