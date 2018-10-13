#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	talkToDawButton.setButtonText("Get DAW Configuration");
	talkToDawButton.addListener(this);
	addAndMakeVisible(talkToDawButton);

	mushraToggleButton.setButtonText("MUSHRA");
	mushraToggleButton.setClickingTogglesState(true);
	mushraToggleButton.setToggleState(true, dontSendNotification);
	mushraToggleButton.setWantsKeyboardFocus(false);
	mushraToggleButton.setRadioGroupId(1212);
	mushraToggleButton.setColour(ToggleButton::textColourId, Colours::black);
	mushraToggleButton.setColour(ToggleButton::tickColourId, Colours::black);
	mushraToggleButton.setColour(ToggleButton::tickDisabledColourId, Colours::black);
	mushraToggleButton.addListener(this);

	localizationToggleButton.setButtonText("Method of Adjustment");
	localizationToggleButton.setClickingTogglesState(true);
	localizationToggleButton.setToggleState(false, dontSendNotification);
	localizationToggleButton.setWantsKeyboardFocus(false);
	localizationToggleButton.setRadioGroupId(1212);
	localizationToggleButton.setColour(ToggleButton::textColourId, Colours::black);
	localizationToggleButton.setColour(ToggleButton::tickColourId, Colours::black);
	localizationToggleButton.setColour(ToggleButton::tickDisabledColourId, Colours::black);
	localizationToggleButton.addListener(this);

	initGuiButton.setButtonText("Launch Test");
	initGuiButton.addListener(this);

	saveResultsButton.setButtonText("Save Results");
	saveResultsButton.addListener(this);

	initClientGuiButton.setButtonText("Start Mobile GUI");
	initClientGuiButton.addListener(this);

	dawTxIpLabel.setEditable(false, true, false);
	dawTxPortLabel.setEditable(false, true, false);
	dawRxPortLabel.setEditable(false, true, false);
	clientTxIpLabel.setEditable(false, true, false);
	clientTxPortLabel.setEditable(false, true, false);
	clientRxPortLabel.setEditable(false, true, false);
	hostIpLabel.setEditable(false, true, false);
	dawTxIpLabel.setText("127.0.0.1", dontSendNotification);
	dawTxPortLabel.setText("9696", dontSendNotification);
	dawRxPortLabel.setText("9797", dontSendNotification);
	clientTxIpLabel.setText("169.254.154.129", dontSendNotification);
    //clientTxIpLabel.setText("192.168.1.225", dontSendNotification);
    //clientTxIpLabel.setText("127.0.0.1", dontSendNotification);
	clientTxPortLabel.setText("8282", dontSendNotification);
	clientRxPortLabel.setText("8181", dontSendNotification);
	hostIpLabel.setText("169.254.143.51", dontSendNotification);
    //hostIpLabel.setText("192.168.1.104", dontSendNotification);
	//hostIpLabel.setText("127.0.0.1", dontSendNotification);
	addAndMakeVisible(dawTxIpLabel);
	addAndMakeVisible(dawTxPortLabel);
	addAndMakeVisible(dawRxPortLabel);
	addAndMakeVisible(clientTxIpLabel);
	addAndMakeVisible(clientTxPortLabel);
	addAndMakeVisible(clientRxPortLabel);
	addAndMakeVisible(hostIpLabel);

	// Call resize()
	setSize(1280, 768);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
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
    g.drawText("Client", labelXPos, labelYPos + 50, 50, 25, Justification::centredLeft, true);
	g.drawText("Host", labelXPos, labelYPos + 75, 50, 25, Justification::centredLeft, true);
}

void MainComponent::resized()
{
	talkToDawButton.setBounds(28, 125, 200, 25);
	mushraToggleButton.setBounds(25, 175, 200, 25);
	localizationToggleButton.setBounds(25, 200, 200, 25);
	initGuiButton.setBounds(25, 230, 100, 25);
	saveResultsButton.setBounds(25, 230, 100, 25);
	initClientGuiButton.setBounds(28, getHeight() - 140, 200, 25);

	// OSC LABELS
	int labelXPos = 60;
	int labelYPos = getHeight() - 60 - 25;
	dawTxIpLabel.setBounds(labelXPos, labelYPos, 75, 25);
	dawTxPortLabel.setBounds(labelXPos + 75, labelYPos, 50, 25);
	dawRxPortLabel.setBounds(labelXPos + 125, labelYPos, 50, 25);	
	clientTxIpLabel.setBounds(labelXPos, labelYPos + 25, 75, 25);
	clientTxPortLabel.setBounds(labelXPos + 75, labelYPos + 25, 50, 25);
	clientRxPortLabel.setBounds(labelXPos + 125, labelYPos + 25, 50, 25);
	hostIpLabel.setBounds(labelXPos, labelYPos + 50, 75, 25);

	dawTxIpLabel.setColour(Label::textColourId, Colours::black);
	dawTxPortLabel.setColour(Label::textColourId, Colours::black);
	dawRxPortLabel.setColour(Label::textColourId, Colours::black);
	clientTxIpLabel.setColour(Label::textColourId, Colours::black);
	clientTxPortLabel.setColour(Label::textColourId, Colours::black);
	clientRxPortLabel.setColour(Label::textColourId, Colours::black);
	hostIpLabel.setColour(Label::textColourId, Colours::black);

	dawTxIpLabel.setJustificationType(Justification::centredLeft);	
	dawTxPortLabel.setJustificationType(Justification::centredLeft);	
	dawRxPortLabel.setJustificationType(Justification::centredLeft);
	clientTxIpLabel.setJustificationType(Justification::centredLeft);	
	clientTxPortLabel.setJustificationType(Justification::centredLeft);	
	clientRxPortLabel.setJustificationType(Justification::centredLeft);
	hostIpLabel.setJustificationType(Justification::centredLeft);

	// MC LC SIZE CONFIG
	mc.setSize(1024, 768);
	mc.setTopLeftPosition(getWidth() - 1024, 0);
	lc.setSize(1024, 768);
	lc.setTopLeftPosition(getWidth() - 1024, 0);
}
void MainComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == &talkToDawButton)
	{
		getTracksAndMarkersFromDaw();
	}
	else if (buttonThatWasClicked == &initGuiButton && mushraToggleButton.getToggleState())
	{
		if(!mushraComponentConnected) configureMushra();
		else DBG("disconnect stuffff");
	}
	else if (buttonThatWasClicked == &initGuiButton && localizationToggleButton.getToggleState())
	{
		if (!locComponentConnected) configureLoc();
		else DBG("disconnect stuffff");
	}
	else if (buttonThatWasClicked == &initClientGuiButton)
	{
		if(mushraComponentConnected) mc.initClientGui();
		//else if(locComponentConnected) lc.initClientGui();
	}
	else if (buttonThatWasClicked == &saveResultsButton)
	{
		if (mushraComponentConnected) mc.saveResults();
		else if(locComponentConnected) lc.saveResults();
	}
}

