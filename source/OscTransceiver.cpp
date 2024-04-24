#include "OscTransceiver.h"

OscTransceiver::OscTransceiver() {}

OscTransceiver::~OscTransceiver() {}

void OscTransceiver::connectSender(juce::String ipToSendTo, int portToSendTo) {
  if (!sender.connect(ipToSendTo, portToSendTo))
    showConnectionErrorMessage("Error: could not connect sender to UDP port: " +
                               juce::String(portToSendTo));
}

void OscTransceiver::disconnectSender() {
  if (!sender.disconnect())
    showConnectionErrorMessage(
        "Error: could not disconnect sender from the currently used UDP port");
}

void OscTransceiver::connectReceiver(int portToReceiveAt) {
  if (!connect(portToReceiveAt))
    showConnectionErrorMessage(
        "Error: could not connect receiver to UDP port " +
        juce::String(portToReceiveAt));
}

void OscTransceiver::disconnectReceiver() {
  if (!disconnect())
    showConnectionErrorMessage(
        "Error: could not disconnect receiver from the currently used UDP "
        "port.");
}

void OscTransceiver::showConnectionErrorMessage(
    const juce::String& messageText) {
  juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                         "Connection error", messageText, "OK");
}
