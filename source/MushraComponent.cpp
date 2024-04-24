#include "MushraComponent.h"

MushraComponent::MushraComponent() {}

MushraComponent::~MushraComponent() {}

void MushraComponent::paint(juce::Graphics& g) {
  // BACKGROUND
  //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
  g.fillAll(juce::Colours::gainsboro);

  // RECTANGULAR OUTLINE
  g.setColour(juce::Colours::black);
  g.drawRect(getLocalBounds(), 1);

  g.setFont(juce::Font(16.0f));
  g.setColour(juce::Colours::black);
  g.drawText(testTypeText, 25, 10, getWidth() - 25, 30,
             juce::Justification::centred, true);
  g.setFont(juce::Font(14.0f));

  if (showInfo) {
    g.drawText("Trial: " + juce::String(currentTrialIndex + 1) + " of " +
                   juce::String(regionArray.size()),
               390, 680, 200, 20, juce::Justification::centredLeft, true);
    g.drawText("Region: " + juce::String(currentRegion + 1) + " of " +
                   juce::String(regionArray.size()),
               390, 700, 200, 20, juce::Justification::centredLeft, true);
    g.drawText("Sample: " + juce::String(currentTrack + 1) + " of " +
                   juce::String(numberOfSamplesPerRegion),
               390, 720, 200, 20, juce::Justification::centredLeft, true);
    g.drawText(
        "Start: " + juce::String(regionArray[currentRegion]->dawStartTime), 545,
        700, 200, 20, juce::Justification::centredLeft, true);
    g.drawText("Stop: " + juce::String(regionArray[currentRegion]->dawStopTime),
               545, 720, 200, 20, juce::Justification::centredLeft, true);
  }

  int linesStartX = 125;
  int linesEndX = 925;
  int linesStartY = 0.06f * getHeight();
  int linesEndY = 0.66f * getHeight();
  int linesYinterval = (linesEndY - linesStartY) / 5;

  //Line<float> line1(Point<float>(linesStartX, linesStartY), Point<float>(linesEndX, linesStartY));
  juce::Line<float> line2(
      juce::Point<float>(linesStartX, linesStartY + linesYinterval * 1),
      juce::Point<float>(linesEndX, linesStartY + linesYinterval * 1));
  juce::Line<float> line3(
      juce::Point<float>(linesStartX, linesStartY + linesYinterval * 2),
      juce::Point<float>(linesEndX, linesStartY + linesYinterval * 2));
  juce::Line<float> line4(
      juce::Point<float>(linesStartX, linesStartY + linesYinterval * 3),
      juce::Point<float>(linesEndX, linesStartY + linesYinterval * 3));
  juce::Line<float> line5(
      juce::Point<float>(linesStartX, linesStartY + linesYinterval * 4),
      juce::Point<float>(linesEndX, linesStartY + linesYinterval * 4));
  //Line<float> line6(Point<float>(linesStartX, linesEndY), Point<float>(linesEndX, linesEndY));

  float dashPattern[2];
  dashPattern[0] = 4.0;
  dashPattern[1] = 8.0;
  g.setColour(juce::Colours::dimgrey);

  //g.drawDashedLine(line1, dashPattern, 2, 1.0f);
  g.drawDashedLine(line2, dashPattern, 2, 1.0f);
  g.drawDashedLine(line3, dashPattern, 2, 1.0f);
  g.drawDashedLine(line4, dashPattern, 2, 1.0f);
  g.drawDashedLine(line5, dashPattern, 2, 1.0f);
  //g.drawDashedLine(line6, dashPattern, 2, 1.0f);

  int textStartX = linesStartX - 160;
  int textWidth = 200;

  for (auto label : ratingScaleLabels) {
    g.drawText(label, textStartX, linesStartY, textWidth, linesYinterval,
               juce::Justification::centredRight, true);
    linesStartY += linesYinterval;
  }
}

