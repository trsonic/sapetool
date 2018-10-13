/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "LocComponent.h"

//==============================================================================
LocComponent::LocComponent()
{
	startTimer(10);
	//sender.connect("127.0.0.1", 9001);

	//oscrec
	//connect(9002);
	//addListener(this, "/controller");

	addKeyListener(this);
    
	//buttons
    Sp_refresh.setButtonText("Refresh");
    Sp_refresh.addListener (this);
    addAndMakeVisible (Sp_refresh);

    Sp_connect.setButtonText("Connect");
    Sp_connect.setColour(TextButton::buttonColourId, Colours::lightgreen);
    Sp_connect.addListener (this);
    addAndMakeVisible (Sp_connect);

	Az_dec.setButtonText("az-");
	Az_dec.addListener(this);
	addAndMakeVisible(Az_dec);

	Az_inc.setButtonText("az+");
	Az_inc.addListener(this);
	addAndMakeVisible(Az_inc);

	El_dec.setButtonText("el-");
	El_dec.addListener(this);
	addAndMakeVisible(El_dec);

	El_inc.setButtonText("el+");
	El_inc.addListener(this);
	addAndMakeVisible(El_inc);

	AzEl_reset.setButtonText("azimuth / elevation reset");
	AzEl_reset.addListener(this);
	addAndMakeVisible(AzEl_reset);

	Play_reference.setButtonText("Play compressed");
	Play_reference.addListener(this);
	addAndMakeVisible(Play_reference);

	Play_test.setButtonText("Play pointer");
	Play_test.addListener(this);
	addAndMakeVisible(Play_test);

	Go_previous.setButtonText("Previous trial");
	Go_previous.addListener(this);
	addAndMakeVisible(Go_previous);

	Go_next.setButtonText("Next trial");
	Go_next.addListener(this);
	addAndMakeVisible(Go_next);

	Resume.setButtonText("Resume test");
	Resume.addListener(this);
	addAndMakeVisible(Resume);
	Resume.setVisible(false);

    
    // combobox
    cb_portlist.setEditableText (false);
    cb_portlist.setJustificationType (Justification::centred);
    cb_portlist.setTextWhenNothingSelected (String("select device"));
    cb_portlist.addListener (this);
    addAndMakeVisible (cb_portlist);

    // refresh port list
    CBox_portlist = bridge.GetPortInfo();
    cb_portlist.clear();
    cb_portlist.addItemList(CBox_portlist, 1);

}

LocComponent::~LocComponent()
{
	stopTimer();
}

//==============================================================================
void LocComponent::paint (Graphics& g)
{
	g.fillAll(Colours::gainsboro);

	// RECTANGULAR OUTLINE
	g.setColour(Colours::black);
	g.drawRect(getLocalBounds(), 1);
	//g.drawRect(Rectangle<int>(300, 300, 300, 300), 1);

    g.setFont (Font (14.0f));
    g.setColour (Colours::black);

	g.drawText(String(Azimuth, 1) + " deg", 100, 20, 100, 25, Justification::centred, true);
	g.drawText(String(Elevation, 1) + " deg", 100, 55, 100, 25, Justification::centred, true);


	//g.drawText ("3D Audio Panner", 25, 300, 150, 25, Justification::left, true);
	//g.drawText("by Tomasz Rudzki", 25, 325, 150, 25, Justification::left, true);
	//g.drawText("University of York", 25, 350, 150, 25, Justification::left, true);

	if (trainingMode)
	{
		g.drawText("TRAINING !!!", 25, 375, 150, 25, Justification::left, true);
		g.drawText("Training Trial: " + String(currentTrainingTrialIndex + 1) + " of " + String(trainingTrialArray.size()), 25, 400, 150, 25, Justification::left, true);
		g.drawText("Sample: " + String(trainingTrialArray[currentTrainingTrialIndex]->sampleIndex), 25, 450, 150, 25, Justification::left, true);
		g.drawText("Location: " + String(trainingTrialArray[currentTrainingTrialIndex]->regionIndex), 25, 475, 150, 25, Justification::left, true);
	}
	else
	{ 
		g.drawText("Test Trial: " + String(currentTrialIndex + 1) + " of " + String(trialArray.size()), 25, 400, 150, 25, Justification::left, true);
		g.drawText("Sample: " + String(trialArray[trialRandomizationArray[currentTrialIndex]]->sampleIndex), 25, 450, 150, 25, Justification::left, true);
		g.drawText("Location: " + String(trialArray[trialRandomizationArray[currentTrialIndex]]->regionIndex), 25, 475, 150, 25, Justification::left, true);
	}

	if (timeBreak && !testFinished)
	{
		g.drawText("TIME BREAK !!! Click Resume test.", 25, 375, 300, 25, Justification::left, true);
	}
	else if (timeBreak && testFinished)
	{
		g.drawText("TEST FINISHED", 25, 375, 300, 25, Justification::left, true);
	}
	
	//g.drawText("Mouse X: " + String(mouseX) + "\tMouse Y: " + String(mouseY), 25, 535, 250, 25, Justification::left, true);

}

