/*
  ==============================================================================

    MushraClientComponent.cpp
    Created: 16 Aug 2018 4:43:39pm
    Author:  tr837

  ==============================================================================
*/

#include "MushraClientComponent.h"

MushraClientComponent::MushraClientComponent()
{
}

MushraClientComponent::~MushraClientComponent()
{
}

//==============================================================================
void MushraClientComponent::paint (Graphics& g)
{
	g.fillAll(Colours::gainsboro);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Mushra: " + String(getWidth()) + " x " + String(getHeight()), getLocalBounds(), Justification::bottomRight, true);
    
	int linesStartX = 125;
	int linesEndX = 925;
	int linesStartY = 0.06 * getHeight();
	int linesEndY = 0.66 * getHeight();
	int linesYinterval = (linesEndY - linesStartY) / 5;
    
    Line<float> line1(Point<float>(linesStartX, linesStartY), Point<float>(linesEndX, linesStartY));
    Line<float> line2(Point<float>(linesStartX, linesStartY + linesYinterval * 1), Point<float>(linesEndX, linesStartY + linesYinterval * 1));
    Line<float> line3(Point<float>(linesStartX, linesStartY + linesYinterval * 2), Point<float>(linesEndX, linesStartY + linesYinterval * 2));
    Line<float> line4(Point<float>(linesStartX, linesStartY + linesYinterval * 3), Point<float>(linesEndX, linesStartY + linesYinterval * 3));
    Line<float> line5(Point<float>(linesStartX, linesStartY + linesYinterval * 4), Point<float>(linesEndX, linesStartY + linesYinterval * 4));
    Line<float> line6(Point<float>(linesStartX, linesEndY), Point<float>(linesEndX, linesEndY));
    
    float dashPattern[2];
    dashPattern[0] = 4.0;
    dashPattern[1] = 8.0;
    g.setColour(Colours::dimgrey);
    
    //g.drawDashedLine(line1, dashPattern, 2, 1.0f);
    g.drawDashedLine(line2, dashPattern, 2, 1.0f);
    g.drawDashedLine(line3, dashPattern, 2, 1.0f);
    g.drawDashedLine(line4, dashPattern, 2, 1.0f);
    g.drawDashedLine(line5, dashPattern, 2, 1.0f);
    //g.drawDashedLine(line6, dashPattern, 2, 1.0f);
    
    int textStartX = linesStartX - 60;
    int textWidth = 80;
    g.drawText("Excellent", textStartX, linesStartY, textWidth, linesYinterval, Justification::centredRight, true);
    g.drawText("Good", textStartX, linesStartY + linesYinterval * 1, textWidth, linesYinterval, Justification::centredRight, true);
    g.drawText("Fair", textStartX, linesStartY + linesYinterval * 2, textWidth, linesYinterval, Justification::centredRight, true);
    g.drawText("Poor", textStartX, linesStartY + linesYinterval * 3, textWidth, linesYinterval, Justification::centredRight, true);
    g.drawText("Bad", textStartX, linesStartY + linesYinterval * 4, textWidth, linesYinterval, Justification::centredRight, true);
    
	g.setColour(Colours::black);
	g.drawText("Trial: " + String(currentTrialIndex + 1) + " of " + String(numberOfTrials), 450, 700, 200, 20, Justification::centredLeft, true);
    
}

