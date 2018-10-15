# SAPETool - A DAW-based interactive tool for perceptual spatial audio evaluation

This repository contains a source code of the tool created for the purpose of perceptual evaluation of Ambisonic recordings encoded with Opus codec at different orders and bit rates. If you would like to use the tool for your research, please contact me (tr837 (at) york.ac.uk) and I will be happy to assist with setting up the system.

![alt text](https://github.com/trsonic/sapetool/blob/master/2018_AESNY_SAPETool_POSTER.jpg "145th AES Convention poster")

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
- MUSHRA [1] and Method of Adjustment [2] implemented

## Implementation
- Created in C++ utilizing JUCE framework [3]
- Networked communication done by Open Sound Control messages
- Method of Adjustment interface based on Arduino and rotary encoders

## Usage
- Import test audio samples to Reaper.
- Set up the required signal processing chain.
- Launch the Main Listening Test App.
- Configuration of the listening test app will be done automatically based on the DAW session structure.
- Launch the mobile user interface app.
- Conduct the test and save the results into a text file.

**References**

[1] *ITU-R, “BS.1534-3: Method for the subjective assessment of intermediate quality levels of coding systems,” 2015.*

[2] *Thresh, L., Armstrong, C., and Kearney, G., “A Direct Comparison of Localization Performance When Using First, Third, and Fifth Ambisonics Order for Real Loudspeaker and Virtual Loudspeaker Rendering,” in Audio Engineering Society Convention 143, Audio Engineering Society, 2017.*

[3] *juce.com, github.com/WeAreROLI/JUCE/*