void LocComponent::resized()
{
    
	//buttons
	Sp_refresh.setBounds(350, 20, 100, 25);
	Sp_connect.setBounds(475, 20, 100, 25);
	
	Az_inc.setBounds(25, 20, 50, 25);
	Az_dec.setBounds(225, 20, 50, 25);
	El_dec.setBounds(25, 55, 50, 25);
	El_inc.setBounds(225, 55, 50, 25);
	AzEl_reset.setBounds(25, 90, 250, 25);

	Play_reference.setBounds(25, 150, 150, 45);
	Play_test.setBounds(225, 150, 150, 45);
	Go_previous.setBounds(25, 225, 150, 45);
	Go_next.setBounds(225, 225, 150, 45);
	Resume.setBounds(25, 300, 150, 45);

	//combobox
	cb_portlist.setBounds(350, 55, 225, 25);
    
}

void LocComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	bridge.PortN = cb_portlist.getSelectedItemIndex(); // +1;
}

void LocComponent::buttonClicked (Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == &Sp_refresh)
    {
        CBox_portlist = bridge.GetPortInfo();
        cb_portlist.clear();
        cb_portlist.addItemList(CBox_portlist, 1);
    }
    else if (buttonThatWasClicked == &Sp_connect)
    {
        if(Sp_connect.getButtonText() == "Connect")
        {
            if (bridge.Connect())
            {
                Sp_connect.setColour(TextButton::buttonColourId, Colours::orange);
                Sp_connect.setButtonText("Disconnect");
            }
        }
        else
        {
            bridge.Disconnect();
            Sp_connect.setColour(TextButton::buttonColourId, Colours::lightgreen);
            Sp_connect.setButtonText("Connect");
        }
    }
	else if (buttonThatWasClicked == &Az_dec)
	{
		if (Azimuth > 0) Azimuth--;
		else Azimuth = 359;
		sendAzElOSC();
		trialArray[trialRandomizationArray[currentTrialIndex]]->azimuthResult = Azimuth;
	}
	else if (buttonThatWasClicked == &Az_inc)
	{
		if (Azimuth < 359) Azimuth++;
		else Azimuth = 0;
		sendAzElOSC();
		trialArray[trialRandomizationArray[currentTrialIndex]]->azimuthResult = Azimuth;
	}
	else if (buttonThatWasClicked == &El_dec)
	{
		if (Elevation > -179) Elevation--;
		else Elevation = 180;
		sendAzElOSC();
		if(trainingMode == false)
			trialArray[trialRandomizationArray[currentTrialIndex]]->elevationResult = Elevation;
	}
	else if (buttonThatWasClicked == &El_inc)
	{
		if (Elevation < 180) Elevation++;
		else Elevation = -179;
		sendAzElOSC();
		if (trainingMode == false)
			trialArray[trialRandomizationArray[currentTrialIndex]]->elevationResult = Elevation;
	}
	else if (buttonThatWasClicked == &AzEl_reset)
    {
		Azimuth = 0;
		Elevation = 0;
		sendAzElOSC();
		if (trainingMode == false)
		{
			trialArray[trialRandomizationArray[currentTrialIndex]]->azimuthResult = Azimuth;
			trialArray[trialRandomizationArray[currentTrialIndex]]->elevationResult = Elevation;
		}
		Play_test.triggerClick();
    }
	else if (buttonThatWasClicked == &Play_reference)
	{
		if (!timeBreak)
		{
			if(trainingMode)
				playSample(trainingTrialArray[currentTrainingTrialIndex]->sampleIndex, trainingTrialArray[currentTrainingTrialIndex]->regionIndex);
			else
				playSample(trialArray[trialRandomizationArray[currentTrialIndex]]->sampleIndex, trialArray[trialRandomizationArray[currentTrialIndex]]->regionIndex);

			if (timerOn == false)
			{
				startTime = Time::getMillisecondCounter();
				timerOn = true;
			}
		}
	}
	else if (buttonThatWasClicked == &Play_test)
	{
		playSample(0, 0);
	}
	else if (buttonThatWasClicked == &Go_previous)
	{
		timerOn = false;
		stopPlayback();
		Azimuth = 0;
		Elevation = 0;
		sendAzElOSC();
		if(trainingMode == true && currentTrainingTrialIndex > 0) currentTrainingTrialIndex--;
		if(trainingMode == false && currentTrialIndex > 0) currentTrialIndex--;
	}
	else if (buttonThatWasClicked == &Go_next)
	{
		if (timerOn)
		{
			int timeTaken = Time::getMillisecondCounter() - startTime;
			timerOn = false;
			if (trainingMode == false)
			{
				trialArray[trialRandomizationArray[currentTrialIndex]]->timeTaken = timeTaken;
				trialArray[trialRandomizationArray[currentTrialIndex]]->timeStamp = Time::getCurrentTime().formatted("%Y-%m-%d %H:%M:%S");
			}
			stopPlayback();
			Azimuth = 0;
			Elevation = 0;
			sendAzElOSC();
			if (trainingMode == true && currentTrainingTrialIndex == trainingTrialArray.size() - 1)
				trainingMode = false;
			else if (trainingMode == true && currentTrainingTrialIndex < trainingTrialArray.size() - 1)
				currentTrainingTrialIndex++;
			else if (trainingMode == false && currentTrialIndex < trialArray.size() - 1)
			{
				currentTrialIndex++;
				if (currentTrialIndex == 30)
				{
					timeBreak = true; // enable break in the middle of the test
					Resume.setVisible(true);
				}
			}
			else if (trainingMode == false && currentTrialIndex == trialArray.size() - 1)
			{
				timeBreak = true;
				testFinished = true;
			}

		}
	}
	else if (buttonThatWasClicked == &Resume)
	{
		timeBreak = false;
		Resume.setVisible(false);
	}
	repaint();
}

