#!/usr/bin/python3
import os

def appendPPMExtension(partialFileName):
	return "{0}.ppm".format(partialFileName)

def ppmIsBinaryFormat(fileName):
	isBinary = False
	file = open(fileName, "rb")
	line = file.readline()
	if (line.find(b"P6") != -1):
		isBinary = True
	file.close()
	return isBinary

def readPPM(fileName, isBinary):
	readMode = "rb" if isBinary else "r"

	output = ""
	with open(fileName, readMode) as file:
		output = file.read()
	return output

def ppmGetDimensions(data, isBinary):
	commentChar = b"#" if isBinary else "#"
	newlineChar = b"\n" if isBinary else "\n"
	informationToRecord = 3
	width = 0
	height = 0
	depth = 0
	# print("'{0}'".format(repr(data[:20])))
	currentItem, data = data.split(newlineChar, maxsplit=1)

	#Extract width, height, and depth numbers from the PPM header
	while informationToRecord > 0:
		#If there is a comment on this line, only consider content to the left of the '#' character
		currentItem = currentItem[:currentItem.find(commentChar) if currentItem.find(commentChar) != -1 else len(currentItem)]
		for subItem in currentItem.split():
			# print("SI: '{0}'".format(subItem))
			if subItem == "P3" or subItem == b"P6":
				continue
			if len(subItem) == 0: #Don't try to convert empty strings
				continue
			elif informationToRecord == 3: #Width
				width = int(subItem)
				informationToRecord -= 1
			elif informationToRecord == 2: #Height
				height = int(subItem)
				informationToRecord -= 1
			elif informationToRecord == 1: #Depth
				depth = int(subItem)
				informationToRecord -= 1
			else: #If all header information is extracted, stop looking
				break
		if informationToRecord == 0:
			break

		currentItem, data = data.split(newlineChar, maxsplit=1)
	# 	print("loop {0}: '{1}'".format(informationToRecord, repr(data[:20])))
	# print("'{0}'".format(repr(data[:20])))

	
	#Discard any comments between the end of the PPM header and the beginning of the data
	hasReachedData = False
	while not hasReachedData:
		firstLine, data = data.split(newlineChar, maxsplit=1)
		if firstLine.strip().find(commentChar) == 0:
			continue
		else:
			data = newlineChar.join([firstLine, data])
			hasReachedData = True
			break

	# print("'{0}'".format(repr(data[:20])))

	return width, height, depth, data

def ppmCleanData(data, isBinary):
	cleanData = []

	if not isBinary:
		cleanData = [int(element) for element in data.split()]
	else:
		# print(data[:20])
		cleanData = list(data)

	return cleanData

def extractPPM(partialFileName):
	file_name = appendPPMExtension(partialFileName)
	if not os.path.exists(file_name):
		raise Exception("Cannot open file '{0}'".format(file_name))

	isBinary = ppmIsBinaryFormat(file_name)

	lines = readPPM(file_name, isBinary)

	width, height, depth, lines = ppmGetDimensions(lines, isBinary)

	data = ppmCleanData(lines, isBinary)
	return (width, height, depth, data)

if __name__ == "__main__":
	import sys
	print(extractPPM(sys.argv[1]))