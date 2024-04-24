#pragma once

class SampleRegion {
 public:
  SampleRegion() {}
  ~SampleRegion() {}

  float getRegionLength() {
    float length = dawStopTime - dawStartTime;
    return length;
  }

  void calculateStartEndTimes() {
    startTime = dawStartTime + loopStartOffset;
    stopTime = dawStopTime - loopEndOffset;
  }

  float startTime, stopTime;
  float dawStartTime, dawStopTime;
  float loopStartOffset = 0, loopEndOffset = 0;

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleRegion)
};