bool LocComponent::keyPressed(const KeyPress &k, Component *c)
{
	if (k == KeyPress::leftKey) {
		Az_inc.triggerClick();
	}
	else if (k == KeyPress::rightKey) {
		Az_dec.triggerClick();
	}
	else if (k == KeyPress::downKey) {
		El_dec.triggerClick();
	}
	else if (k == KeyPress::upKey) {
		El_inc.triggerClick();
	}
	return true;
}

void LocComponent::timerCallback()
{
	bridge.Read();
	
	if (bridge.Az_dec_state)
	{
		Az_dec.triggerClick();
		bridge.Az_dec_state = false;
	}
	if (bridge.Az_inc_state)
	{
		Az_inc.triggerClick();
		bridge.Az_inc_state = false;
	}
	if (bridge.El_dec_state)
	{
		El_dec.triggerClick();
		bridge.El_dec_state = false;
	}
	if (bridge.El_inc_state)
	{
		El_inc.triggerClick();
		bridge.El_inc_state = false;
	}
	if (bridge.Play_reference)
	{
		Play_reference.triggerClick();
		bridge.Play_reference = false;
	}
	if (bridge.Play_test)
	{
		Play_test.triggerClick();
		bridge.Play_test = false;
	}
	if (bridge.AzEl_reset)
	{
		AzEl_reset.triggerClick();
		bridge.AzEl_reset = false;
	}
	if (bridge.Go_next)
	{
		Go_next.triggerClick();
		bridge.Go_next = false;
	}
}

void LocComponent::oscMessageReceived(const OSCMessage& message)
{
	// DAW

	if (message.size() == 1 && message.getAddressPattern() == "/time" && message[0].isFloat32())
	{
		currentPosition = message[0].getFloat32();

		//dawTimeLabel.setText("DAW Time: " + String(currentPosition, 3), NotificationType::dontSendNotification);
		//sampleTimeLabel.setText("Sample Time: " + String(currentPosition - regionArray[currentRegion]->startTime, 3), NotificationType::dontSendNotification);

		if (currentPosition >= regionArray[currentRegion]->stopTime)
		{
			if (adjRectClicked) playSampleLoop();
			else stopPlayback();
		}
	}	
	
	
	if (message.size() == 1 && message[0].isString())
	{
		if (message[0].getString() == "Reset") AzEl_reset.triggerClick();
	}
}

