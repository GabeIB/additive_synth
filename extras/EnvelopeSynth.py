class edge:
    def __init__(self, x, y, skew):
        self.x = x
        self.y = y
        self.skew = skew
        self.version = 1.0

    def toString(self):
        return str(self.version)+" "+str(self.x)+" "+str(self.y)+" "+str(self.skew)

def getX(edge):
    return edge.x

class envelope:
    def __init__(self, frequency=440.0):
        self.frequency = frequency
        self.edgeList = []

    def addEdge(self, e):
        self.edgeList.append(e)
        self.edgeList.sort(key=getX)

    def toString(self):
        r = ""
        r += "sin " + str(self.frequency) + "\n"
        for edge in self.edgeList:
            r += edge.toString()+"\n"
        return r

class envBank:
    def __init__(self):
        self.envList = []
        self.version = 1.0
        self.base_freq = 440.0
        self.duration = 1.0
        self.oscNum = 0

    def addEnv(self, freq):
        self.envList.append(envelope(freq))
        self.oscNum += 1

    def toString(self):
        r = ""
        r += "vesion: "+str(self.version)+"\n"
        r += "base frequency: " + str(self.base_freq) + "\n"
        r += "duration: " + str(self.duration) + "\n"
        r += "number of oscillators: " + str(self.oscNum) + "\n"
        r += "{\n"
        for env in self.envList:
            r += env.toString()
            r += ";\n"
        r += "}\n"
        return r

class EnvelopeSynth:
    def __init__(self):
        self.bank = envBank()

    def addPartial(self, frequency):
        self.bank.addEnv(frequency)
        return len(self.bank.envList)-1

    def getFrequency(self, index):
        return self.bank.envList[index].frequency

    def setFrequency(self, index, frequency):
        self.bank.envList[index].frequency = frequency

    def size(self):
        return len(self.bank.envList)

    def addPoint(self, index, time, amp, skew = 0.5):
        self.bank.envList[index].addEdge(edge(time, amp, skew))

    def adjustGain(self):
        maxGain = 0.0
        eList = self.bank.envList
        for envelope in eList:
            for edge in envelope.edgeList:
                if edge.y > maxGain: maxGain = edge.y
        if maxGain > 0.9:
            adjustAmt = 0.9 / maxGain
            for envelope in eList:
                for edge in envelope.edgeList:
                    edge.y = edge.y*adjustAmt


    def save(self, fileName="EnvSynth.ctl"):
        self.adjustGain()
        f = open(fileName, "w")
        f.write(self.bank.toString())