void MushraComponent::createGui() {
  // scores array
  scoresArray.resize(regionArray.size());
  for (int i = 0; i < regionArray.size(); ++i)
    scoresArray[i].resize(numberOfSamplesPerRegion);

  // Put some values in
  //scoresArray[1][2] = 6.0;
  //scoresArray[3][1] = 5.5;

  // trial randomization array
  trialRandomizationArray.resize(regionArray.size());
  for (int i = 0; i < regionArray.size(); ++i)
    trialRandomizationArray[i] = i;
  std::random_device seed;
  std::mt19937 rng(seed());
  std::shuffle(trialRandomizationArray.begin(), trialRandomizationArray.end(),
               rng);
  currentRegion = trialRandomizationArray
      [currentTrialIndex];  // set the first region according to the randomized order

  // sample randomization array
  sampleRandomizationArray.resize(regionArray.size());
  for (int i = 0; i < regionArray.size(); ++i) {
    sampleRandomizationArray[i].resize(numberOfSamplesPerRegion);
    for (int j = 0; j < numberOfSamplesPerRegion; ++j)
      sampleRandomizationArray[i][j] = j;

    std::random_device seed;
    std::mt19937 rng(seed());
    std::shuffle(sampleRandomizationArray[i].begin(),
                 sampleRandomizationArray[i].end(), rng);
  }

  for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
    rateSampleSliderArray.add(new juce::Slider());
    rateSampleSliderArray[i]->getProperties().set("rateSampleSlider", true);
    rateSampleSliderArray[i]->getProperties().set("sliderIndex", i);
    rateSampleSliderArray[i]->setSliderStyle(juce::Slider::LinearBarVertical);
    rateSampleSliderArray[i]->setTextBoxIsEditable(false);
    rateSampleSliderArray[i]->setTextBoxStyle(juce::Slider::NoTextBox, true, 0,
                                              0);
    rateSampleSliderArray[i]->setRange(0, 100, 1);
    rateSampleSliderArray[i]->setValue(0);
    rateSampleSliderArray[i]->addListener(this);
    addAndMakeVisible(rateSampleSliderArray[i]);
  }

  for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
    rateSampleSliderLabelArray.add(new juce::Label());
    rateSampleSliderLabelArray[i]->setText(
        juce::String(scoresArray[currentRegion][i], 0),
        juce::dontSendNotification);
    rateSampleSliderLabelArray[i]->setJustificationType(
        juce::Justification::centred);
    rateSampleSliderLabelArray[i]->setColour(juce::Label::textColourId,
                                             juce::Colours::black);
    addAndMakeVisible(rateSampleSliderLabelArray[i]);
  }

  juce::StringArray playButtonAlphabet = {
      "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
      "N", "O", "P", "Q", "R", "S", "T", "U", "V", "X", "Y", "Z"};
  for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
    playSampleButtonArray.add(new juce::TextButton());
    playSampleButtonArray[i]->getProperties().set("playSampleButton", true);
    playSampleButtonArray[i]->getProperties().set("buttonIndex", i);
    if (i < playButtonAlphabet.size())
      playSampleButtonArray[i]->setButtonText(playButtonAlphabet[i]);
    playSampleButtonArray[i]->addListener(this);
    addAndMakeVisible(playSampleButtonArray[i]);
  }

  playReference.setButtonText("Reference");
  playReference.addListener(this);
  if (isReferenceButtonVisible)
    addAndMakeVisible(playReference);

  loopSlider.setSliderStyle(juce::Slider::ThreeValueHorizontal);
  loopSlider.setRange(0, 1);
  loopSlider.setMinAndMaxValues(0, 1);
  loopSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  loopSlider.addListener(this);
  addAndMakeVisible(loopSlider);

  loopTB.setButtonText("Loop");
  loopTB.setToggleState(loop, juce::dontSendNotification);
  loopTB.addListener(this);
  loopTB.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
  loopTB.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
  loopTB.setColour(juce::ToggleButton::tickDisabledColourId,
                   juce::Colours::black);
  addAndMakeVisible(loopTB);

  showInfoTB.setButtonText("Info");
  showInfoTB.setToggleState(showInfo, juce::dontSendNotification);
  showInfoTB.addListener(this);
  showInfoTB.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
  showInfoTB.setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
  showInfoTB.setColour(juce::ToggleButton::tickDisabledColourId,
                       juce::Colours::black);
  addAndMakeVisible(showInfoTB);

  stopPlaybackB.setButtonText("Stop");
  stopPlaybackB.addListener(this);
  addAndMakeVisible(stopPlaybackB);

  goToPrevious.setButtonText("Previous Trial");
  goToPrevious.addListener(this);
  addAndMakeVisible(goToPrevious);

  goToNext.setButtonText("Next Trial");
  goToNext.addListener(this);
  addAndMakeVisible(goToNext);

  dawTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
  sampleTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
  addAndMakeVisible(dawTimeLabel);
  addAndMakeVisible(sampleTimeLabel);

  updateTransportSlider(true);
  updatePrevNextButtons();

  resized();
}