void MushraClientComponent::resized()
{
	auto rateRectWidth = 0.8 * getWidth();
	auto rateRectHeight = 0.6 * getHeight();

	auto sliderSpacing = rateRectWidth / (numberOfSamplesPerRegion + 1);

	auto sliderWidth = 50;
	auto sliderHeight = rateRectHeight;

	auto buttonWidth = 65;
	auto buttonHeight = buttonWidth;
	auto labelHeight = 0.5 * buttonWidth;


	auto sliderPositionX = (getWidth() - rateRectWidth) / 2 + sliderWidth / 2;
	auto sliderPositionY = 0.06 * getHeight() + sliderHeight / 2;
	auto labelPositionY = sliderPositionY + sliderHeight / 2 + labelHeight * 0.5;
	auto buttonPositionY = sliderPositionY + sliderHeight / 2 + buttonHeight;

	for (int i = 0; i < numberOfSamplesPerRegion; ++i)
	{
		rateSampleSliderArray[i]->setSize(sliderWidth, sliderHeight);
		rateSampleSliderLabelArray[i]->setSize(buttonWidth, labelHeight);
		playSampleButtonArray[i]->setSize(buttonWidth, buttonHeight);

		rateSampleSliderArray[i]->setCentrePosition(sliderPositionX + (i + 1) * sliderSpacing, sliderPositionY);
		rateSampleSliderLabelArray[i]->setCentrePosition(sliderPositionX + (i + 1) * sliderSpacing, labelPositionY);
		playSampleButtonArray[i]->setCentrePosition(sliderPositionX + (i + 1) * sliderSpacing, buttonPositionY);
	}

	playReference.setBounds(25, buttonPositionY - buttonHeight / 2, 80, buttonHeight);
	stopPlaybackB.setBounds(25, buttonPositionY + buttonHeight, 80, buttonHeight);
	loopSlider.setBounds(sliderPositionX, buttonPositionY + buttonHeight, rateRectWidth, labelHeight);

	loopTB.setBounds(0.8 * getWidth(), 0.875 * getHeight(), 80, labelHeight);
	goToPrevious.setBounds(sliderPositionX, buttonPositionY + 2 * buttonHeight, 100, labelHeight);
	goToNext.setBounds(sliderPositionX + 125, buttonPositionY + 2 * buttonHeight, 100, labelHeight);

}

void MushraClientComponent::buttonClicked(Button* buttonThatWasClicked)
{
	bool playSampleButtonState = buttonThatWasClicked->getProperties()["playSampleButton"];

	if (playSampleButtonState)
	{
		int buttonIndex = buttonThatWasClicked->getProperties()["buttonIndex"];
		hostTx.sendOscMessage("/host/play", buttonIndex);
	}
	else if (buttonThatWasClicked == &playReference)
	{
		hostTx.sendOscMessage("/host", "playref");
	}
	else if (buttonThatWasClicked == &stopPlaybackB)
	{
		hostTx.sendOscMessage("/host", "stopplayback");
	}
	else if (buttonThatWasClicked == &loopTB)
	{
		loop = loopTB.getToggleState();
		if(loop) hostTx.sendOscMessage("/host","loopon");
		else hostTx.sendOscMessage("/host", "loopoff");
	}
	else if (buttonThatWasClicked == &goToNext)
	{
		hostTx.sendOscMessage("/host", "gonext");
	}
	else if (buttonThatWasClicked == &goToPrevious)
	{
		hostTx.sendOscMessage("/host", "goprev");
	}
}

void MushraClientComponent::sliderValueChanged(Slider *sliderThatWasChanged)
{
	bool rateSampleSliderChanged = sliderThatWasChanged->getProperties()["rateSampleSlider"];

	if (rateSampleSliderChanged)
	{
		int sliderIndex = sliderThatWasChanged->getProperties()["sliderIndex"];
		rateSampleSliderLabelArray[sliderIndex]->setText(String(sliderThatWasChanged->getValue(),0), dontSendNotification);
		hostTx.sendOscMessage("/host/slider/", (int) sliderIndex, (float) sliderThatWasChanged->getValue());

	}
	else if (sliderThatWasChanged == &loopSlider)
	{
		hostTx.sendOscMessage("/host/loopslider/", loopSlider.getRange().getEnd(), loopSlider.getValue(), loopSlider.getMinValue(), loopSlider.getMaxValue());
		DBG("slider updated by click");
	}
}

