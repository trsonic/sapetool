#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	getTracksAndMarkersFromDawButton.setButtonText("Get DAW Configuration");
	getTracksAndMarkersFromDawButton.onClick = [this] { getTracksAndMarkersFromDaw(); };
	addAndMakeVisible(getTracksAndMarkersFromDawButton);

	StringArray testTypes= { "Mushra", "Externalisation", "Degree of Liking" };

	for ( auto test : testTypes)
{
		testTypeSelectionButtons.add(new ToggleButton());
		testTypeSelectionButtons.getLast()->setButtonText(test);
		testTypeSelectionButtons.getLast()->setRadioGroupId(1);
		testTypeSelectionButtons.getLast()->setColour(ToggleButton::textColourId, Colours::black);
		testTypeSelectionButtons.getLast()->setColour(ToggleButton::tickColourId, Colours::black);
		addAndMakeVisible(testTypeSelectionButtons.getLast());
	}

	initGuiButton.setButtonText("Launch Test");
	initGuiButton.onClick = [this] { launchTest(); };

	saveResultsButton.setButtonText("Save Results");
	saveResultsButton.onClick = [this] { mc.saveResults();  };


	dawTxIpLabel.setEditable(false, true, false);
	dawTxPortLabel.setEditable(false, true, false);
	dawRxPortLabel.setEditable(false, true, false);

	dawTxIpLabel.setText("127.0.0.1", dontSendNotification);
	dawTxPortLabel.setText("9696", dontSendNotification);
	dawRxPortLabel.setText("9797", dontSendNotification);

	addAndMakeVisible(dawTxIpLabel);
	addAndMakeVisible(dawTxPortLabel);
	addAndMakeVisible(dawRxPortLabel);

	setSize(1280, 768);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
	// BACKGROUND
	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillAll(Colours::gainsboro);


	// RECTANGULAR OUTLINE
	g.setColour(Colours::black);
	g.drawRect(getLocalBounds(), 1);
    g.drawRect(Rectangle<int>(0, 0, 256, getHeight()), 1);
	g.drawRect(Rectangle<int>(0, getHeight() - 110, 256, 110), 1);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
	g.drawText("Listening Test App", 25, 25, 200, 25, Justification::centredLeft, true);

	g.setFont(Font(14.0f));

	if (dawReady)
	{
		g.drawText("Number of Regions: " + String(numberOfRegions), 25, 125, 200, 25, Justification::centredLeft, true);
		g.drawText("Number of Samples: " + String(numberOfSamplesPerRegion), 25, 150, 200, 25, Justification::centredLeft, true);
	}

    int labelXPos = 25;
	int labelYPos = getHeight() - 85 - 25;

    g.drawText("IP", labelXPos + 42, labelYPos, 50, 25, Justification::centredLeft, true);
    g.drawText("Tx Port", labelXPos + 115, labelYPos, 50, 25, Justification::centredLeft, true);
    g.drawText("Rx Port", labelXPos + 165, labelYPos, 50, 25, Justification::centredLeft, true);
    g.drawText("DAW", labelXPos, labelYPos + 25, 50, 25, Justification::centredLeft, true);
}

void MainComponent::resized()
{
	getTracksAndMarkersFromDawButton.setBounds(28, 75, 200, 25);

	for (int i = 0; i < testTypeSelectionButtons.size(); i++)
	{
		testTypeSelectionButtons[i]->setBounds(25, 200 + i * 30, 200, 25);
	}

	initGuiButton.setBounds(25, 360, 100, 25);
	saveResultsButton.setBounds(25, 360, 100, 25);

	// OSC LABELS
	int labelXPos = 60;
	int labelYPos = getHeight() - 60 - 25;
	dawTxIpLabel.setBounds(labelXPos, labelYPos, 75, 25);
	dawTxPortLabel.setBounds(labelXPos + 75, labelYPos, 50, 25);
	dawRxPortLabel.setBounds(labelXPos + 125, labelYPos, 50, 25);	


	dawTxIpLabel.setColour(Label::textColourId, Colours::black);
	dawTxPortLabel.setColour(Label::textColourId, Colours::black);
	dawRxPortLabel.setColour(Label::textColourId, Colours::black);


	dawTxIpLabel.setJustificationType(Justification::centredLeft);	
	dawTxPortLabel.setJustificationType(Justification::centredLeft);	
	dawRxPortLabel.setJustificationType(Justification::centredLeft);

	// MC SIZE CONFIG
	mc.setSize(1024, 768);
	mc.setTopLeftPosition(getWidth() - 1024, 0);

}

