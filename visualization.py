import matplotlib.pyplot as plt

# filePath = 'source/tmp/fdrBlackbox.csv'

"""
o ficheiro e do tipo csv em que as linhas tem a informacao desta forma: timestamp, speed, thrust
"""

def openFile(filename):
    records = []
    
    dataFile = open(filename, 'r')  # abre o ficheiro
    tmpData = dataFile.readlines()  # le as linhas

    for line in tmpData:
        records.append(line [:-1])  # retira os \n
    print("Records lines:", records)

    return records

def createGraph(record):
    
    timeAxis = []
    speedAxis = []
    thrustAxis = []
    
    for line in record:
        temp = []
        temp = line.split(",")

        timeAxis.append(temp[0])
        speedAxis.append(temp[1])
        thrustAxis.append(temp[2])
    
    print("time: ", timeAxis)
    print("speed: ", speedAxis)
    print("thrust: ", thrustAxis)
    

    plt.subplot(2, 1, 1)
    plt.plot(timeAxis, speedAxis, "o-", label = "Speed Variation")
    plt.ylabel("Thrust (T)")
    
    plt.title("Comparison between Speed and Thrust according to time")

    plt.subplot(2, 1, 2)
    plt.plot(timeAxis, thrustAxis, ".-", label = "Thrust Variation")
    plt.ylabel("Speed (v)")
    plt.xlabel("Time (t)")
    
    plt.show()

filePath = "visualizationTest.csv"
file = openFile(filePath)
createGraph(file)