void MainComponent::getTracksAndMarkersFromDaw()
{
	talkToDawButton.setVisible(false);

	hostIp = hostIpLabel.getText();
	dawIp = dawTxIpLabel.getText();
	dawTxPort = dawTxPortLabel.getText().getIntValue();
	dawRxPort = dawRxPortLabel.getText().getIntValue();
	clientIp = clientTxIpLabel.getText();
	clientTxPort = clientTxPortLabel.getText().getIntValue();
	clientRxPort = clientRxPortLabel.getText().getIntValue();

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
    if(passiveLoopCounter == 0)
    {
		if (markerIndex == lastMarkerIndex + 2) countMarkers = false;
		if (trackIndex == lastTrackIndex + 2) countTracks = false;
		
		if(countMarkers)
		{
			DBG("Jump to marker " + String(markerIndex));
			dawTx.sendOscMessage("/marker/" + String(markerIndex));
			markerIndex++;
		}
		if(countTracks)
		{
			DBG("Jump to track " + String(trackIndex));
			dawTx.sendOscMessage("/device/track/select/" + String(trackIndex));
			trackIndex++;
		}
		if(countMarkers == false && countTracks == false)
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
				//talkToDawButton.setVisible(false);
				addAndMakeVisible(mushraToggleButton);
				addAndMakeVisible(localizationToggleButton);
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

void MainComponent::configureMushra()
{
    for(int i = 0; i < numberOfRegions; ++i)
    {
		mc.regionArray.add(new SampleRegion());
		mc.regionArray[i]->dawStartTime = markerTimeArray[i * 2];         // 0 2 4 6
		mc.regionArray[i]->dawStopTime = markerTimeArray[(i * 2) + 1];    // 1 3 5 7
		mc.regionArray[i]->calculateStartEndTimes();
    }
	mc.numberOfSamplesPerRegion = numberOfSamplesPerRegion;
	mc.trackNameArray.addArray(trackNameArray);
	mc.hostIp = hostIp;
	//mc.clientRxPortAtHost =
	mc.connectOsc(dawIp, clientIp, dawTxPort, dawRxPort, clientTxPort, clientRxPort);
	mc.createGui();
	addAndMakeVisible(mc);
	mushraComponentConnected = true;
	removeGuiInitButton();
	addAndMakeVisible(saveResultsButton);
	addAndMakeVisible(initClientGuiButton);
    repaint();
}

void MainComponent::configureLoc()
{
	for (int i = 0; i < numberOfRegions; ++i)
	{
		lc.regionArray.add(new SampleRegion());
		lc.regionArray[i]->dawStartTime = markerTimeArray[i * 2];         // 0 2 4 6
		lc.regionArray[i]->dawStopTime = markerTimeArray[(i * 2) + 1];    // 1 3 5 7
		lc.regionArray[i]->calculateStartEndTimes();
	}

	lc.createTrialArray(numberOfSamplesPerRegion, numberOfRegions);
	lc.trackNameArray.addArray(trackNameArray);
	lc.connectOsc(dawIp, clientIp, dawTxPort, dawRxPort, clientTxPort, clientRxPort);
	addAndMakeVisible(lc);
	locComponentConnected = true;
	removeGuiInitButton();
	addAndMakeVisible(saveResultsButton);
	addAndMakeVisible(initClientGuiButton);
	repaint();
}

void MainComponent::removeGuiInitButton()
{
	initGuiButton.setVisible(false);
	mushraToggleButton.setColour(ToggleButton::textColourId, Colours::grey);
	mushraToggleButton.setColour(ToggleButton::tickColourId, Colours::grey);
	mushraToggleButton.setColour(ToggleButton::tickDisabledColourId, Colours::grey);
	mushraToggleButton.setClickingTogglesState(false);
	localizationToggleButton.setColour(ToggleButton::textColourId, Colours::grey);
	localizationToggleButton.setColour(ToggleButton::tickColourId, Colours::grey);
	localizationToggleButton.setColour(ToggleButton::tickDisabledColourId, Colours::grey);
	localizationToggleButton.setClickingTogglesState(false);
}