void LocComponent::connectOsc(String dawIp, String clientIp, int dawTxPort, int dawRxPort, int clientTxPort, int clientRxPort)
{
	// DAW
	dawTx.connectSender(dawIp, dawTxPort);
	dawRx.connectReceiver(dawRxPort);
	dawRx.addListener(this);

	// CLIENT
	clientTx.connectSender(clientIp, clientTxPort);
	clientRx.connectReceiver(clientRxPort);
	clientRx.addListener(this);

	clientRxPortAtHost = clientRxPort;

	// DAW state
	stopPlayback();
}

void LocComponent::createTrialArray(int numberOfSamplesPerRegion, int numberOfRegions)
{
	// Prepare test trial samples
	int numberOfTrainingTrials = 3;
	for (int i = 0; i < numberOfTrainingTrials; ++i)
	{
		trainingTrialArray.add(new LocTrial);
	}
	
	trainingTrialArray[0]->sampleIndex = 10;
	trainingTrialArray[0]->regionIndex = 3;
	trainingTrialArray[1]->sampleIndex = 10;
	trainingTrialArray[1]->regionIndex = 2;
	trainingTrialArray[2]->sampleIndex = 10;
	trainingTrialArray[2]->regionIndex = 4;

	// prepare proper test
	int numberOfTrials = (numberOfSamplesPerRegion - 1) * (numberOfRegions - 1);
	for (int i = 0; i < numberOfTrials; ++i)
	{
		trialArray.add(new LocTrial);
		trialArray[i]->sampleIndex = ceil(i / (numberOfRegions - 1)) + 1;
		trialArray[i]->regionIndex = (i % (numberOfRegions - 1)) + 1;
	}
	trialRandomizationArray.resize(numberOfTrials);
	for (int j = 0; j < numberOfTrials; ++j) trialRandomizationArray[j] = j;
	std::random_device seed;
	std::mt19937 rng(seed());
	std::shuffle(trialRandomizationArray.begin(), trialRandomizationArray.end(), rng);
	//for (int i = 0; i < numberOfTrials; ++i)
	//{
	//	trialArray[i]->trialIndex = trialRandomizationArray[i];
	//}
}

void LocComponent::sendAzElOSC()
{
	float tempAz;
	if (Azimuth < 180) tempAz = Azimuth * -1;
	if (Azimuth >= 180) tempAz = 360 - Azimuth;
	AzimuthOSC = (float)jmap(tempAz, (float)-180, (float)180, (float)0, (float)1);
	ElevationOSC = (float)jmap(Elevation, (float)-180, (float)180, (float)0, (float)1);

	if (AzimuthOSC != AzimuthOSCsent )
	{
		dawTx.sendOscMessage("/Azimuth/", (float)AzimuthOSC);
		AzimuthOSCsent = AzimuthOSC;
	}
	if (ElevationOSC != ElevationOSCsent)
	{
		dawTx.sendOscMessage("/Elevation/", (float)ElevationOSC);
		ElevationOSCsent = ElevationOSC;
	}
}


void LocComponent::playSample(int track, int region)
{
	if (playback == true) stopPlayback(); //dawTx.sendOscMessage("/stop");
	dawTx.sendOscMessage("/time", regionArray[region]->startTime);
	if (track != currentTrack)
	{
		dawTx.sendOscMessage("/track/" + String(currentTrack + 1) + "/solo", 0);
	}
	dawTx.sendOscMessage("/track/" + String(track + 1) + "/solo", 1);
	dawTx.sendOscMessage("/play");
	playback = true;
	currentTrack = track;
	currentRegion = region;
}

void LocComponent::playSampleLoop()
{
	dawTx.sendOscMessage("/time", regionArray[currentRegion]->startTime);
}