void MainComponent::getTracksAndMarkersFromDaw()
{
	//getTracksAndMarkersFromDawButton.setVisible(false);

	dawIp = dawTxIpLabel.getText();
	dawTxPort = dawTxPortLabel.getText().getIntValue();
	dawRxPort = dawRxPortLabel.getText().getIntValue();


	// OSC sender and receiver connect
	DBG("Connecting OSC ...");
	dawTx.connectSender(dawIp, dawTxPort);
	dawRx.connectReceiver(dawRxPort);
	dawRx.addListener(this);

	DBG("Getting tracks / markers ...");
    countMarkers = true;
	countTracks = true;

	dawTx.sendOscMessage("/time", 0);

    markerIndex = 1;
	trackIndex = 1;

    lastMarkerIndex = 0;
	lastTrackIndex = 0;
	trackArrayIndex = 0;
    startTimer(250);
}

void MainComponent::timerCallback()
{
    if (passiveLoopCounter == 0)
    {
		if (markerIndex == lastMarkerIndex + 2) countMarkers = false;
		if (trackIndex == lastTrackIndex + 2) countTracks = false;
		
		if (countMarkers)
		{
			DBG("Jump to marker " + String(markerIndex));
			dawTx.sendOscMessage("/marker/" + String(markerIndex));
			markerIndex++;
		}
		if (countTracks)
		{
			DBG("Jump to track " + String(trackIndex));
			dawTx.sendOscMessage("/device/track/select/" + String(trackIndex));
			trackIndex++;
		}
		if (countMarkers == false && countTracks == false)
		{
			stopTimer();
			dawTx.sendOscMessage("/time", 0);
			dawTx.disconnectSender();
			dawRx.disconnectReceiver();

			for (int i = 0; i < markerTimeArray.size(); i++)
			{
				DBG("Marker(-1): " + String(i) + " Pos: " + String(markerTimeArray[i]));
			}
			
			for (int i = 0; i < trackNameArray.size(); i++)
			{
				DBG("Track(-1): " + String(i) + " Name: " + trackNameArray[i]);
			}

			if (markerTimeArray.size() >= 2 && trackNameArray.size() >= 2)
			{
				numberOfRegions = floor(markerTimeArray.size() / 2);
				numberOfSamplesPerRegion = trackNameArray.size();
				dawReady = true;

				addAndMakeVisible(initGuiButton);
			}
			else 	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
				"DAW error",
				"Not enough markers or tracks",
				"OK");
		}
    }
    else passiveLoopCounter--;

	repaint();
}

void MainComponent::oscMessageReceived(const OSCMessage& message)
{
    if (message.size() == 1 && message.getAddressPattern() == "/lastmarker/number/str" && message[0].isString())
    {
        lastMarkerIndex = message[0].getString().getIntValue();
        DBG("Last marker index: " + String(lastMarkerIndex));
    }
    if (message.size() == 1 && message.getAddressPattern() == "/time" && message[0].isFloat32() && message[0].getFloat32() != 0)
    {
        currentPosition = message[0].getFloat32();
        markerTimeArray.set(markerIndex, currentPosition);
		DBG("Last marker position: " + String(currentPosition));
    }

	if (message.size() == 1 && message.getAddressPattern() == "/track/number/str" && message[0].isString())
	{
		lastTrackIndex = message[0].getString().getIntValue();
		DBG("Last track index: " + String(lastTrackIndex));
	}
	if (message.size() == 1 && message.getAddressPattern() == "/track/name" && message[0].isString())
	{
		String trackName = message[0].getString();
		DBG("Last track name: " + trackName);
		if (trackName.startsWith("##"))
		{
			trackNameArray.set(trackArrayIndex, trackName);
			trackArrayIndex++;
		}
	}
}

void MainComponent::launchTest()
{
	MushraComponent::TestType typeToLaunch;

	if (testTypeSelectionButtons[0]->getToggleState()) typeToLaunch = MushraComponent::TestType::standardMushra;
	else if (testTypeSelectionButtons[1]->getToggleState()) typeToLaunch = MushraComponent::TestType::externalisation;
	else if (testTypeSelectionButtons[2]->getToggleState()) typeToLaunch = MushraComponent::TestType::degreeOfLiking;
	else return; // no test type selected

	mc.setupRatingLabels(typeToLaunch);

	for (auto testTypeSelectionButton : testTypeSelectionButtons)
	{
		testTypeSelectionButton->setVisible(false);
		testTypeSelectionButton->setToggleState(false, dontSendNotification);
	}

    for(int i = 0; i < numberOfRegions; ++i)
    {
		mc.regionArray.add(new SampleRegion());
		mc.regionArray[i]->dawStartTime = markerTimeArray[i * 2];         // 0 2 4 6
		mc.regionArray[i]->dawStopTime = markerTimeArray[(i * 2) + 1];    // 1 3 5 7
		mc.regionArray[i]->calculateStartEndTimes();
    }
	mc.numberOfSamplesPerRegion = numberOfSamplesPerRegion;
	mc.trackNameArray.addArray(trackNameArray);
	mc.connectOsc(dawIp, dawTxPort, dawRxPort);
	mc.createGui();
	addAndMakeVisible(mc);
	mushraComponentConnected = true;
	initGuiButton.setVisible(false);
	addAndMakeVisible(saveResultsButton);
    repaint();
}