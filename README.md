# SAPETool - A DAW-based interactive tool for perceptual spatial audio evaluation

This repository contains a source code of the tool created for the purpose of perceptual evaluation of Ambisonic recordings encoded with Opus codec at different orders and bit rates. If you would like to use the tool for your research, please contact me (tr837 (at) york.ac.uk) and I will be happy to assist with setting up the system.

The software has been described in the 145th AES Convention [Engineering Brief 465](http://www.aes.org/e-lib/browse.cfm?elib=19730).

## Applications
Perceptual evaluation of: spatial audio codecs, binaural rendering engines, virtual acoustics.

## Key features
- Multichannel playback of the listening test stimuli
- Flexible signal processing – each sample can have its own designated decoder
- Easy implementation of head tracking
- The app can be run on macOS and Windows
- Mobile participant’s interface (iOS and Android)
- Straightforward test preparation done by setting up the DAW session
- MUSHRA and Method of Adjustment implemented

## Implementation
- Created in C++ utilizing JUCE framework
- Networked communication done by Open Sound Control messages
- Method of Adjustment interface based on Arduino and rotary encoders

## Usage
- Import test audio samples to Reaper.
- Set up the required signal processing chain.
- Launch the Main Listening Test App.
- Configuration of the listening test app will be done automatically based on the DAW session structure.
- Launch the mobile user interface app.
- Conduct the test and save the results into a text file.