void MushraComponent::connectOsc(OscTransceiver* oscTxRx, juce::String dawIp, int dawTxPort,
                                 int dawRxPort) {
  dawTxRx = oscTxRx;

  // DAW
  dawTxRx->connectSender(dawIp, dawTxPort);
  dawTxRx->connectReceiver(dawRxPort);
  dawTxRx->addListener(this);

  // DAW state
  stopPlayback();
}

void MushraComponent::resized() {
  auto rateRectWidth = 0.8f * getWidth();
  auto rateRectHeight = 0.6f * getHeight();

  auto sliderSpacing = rateRectWidth / (numberOfSamplesPerRegion + 1);

  auto sliderWidth = 50;
  auto sliderHeight = rateRectHeight;

  auto buttonWidth = 65;
  auto buttonHeight = buttonWidth;
  auto labelHeight = 0.5 * buttonWidth;

  auto sliderPositionX = (getWidth() - rateRectWidth) / 2 + sliderWidth / 2;
  auto sliderPositionY = 0.06f * getHeight() + sliderHeight / 2;
  auto labelPositionY = sliderPositionY + sliderHeight / 2 + labelHeight * 0.5f;
  auto buttonPositionY = sliderPositionY + sliderHeight / 2 + buttonHeight;

  for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
    rateSampleSliderArray[i]->setSize(sliderWidth, sliderHeight);
    rateSampleSliderLabelArray[i]->setSize(buttonWidth, labelHeight);
    playSampleButtonArray[i]->setSize(buttonWidth, buttonHeight);

    rateSampleSliderArray[i]->setCentrePosition(
        sliderPositionX + (i + 1) * sliderSpacing, sliderPositionY);
    rateSampleSliderLabelArray[i]->setCentrePosition(
        sliderPositionX + (i + 1) * sliderSpacing, labelPositionY);
    playSampleButtonArray[i]->setCentrePosition(
        sliderPositionX + (i + 1) * sliderSpacing, buttonPositionY);
  }

  playReference.setBounds(25, buttonPositionY - buttonHeight / 2, 80,
                          buttonHeight);
  stopPlaybackB.setBounds(25, buttonPositionY + buttonHeight, 80, buttonHeight);
  loopSlider.setBounds(sliderPositionX, buttonPositionY + buttonHeight,
                       rateRectWidth, labelHeight);

  loopTB.setBounds(0.8 * getWidth(), 0.875f * getHeight(), 80, labelHeight);
  showInfoTB.setBounds(getWidth() - 80, getHeight() - labelHeight, 80,
                       labelHeight);
  goToPrevious.setBounds(sliderPositionX, buttonPositionY + 2 * buttonHeight,
                         100, labelHeight);
  goToNext.setBounds(sliderPositionX + 125, buttonPositionY + 2 * buttonHeight,
                     100, labelHeight);

  dawTimeLabel.setBounds(700, 700, 200, 20);
  sampleTimeLabel.setBounds(700, 720, 200, 20);
}

void MushraComponent::buttonClicked(juce::Button* buttonThatWasClicked) {
  bool playSampleButtonState =
      buttonThatWasClicked->getProperties()["playSampleButton"];
  if (playSampleButtonState) {
    int buttonIndex = buttonThatWasClicked->getProperties()["buttonIndex"];
    playSample(buttonIndex, randomizeSamples);
  } else if (buttonThatWasClicked == &playReference) {
    playSample(0, false);
  } else if (buttonThatWasClicked == &stopPlaybackB) {
    stopPlayback();
  } else if (buttonThatWasClicked == &loopTB) {
    loop = loopTB.getToggleState();
  } else if (buttonThatWasClicked == &showInfoTB) {
    showInfo = showInfoTB.getToggleState();
  } else if (buttonThatWasClicked == &goToNext) {
    if (currentTrialIndex < regionArray.size() - 1) {
      currentTrialIndex++;
      currentRegion = trialRandomizationArray[currentTrialIndex];
      stopPlayback();
      updateTransportSlider(true);
      updateRatingSliders();
      updatePrevNextButtons();
    }

  } else if (buttonThatWasClicked == &goToPrevious) {
    if (currentTrialIndex > 0) {
      currentTrialIndex--;
      currentRegion = trialRandomizationArray[currentTrialIndex];
      stopPlayback();
      updateTransportSlider(true);
      updateRatingSliders();
      updatePrevNextButtons();
    }
  }
  repaint();
}

