#high level control for oscillator resynthesis technique
#depends on implementations of getImportantFrequencies and getAmplitudes in fft_util
#Gabriel Brown, September 2020

import EnvelopeSynth
import fft_util
import wave, struct
import sys

if len(sys.argv) < 3:
    print("takes 2 arguments: filename of wav file to analyze, and filename of output")
    exit()

audio_filename = sys.argv[1]
output_filename = sys.argv[2]
audio_data = wave.open(audio_filename, 'r') #we read audio data from sample

e = EnvelopeSynth.EnvelopeSynth()

freqList = fft_util.getImportantFrequencies(audio_data)
for freq in freqList:
    e.addPartial(freq)

ampList = fft_util.getAmplitudes(audio_data, freqList)
for freq_index in range(len(ampList)):
    for amplitude_data in ampList[freq_index]:
        time = amplitude_data[0]
        amplitude = amplitude_data[1]
        e.addPoint(freq_index, time, amplitude)

e.save(output_filename)