void LocComponent::stopPlayback()
{
	dawTx.sendOscMessage("/track/" + String(0) + "/mute", 1);
	dawTx.sendOscMessage("/stop");
	currentPosition = regionArray[currentRegion]->startTime;
	dawTx.sendOscMessage("/time", currentPosition);
	dawTx.sendOscMessage("/soloreset");
	//Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100);
	dawTx.sendOscMessage("/track/" + String(0) + "/mute", 0);
}

void LocComponent::saveResults()
{
	File chosenFile;
	String dataToSave;

#if JUCE_MODAL_LOOPS_PERMITTED
	FileChooser fc("Choose a file to save...", File::getCurrentWorkingDirectory(),
		"*.txt", true);
	if (fc.browseForFileToSave(true))
	{
		chosenFile = fc.getResult().withFileExtension(".txt");

		dataToSave = "##### RAW RESULTS #####\n\n";
		dataToSave += "smp\tloc\taz\tel\ttin\ttime\tdate\t\n";
		for (int i = 0; i < trialArray.size(); i++)
		{
			dataToSave += String(trialArray[i]->sampleIndex) + "\t" + String(trialArray[i]->regionIndex) + "\t";
			dataToSave += String(trialArray[i]->azimuthResult) + "\t" + String(trialArray[i]->elevationResult) + "\t";
			
			std::vector<int>::iterator it = std::find(trialRandomizationArray.begin(), trialRandomizationArray.end(), i);
			int index = std::distance(trialRandomizationArray.begin(), it);
			dataToSave += String(index + 1) + "\t";

			float timeTakenSeconds = (float) trialArray[i]->timeTaken / 1000;
			dataToSave += String(timeTakenSeconds, 0) + "\t" + String(trialArray[i]->timeStamp) + "\n";
		}

		dataToSave += "\n##### TRACK NAMES #####\n\n";
		for (int i = 0; i < trackNameArray.size(); i++)
		{
			dataToSave += trackNameArray[i] + "\n";
		}
		
		dataToSave += "\n" + String("##### DATE AND TIME #####\n\n");
		dataToSave += Time::getCurrentTime().formatted("%Y-%m-%d %H:%M:%S") + "\n";

		dataToSave += "\n" + String("##### REGIONS #####\n\n");
		for (int i = 0; i < regionArray.size(); i++)
		{
			dataToSave += "Region: " + String(i + 1) + "\n";
			dataToSave += "Beginning: " + String(regionArray[i]->dawStartTime) + "\n";
			dataToSave += "End: " + String(regionArray[i]->dawStopTime) + "\n\n";
		}

		chosenFile.replaceWithText(dataToSave);
	}
#endif
}

void LocComponent::mouseDrag(const MouseEvent& e)
{
	//if (e.mouseWasDraggedSinceMouseDown() && adjRectClicked)
	//{
	//	mouseX = e.position.getX() - e.getMouseDownX();
	//	mouseY = e.getMouseDownY() - e.position.getY();
	//	
	//	int pixelsPerDegree = 5;

	//	float draggedAzimuth = mouseDownAz - floor(mouseX / pixelsPerDegree);
	//	float draggedElevation = mouseDownEl + floor(mouseY / pixelsPerDegree);

	//	if (draggedAzimuth >=  0 && draggedAzimuth < 360) Azimuth = draggedAzimuth;
	//	else if (draggedAzimuth >= 360) Azimuth = draggedAzimuth - 360;
	//	else if (draggedAzimuth < 0) Azimuth = draggedAzimuth + 360;

	//	if (draggedElevation > -180 && draggedElevation <= 180) Elevation = draggedElevation;
	//	else if (draggedElevation > 180) Elevation = draggedElevation - 360;
	//	else if   (draggedElevation <= -180) Elevation = draggedElevation + 360;

	//	sendAzElOSC();
	//	repaint();
	//}
}

void LocComponent::mouseDown(const MouseEvent& e)
{
	//if (e.getMouseDownX() > 300 && e.getMouseDownX() < 600 && e.getMouseDownY() > 300 && e.getMouseDownY() < 600)
	//{
	//	adjRectClicked = true;
	//	mouseDownAz = Azimuth;
	//	mouseDownEl = Elevation;
	//	Play_test.triggerClick();
	//}
}

void LocComponent::mouseUp(const MouseEvent& e)
{
	//if (adjRectClicked)
	//{
	//	stopPlayback();
	//	adjRectClicked = false;
	//}
}