void MushraComponent::sliderValueChanged(juce::Slider* sliderThatWasChanged) {
  bool rateSampleSliderChanged =
      sliderThatWasChanged->getProperties()["rateSampleSlider"];

  if (rateSampleSliderChanged) {
    int sliderIndex = sliderThatWasChanged->getProperties()["sliderIndex"];
    int trackNum = sampleRandomizationArray[currentRegion][sliderIndex];
    scoresArray[currentRegion][trackNum] = sliderThatWasChanged->getValue();

    for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
      int trackNum = sampleRandomizationArray[currentRegion][i];
      rateSampleSliderLabelArray[i]->setText(
          juce::String(scoresArray[currentRegion][trackNum], 0),
          juce::dontSendNotification);
    }
  } else if (sliderThatWasChanged == &loopSlider) {
    float regionLength = regionArray[currentRegion]->getRegionLength();
    regionArray[currentRegion]->loopStartOffset = loopSlider.getMinValue();
    regionArray[currentRegion]->loopEndOffset =
        regionLength - loopSlider.getMaxValue();
    regionArray[currentRegion]->calculateStartEndTimes();
  }
}

void MushraComponent::playSample(int track, bool randomize) {
  if (randomize)
    track = sampleRandomizationArray[currentRegion][track];

  if (playback == true)
    stopPlayback();  //dawTx.sendOscMessage("/stop");
  dawTxRx->sendOscMessage("/time", regionArray[currentRegion]->startTime);
  if (track != currentTrack) {
    dawTxRx->sendOscMessage("/track/" + juce::String(currentTrack + 1) + "/solo",
                         0);
  }
  dawTxRx->sendOscMessage("/track/" + juce::String(track + 1) + "/solo", 1);
  dawTxRx->sendOscMessage("/play");
  playback = true;
  currentTrack = track;
}

void MushraComponent::playSampleLoop() {
  dawTxRx->sendOscMessage("/track/" + juce::String(0) + "/mute", 1);
  dawTxRx->sendOscMessage("/time", regionArray[currentRegion]->startTime);
  //Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100 );
  dawTxRx->sendOscMessage("/track/" + juce::String(0) + "/mute", 0);
}

void MushraComponent::stopPlayback() {
  dawTxRx->sendOscMessage("/track/" + juce::String(0) + "/mute", 1);
  dawTxRx->sendOscMessage("/stop");
  currentPosition = regionArray[currentRegion]->startTime;
  dawTxRx->sendOscMessage("/time", currentPosition);
  dawTxRx->sendOscMessage("/soloreset");
  //Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100);
  dawTxRx->sendOscMessage("/track/" + juce::String(0) + "/mute", 0);
}

void MushraComponent::oscBundleReceived(const juce::OSCBundle& bundle) {
  for (int i=0; i < bundle.size(); ++i) { oscMessageReceived(bundle[i].getMessage()); }
}

void MushraComponent::oscMessageReceived(const juce::OSCMessage& message) {
  // DAW
  if (message.size() == 1 && message.getAddressPattern() == "/time" &&
      message[0].isFloat32()) {
    currentPosition = message[0].getFloat32();
    updateTransportSlider(false);

    dawTimeLabel.setText("DAW Time: " + juce::String(currentPosition, 3),
                         juce::NotificationType::dontSendNotification);
    sampleTimeLabel.setText(
        "Sample Time: " +
            juce::String(
                currentPosition - regionArray[currentRegion]->startTime, 3),
        juce::NotificationType::dontSendNotification);

    if (currentPosition >= regionArray[currentRegion]->stopTime) {
      if (loop)
        playSampleLoop();
      else
        stopPlayback();
    }
  }

  // CLIENT
  if (message.size() == 1 && message.getAddressPattern() == "/host/play" &&
      message[0].isInt32()) {
    playSampleButtonArray[message[0].getInt32()]->triggerClick();
  } else if (message.size() == 1 && message.getAddressPattern() == "/host" &&
             message[0].isString()) {
    if (message[0].getString() == "playref")
      playReference.triggerClick();
    if (message[0].getString() == "stopplayback")
      stopPlaybackB.triggerClick();
    if (message[0].getString() == "loopon")
      loopTB.setToggleState(true, juce::sendNotification);
    if (message[0].getString() == "loopoff")
      loopTB.setToggleState(false, juce::sendNotification);
    if (message[0].getString() == "gonext")
      goToNext.triggerClick();
    if (message[0].getString() == "goprev")
      goToPrevious.triggerClick();
  } else if (message.size() == 2 &&
             message.getAddressPattern() == "/host/slider" &&
             message[0].isInt32() && message[1].isFloat32()) {
    rateSampleSliderArray[message[0].getInt32()]->setValue(
        message[1].getFloat32());
  } else if (message.size() == 4 &&
             message.getAddressPattern() == "/host/loopslider" &&
             message[0].isFloat32() && message[1].isFloat32() &&
             message[2].isFloat32() && message[3].isFloat32()) {
    regionArray[currentRegion]->loopStartOffset = message[2].getFloat32();
    regionArray[currentRegion]->loopEndOffset =
        message[0].getFloat32() - message[3].getFloat32();
    regionArray[currentRegion]->calculateStartEndTimes();
    updateTransportSlider(true);
  }
}

