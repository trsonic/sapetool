#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

class OscTransceiver : public juce::OSCReceiver {
 public:
  OscTransceiver();
  ~OscTransceiver();

  void connectSender(juce::String ipToSendTo, int portToSendTo);
  void disconnectSender();
  void connectReceiver(int portToReceiveAt);
  void disconnectReceiver();

  template <typename... Args>
  void sendOscMessage(const juce::String& message, Args&& ... args);

 private:
  juce::OSCSender sender;

  void showConnectionErrorMessage(const juce::String& messageText);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscTransceiver)
};

template <typename... Args>
void OscTransceiver::sendOscMessage(const juce::String& message, Args&& ... args)
{
  if (!sender.send(message, std::forward<Args>(args)...))
    showConnectionErrorMessage("Error: could not send OSC message.");
}

