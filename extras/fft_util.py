import numpy as np
from scipy.fft import fft
import wave, struct

import matplotlib.pyplot as plt

#interface
#---------------------------------------------------------------------------------#

def getImportantFrequencies(audio_data):
    window = getWindow(0.0, audio_data) #lets getWindow know to take the full audiofile
    #window = applyWindowingFunction(window)
    frequencyData = applyFFT(window, audio_data.getframerate())
    localMax = getLocalMaxima(frequencyData, audio_data.getframerate())
    interestingFreq = getMaximaPerOctave(localMax, 5, 20, 10)
    return interestingFreq

def getAmplitudes(audio_data, important_frequencies):
    audio_data.rewind()
    r = []
    time = 0
    for i in range(len(important_frequencies)):
        r.append([])
    grainSize = 0.1 #seconds
    while(time <= 1.0):
        try:
            window = getWindow(grainSize, audio_data)
        except:
            break
        frequencyData = applyFFT(window, audio_data.getframerate())
        for i in range(len(important_frequencies)):
            amplitude = getAmplitude(frequencyData, important_frequencies[i], audio_data.getframerate(), len(window))
            r[i].append([time, amplitude])
        time = grainSize + time
    return r

#---------------------------------------------------------------------------------#

def getWindow(length, waveFile):
    if(length > 0.0):
        numSamples = length * waveFile.getframerate()
        if numSamples > waveFile.getnframes():
            numSamples = waveFile.getnframes()
    else:
        numSamples = waveFile.getnframes()
    window = []
    for x in range(int(numSamples)):
        f = waveFile.readframes(1)
        if(f==b''):
            raise IndexError()
        window.append((struct.unpack('<h', f[:2])[0])/(2**15))
    return window

def applyWindowingFunction(window):
    modifier = np.hanning(len(window))
    for i in range(len(window)):
        window[i] = window[i] * modifier[i]
    return modifier

def applyFFT(window, framerate):
    r = np.abs(fft(window))
    return r
    dataInHzGain = []
    for index in range(len(r)):
        freqHz = indexToFreq(index, framerate, len(window))
        amp = toGain(r[index], len(window))
        dataInHzGain.append([freqHz, amp])
    return dataInHzGain

def getAmplitude(frequencyData, frequency, framerate, windowsize):
    i = int(toIndex(frequency, framerate, windowsize))
    a0 = frequencyData[i]
    a1 = frequencyData[i+1]
    if a0 > a1:
        return a0
    else:
        return a1

def indexToFreq(index, framerate, windowSize):
    freq =  framerate * index / windowSize
    return freq

def toIndex(freq, framerate, windowsize):
    index = freq * windowsize / framerate
    return index

def toGain(fftAmp, windowLen):
    adjustedGain = fftAmp * 2 / windowLen
    return adjustedGain

def getMaximaPerOctave(pts, n, base, octaves):
    split = []
    important = []
    for i in range(octaves):
        lowbound = base*(2**i)
        highbound = lowbound*2
        f = list(filter(lambda l: (l[0]>=lowbound and l[0]<highbound), pts)) 
        split.append(f)
    for s in split:
        local_freq = getMaxima(s, n)
        for f in local_freq:
            important.append(f[0])
    return sorted(important)


#given a set of pts, return the local maxima
def getLocalMaxima(data, frameRate):
    maxima = []
    for i in range(1, len(data)-1):
        if data[i] > data[i-1] and data[i] > data[i+1]:
            maxima.append([indexToFreq(i, frameRate, len(data)), toGain(data[i], len(data))])
    return maxima

def linearInterpolate(a, b, remainder):
    return a*(1-remainder)+b*remainder

def getMaxima(pts, n):
    local_maxima = []
    global_maxima = []
    for x in range(len(pts)-1):
        if(x != 0):
            if (pts[x][1] > pts[x-1][1] and pts[x][1] > pts[x+1][1]):
                local_maxima.append(pts[x])
    local_maxima = sorted(local_maxima, key=lambda pt: pt[1])
    if (n > len(local_maxima)): n = len(local_maxima)
    for i in range(n):
        global_maxima.append(local_maxima.pop())
    return global_maxima


