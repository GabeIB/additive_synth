This folder includes a set of python scripts for programatically generating control data for the additive synthesizer.

EnvelopeSynth.py takes care of the implementation details of how control data is written.

sample_resynthesis.py is a script that takes an audio file (in wav format) and generates control data for the additive synthesizer using a technique called oscillator bank resynthesis.

synthTest.py is an example program which hopefully makes clear how one would use EnvelopeSynth.py to write their own programs for generating synth control data. For the sake of ease, I will include it below:

#make sure that EnvelopeSynth.py is in the same directory as your program
import EnvelopeSynth

#you must make an EnvelopeSynth object first
e = EnvelopeSynth.EnvelopeSynth()

#then you must add partials. AddPartial takes frequency as an argument
#remember that this is just the relative frequency, and the ratio between partials is most important
e.addPartial(400)
e.addPartial(600)
e.addPartial(900)

#Then, we add points to create amplitude envelopes for each partial
#addPoint takes the index of the partial, then the time value of the point, and then the amplitude value of the point
#as of now, time and amplitude must be between 0 and 1 (inclusive)
#the index of a partial is related to the order you create the partials
#in this case, partial 0 is 400hz, partial 1 is 600hz and partial 2 is 900hz
e.addPoint(0, 0.5, 0.3) #adds point to 400Hz partial at t=0.5 amplitude=0.3
e.addPoint(1, 0.2, 0.1)
e.addPoint(2, 0.7, 0.5)

#you must finally save your control data to a file.
#you then must load that file in the synthesizer to use the preset.
e.save("controlData.ctl")