void MushraComponent::updateTransportSlider(bool updateLoop) {
  float regionLength = regionArray[currentRegion]->getRegionLength();
  if (updateLoop) {
    loopSlider.setRange(0, regionLength, juce::dontSendNotification);
    loopSlider.setValue(regionArray[currentRegion]->loopStartOffset,
                        juce::dontSendNotification);
    loopSlider.setMinValue(regionArray[currentRegion]->loopStartOffset,
                           juce::dontSendNotification);
    loopSlider.setMaxValue(
        regionLength - regionArray[currentRegion]->loopEndOffset,
        juce::dontSendNotification);
  } else {
    loopSlider.setValue(
        currentPosition - regionArray[currentRegion]->dawStartTime,
        juce::dontSendNotification);
  }
}

void MushraComponent::updateRatingSliders() {
  for (int i = 0; i < numberOfSamplesPerRegion; ++i) {
    int trackNum = sampleRandomizationArray[currentRegion][i];
    rateSampleSliderArray[i]->setValue(scoresArray[currentRegion][trackNum]);
  }
}

void MushraComponent::updatePrevNextButtons() {
  if (currentTrialIndex == regionArray.size() - 1)
    goToNext.setVisible(false);
  else
    goToNext.setVisible(true);
  if (currentTrialIndex == 0)
    goToPrevious.setVisible(false);
  else
    goToPrevious.setVisible(true);
}

void MushraComponent::saveResults() {
  juce::File chosenFile;
  juce::String dataToSave;

  juce::FileChooser fc("Choose a file to save...",
                       juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory), "*.txt", true);

#if JUCE_MODAL_LOOPS_PERMITTED
  if (fc.browseForFileToSave(true)) {
    chosenFile = fc.getResult().withFileExtension(".txt");

    dataToSave = juce::String("Test Type: ") + testTypeText + juce::String("\n\n");

    dataToSave += juce::String("##### RAW RESULTS #####\n\n");
    for (int i = 0; i < scoresArray.size(); i++) {
      for (int j = 0; j < scoresArray[0].size(); j++) {
        dataToSave += juce::String(scoresArray[i][j]);
        dataToSave += "\t";
      }
      dataToSave += "\n";
    }

    dataToSave += juce::String("\n");
    dataToSave += juce::String("##### TRIAL RANDOMIZATION ARRAY #####\n\n");
    for (int i = 0; i < scoresArray.size(); i++) {
      dataToSave += juce::String(trialRandomizationArray[i]);
      dataToSave += "\t";
    }
    dataToSave += juce::String("\n\n");
    dataToSave += juce::String("##### SAMPLE RANDOMIZATION ARRAY #####\n\n");
    for (int i = 0; i < scoresArray.size(); i++) {
      for (int j = 0; j < scoresArray[0].size(); j++) {
        dataToSave += juce::String(sampleRandomizationArray[i][j]);
        dataToSave += ",";
      }
      dataToSave += "\n";
    }

    dataToSave += "\n" + juce::String("##### TRACK NAMES #####\n\n");
    for (int i = 0; i < trackNameArray.size(); i++) {
      dataToSave += trackNameArray[i] + "\n";
    }
    dataToSave += "\n" + juce::String("##### DATE AND TIME #####\n\n");
    dataToSave += juce::Time::getCurrentTime().formatted("%Y-%m-%d %H:%M:%S") + "\n";

    dataToSave += "\n" + juce::String("##### REGIONS #####\n\n");
    for (int i = 0; i < regionArray.size(); i++) {
      dataToSave += "Region: " + juce::String(i + 1) + "\n";
      dataToSave += "Beginning: " + juce::String(regionArray[i]->dawStartTime) + "\n";
      dataToSave += "End: " + juce::String(regionArray[i]->dawStopTime) + "\n\n";
    }
    chosenFile.replaceWithText(dataToSave);
  }
#endif
}