void MushraClientComponent::oscMessageReceived(const OSCMessage& message)
{
	if (message.size() == 2 && message.getAddressPattern() == "/client/slider" && message[0].isInt32() && message[1].isFloat32())
	{
		if(rateSampleSliderArray[message[0].getInt32()]->getValue() != message[1].getFloat32())
            rateSampleSliderArray[message[0].getInt32()]->setValue(message[1].getFloat32());
	}
	else if (message.size() == 4 && message.getAddressPattern() == "/client/loopslider" && message[0].isFloat32() && message[1].isFloat32() && message[2].isFloat32() && message[3].isFloat32())
	{
		loopSlider.setRange(0, message[0].getFloat32(), dontSendNotification);
		loopSlider.setValue(message[1].getFloat32(), dontSendNotification);
		loopSlider.setMinValue(message[2].getFloat32(), dontSendNotification);
		loopSlider.setMaxValue(message[3].getFloat32(), dontSendNotification);
		String currentSystemTime = Time::getCurrentTime().toString(true, true, true, true);
		DBG("slider updated by osc " + currentSystemTime);
	}
	else if (message.size() == 2 && message.getAddressPattern() == "/client/currenttrial" && message[0].isInt32() && message[1].isInt32())
	{
		currentTrialIndex = message[0].getInt32();
		numberOfTrials = message[1].getInt32();
		repaint();
	}
}

void MushraClientComponent::createGui()
{
	for (int i = 0; i < numberOfSamplesPerRegion; ++i)
	{
		rateSampleSliderArray.add(new Slider());
		rateSampleSliderArray[i]->getProperties().set("rateSampleSlider", true);
		rateSampleSliderArray[i]->getProperties().set("sliderIndex", i);
		rateSampleSliderArray[i]->setSliderStyle(Slider::LinearBarVertical);
		rateSampleSliderArray[i]->setTextBoxIsEditable(false);
		rateSampleSliderArray[i]->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		rateSampleSliderArray[i]->setRange(0, 100, 1);
		rateSampleSliderArray[i]->setValue(0);
		rateSampleSliderArray[i]->addListener(this);
		addAndMakeVisible(rateSampleSliderArray[i]);
	}

	for (int i = 0; i < numberOfSamplesPerRegion; ++i)
	{
		rateSampleSliderLabelArray.add(new Label());
		rateSampleSliderLabelArray[i]->setText(String(0), dontSendNotification);
		rateSampleSliderLabelArray[i]->setJustificationType(Justification::centred);
		rateSampleSliderLabelArray[i]->setColour(Label::textColourId, Colours::black);
		addAndMakeVisible(rateSampleSliderLabelArray[i]);
	}

	StringArray playButtonAlphabet = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "X", "Y", "Z" };
	for (int i = 0; i < numberOfSamplesPerRegion; ++i)
	{
		playSampleButtonArray.add(new TextButton());
		playSampleButtonArray[i]->getProperties().set("playSampleButton", true);
		playSampleButtonArray[i]->getProperties().set("buttonIndex", i);
		if (i < playButtonAlphabet.size()) playSampleButtonArray[i]->setButtonText(playButtonAlphabet[i]);
		playSampleButtonArray[i]->addListener(this);
		addAndMakeVisible(playSampleButtonArray[i]);
	}

	playReference.setButtonText("Reference");
	playReference.addListener(this);
	if (isReferenceButtonVisible) addAndMakeVisible(playReference);

	loopSlider.setSliderStyle(Slider::ThreeValueHorizontal);
	loopSlider.setRange(0, 1);
	loopSlider.setMinAndMaxValues(0, 1);
	loopSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	loopSlider.addListener(this);
	addAndMakeVisible(loopSlider);

	loopTB.setButtonText("Loop");
	loopTB.setToggleState(loop, dontSendNotification);
	loopTB.setColour(ToggleButton::textColourId, Colours::black);
	loopTB.setColour(ToggleButton::tickColourId, Colours::black);
	loopTB.setColour(ToggleButton::tickDisabledColourId, Colours::black);
	loopTB.addListener(this);
	addAndMakeVisible(loopTB);

	stopPlaybackB.setButtonText("Stop");
	stopPlaybackB.addListener(this);
	addAndMakeVisible(stopPlaybackB);

	goToPrevious.setButtonText("Previous Trial");
	goToPrevious.addListener(this);
	addAndMakeVisible(goToPrevious);

	goToNext.setButtonText("Next Trial");
	goToNext.addListener(this);
	addAndMakeVisible(goToNext);

	resized();
}

void MushraClientComponent::connectOsc()
{
	// OSC sender and receiver connect
	hostTx.connectSender(ipToSendTo, portToSendTo);
	hostRx.connectReceiver(portToReceiveAt);
	hostRx.addListener(this);
}
