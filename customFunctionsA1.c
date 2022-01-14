//Name: Robert Clarke
//Student Number: 1120919
#include "customFunctionsA1.h"

#define PARSE_BUFF_SIZE 1024
#define PARSE_WHITESPACE " \t"
#define PARSE_WHITESPACE_AND_SLASH " \t/"
#define DATA_STRUCTURE_DEFAULT_CAPACITY 1024


// ______        _           _____  _                       _                     
// |  _  \      | |         /  ___|| |                     | |                    
// | | | | __ _ | |_  __ _  \ `--. | |_  _ __  _   _   ___ | |_  _   _  _ __  ___ 
// | | | |/ _` || __|/ _` |  `--. \| __|| '__|| | | | / __|| __|| | | || '__|/ _ |
// | |/ /| (_| || |_| (_| | /\__/ /| |_ | |   | |_| || (__ | |_ | |_| || |  |  __/
// |___/  \__,_| \__|\__,_| \____/  \__||_|    \__,_| \___| \__| \__,_||_|   \___|
//
//
//               ______                    _    _                    
//               |  ___|                  | |  (_)                   
//               | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
//               |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//               | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ |
//               \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
// 
// ===============================================================================


//START: Float2D conveniences
float** allocFloat2D(size_t length, size_t depth) {
	float** arr = malloc(sizeof(float*) * length);
	for (size_t item = 0; item < length; item++) {
		arr[item] = malloc(sizeof(float*) * depth);
	}
	return arr;
}

void freeFloat2D(float** arr, size_t length) {
	for (size_t item = 0; item < length; item++) {
		free(arr[item]);
	}
	free(arr);
}

float** reallocFloat2D(float** pointer, size_t oldLen, size_t newLen, size_t depth) {
	float** newArr = allocFloat2D(newLen, depth);
	for (size_t item = 0; item < oldLen; item++) {
		for (size_t d = 0; d < depth; d++)
			newArr[item][d] = pointer[item][d];
	}
	freeFloat2D(pointer, oldLen);
	return newArr;
}

void printFloat2D(float** arr, size_t length, size_t depth, char* linePrefix, int skipFirstItem) {
	for (size_t l = (skipFirstItem ? 1 : 0); l < length; l++) {
		printf(linePrefix);
		for (size_t d = 0; d < depth; d++) {
			printf("%f ", arr[l][d]);
		}
		printf("\n");
	}
}
//END: Float2D conveniences

//START: Enhanced Float2D arrays
void setupFloat2D(struct Float2DArray* arr, size_t depth) {
	arr->capacity = DATA_STRUCTURE_DEFAULT_CAPACITY;
	arr->size = 0;
	arr->depth = depth;
	arr->_array = allocFloat2D(arr->capacity, arr->depth);
}

void conditionalResizeFloat2D(struct Float2DArray* arr) {
	if (arr->capacity == arr->size) {
		arr->_array = reallocFloat2D(arr->_array, arr->capacity, arr->capacity*2, arr->depth);
		arr->capacity *= 2;
	}
}

void terminateFloat2D(struct Float2DArray* arr) {
	freeFloat2D(arr->_array, arr->capacity);
}

void printFloat2DArray(struct Float2DArray* arr, char* prefix) {
	printFloat2D(arr->_array, arr->size+1, arr->depth, prefix, 1);
}

void insert2Float2D(struct Float2DArray* arr, float x, float y) {
	arr->size++;
	conditionalResizeFloat2D(arr);
	arr->_array[arr->size][0] = x;
	arr->_array[arr->size][1] = y;
}

void insert3Float2D(struct Float2DArray* arr, float x, float y, float z) {
	arr->size++;
	conditionalResizeFloat2D(arr);
	arr->_array[arr->size][0] = x;
	arr->_array[arr->size][1] = y;
	arr->_array[arr->size][2] = z;
}
//END: Enhanced Float2D arrays



//START: faces
void displayFaceData(struct FaceData* data, size_t faceNumber) {
	printf("f%lu v  ", faceNumber);
	for (size_t v=0; v < data->_substrings; v++) printf("%d ", data->vertices[v]);
	printf("\n");
	if (data->state == WithTexture || data->state == BothTextureAndNormal) {
		printf("f%lu vt ", faceNumber);
		for (size_t t=0; t<data->_substrings; t++) printf("%d ", data->textures[t]);
		printf("\n");
	}
	if (data->state == WithNormal || data->state == BothTextureAndNormal) {
		printf("f%lu vn ", faceNumber);
		for (size_t n=0; n<data->_substrings; n++) printf("%d ", data->normals[n]);
		printf("\n");
	}
}

void setupFaceArray(struct FaceArray* arr) {
	arr->capacity = DATA_STRUCTURE_DEFAULT_CAPACITY;
	arr->size = 0;
	arr->faces = malloc(sizeof(struct FaceData) * arr->capacity);
}

void terminateFaceArray(struct FaceArray* arr) {
	free(arr->faces);
}

void conditionalResizeFaceArray(struct FaceArray* arr) {
	if (arr->capacity == (arr->size+1)) {
		arr->capacity *= 2;
		arr->faces = realloc(arr->faces, sizeof(struct FaceData) * arr->capacity);
	}
}
//END: faces












//          _   _        _                    
//         | | | |      | |                   
//         | |_| |  ___ | | _ __    ___  _ __ 
//         |  _  | / _ \| || '_ \  / _ \| '__|
//         | | | ||  __/| || |_) ||  __/| |   
//         \_| |_/ \___||_|| .__/  \___||_|   
//                         | |                
//                         |_|                
// ______                    _    _                    
// |  ___|                  | |  (_)                   
// | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
// |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
// | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ |
// \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
// ====================================================

//Determines if the provided character is a space or a tab
int isWhitespace(const char c) {
	return c == ' ' || c == '\t';
}

//Gives the amount of whitespace-separated tokens on the given string
//Requirement: The string must not have leading or trailing whitespace
size_t numberOfTokens(const char* line) {
	if (line == NULL)
		return 0;

	int hasContent = !isWhitespace(line[0]);
	size_t tokens = 0;
	for (size_t x = 1; x < strlen(line); x++) {
		if (isWhitespace(line[x]) && !isWhitespace(line[x-1])) {
			tokens++;
		}
		else if (!hasContent && !isWhitespace(line[x])) {
			hasContent = !hasContent;
		}
	}
	return tokens + ((hasContent) ? 1 : 0);
}

//Helper function for handleFace, determines if the input string is in the proper format
enum FaceStates getFaceState(const char* substring, struct ErrorReport* errReport) {
	char buff[PARSE_BUFF_SIZE];
	enum FaceStates val = Unset;
	
	//If there are not the correct amount of items in the line, it is invalid
	size_t tokens = numberOfTokens(substring);
	if (!(tokens == 4 || tokens == 5)) {
		sprintf(buff, "Improper grammar '%s': must have 3 or 4 sets of data indices", substring);
		registerError(errReport, buff);
		return FaceInvalid;
	}

	if (strstr(substring, "/") == NULL)
		return OnlyVertex;

	//Check to see if the string contains single or double slashes '/' or '//', lines with mixtures of both are invalid
	size_t dividers = 0;
	for (size_t x = 0; x < strlen(substring); x++) {
		if (substring[x]=='/' && substring[x+1]=='/') {
			if (val != Unset && val != WithNormal) {
				sprintf(buff, "Improper grammar '%s': index dividers '/' or '//' must be consistant throughout the line", substring);
				registerError(errReport, buff);
				return FaceInvalid;
			}
			x++;
			dividers++;
			val = WithNormal;
		}
		else if (substring[x]=='/') {
			if (val != Unset && val != WithTexture) {
				sprintf(buff, "Improper grammar '%s': index dividers '/' or '//' must be consistant throughout the line", substring);
				registerError(errReport, buff);
				return FaceInvalid;
			}
			dividers++;
			val = WithTexture;
		}
	}

	//If the string contains single slashes '/', check to see if it also includes normal data
	if (val == WithTexture) {
		int lookForPair = 0;
		for (size_t x = 0; x < strlen(substring); x++) {
			if (substring[x]=='/' && !lookForPair) {
				lookForPair = 1;
			}
			//If there are more than 2 slashes before encountering whitespace, it is invalid
			else if (substring[x]=='/' && lookForPair==2) {
				val = FaceInvalid;
				sprintf(buff, "Wrong number of dividers '%s': maximum 2 per set of indices", substring);
				registerError(errReport, buff);
				break;
			}
			else if (substring[x]=='/' && lookForPair) {
				lookForPair = 2;
			}
			else if (isWhitespace(substring[x])) {
				if (lookForPair == 2) {
					val = BothTextureAndNormal;
				}
				//If indices have only one slash when previously determined to have texture and normal data, it is invalid
				else if (lookForPair < 2 && val == BothTextureAndNormal) {
					val = FaceInvalid;
					sprintf(buff, "Wrong number of dividers '%s': faces with texture and normal data must have 2 dividers in each set of indices", substring);
					registerError(errReport, buff);
					break;
				}
				lookForPair = 0;
			}
		}
	}

	//If there are the wrong number of divisions for the line format, it is invalid
	if (val == OnlyVertex && dividers != 0) {
		sprintf(buff, "Wrong number of dividers '%s': %lu, expected 0", substring, dividers);
		registerError(errReport, buff);
		return FaceInvalid;
	}
	else if ((val == WithNormal || val == WithTexture) && !(dividers == 3 || dividers == 4)) {
		sprintf(buff, "Wrong number of dividers '%s': %lu, expected 3 or 4", substring, dividers);
		registerError(errReport, buff);
		return FaceInvalid;
	}
	else if ((val == WithNormal || val == WithTexture) && dividers != tokens-1) {
		sprintf(buff, "Extra divider found '%s', did you miss a space?", substring);
		registerError(errReport, buff);
		return FaceInvalid;
	}
	else if (val == BothTextureAndNormal && !((tokens == 4 && dividers == 6) || (tokens == 5 && dividers == 8))) {
		sprintf(buff, "Wrong number of dividers '%s': %lu, expected 6 or 8", substring, dividers);
		registerError(errReport, buff);
		return FaceInvalid;
	}
	
	return val;
}

//Helper function for handleObjectName and handleGroupName, reponsibility for deallocating returned string passes to calling function
char* duplicateStr(const char* input) {
	if (input == NULL) return NULL;
	char* duplicate = malloc(strlen(input)+1);
	if (duplicate) {
		strcpy(duplicate, input);
	}
	return duplicate;
}

//Helper function for printAllFaceInformation
void get3DepthArrayOutput(struct Float2DArray* array, size_t itemIndex, char* output) {
	sprintf(output, "%10f %10f %10f ", array->_array[itemIndex][0], array->_array[itemIndex][1], array->_array[itemIndex][2]);
}

//Helper function for printAllFaceInformation
void get2DepthArrayOutput(struct Float2DArray* textureData, size_t itemIndex, char* output) {
	sprintf(output, "%10f %10f ", textureData->_array[itemIndex][0], textureData->_array[itemIndex][1]);
}

//START: Numeric conversions
float simpleNumericConversion(char* numericString, struct ErrorReport* report) {
	if (numericString == NULL) return 0.0;

	char* endptr;
	float result = strtof(numericString, &endptr);
	if (report != NULL && numericString == endptr) {
		char buff[PARSE_BUFF_SIZE];
		sprintf(buff, "Bad conversion target '%s': expected an floating-point number", numericString);
		registerError(report, buff);
	}
	return result;
}

unsigned long simpleIntegerConversion(char* numericString, struct ErrorReport* report) {
	if (numericString == NULL) return 0;

	char* endptr;
	unsigned long result = strtoul(numericString, &endptr, 10);
	if (report != NULL && numericString == endptr) {
		char buff[PARSE_BUFF_SIZE];
		sprintf(buff, "Bad conversion target '%s': expected an integer", numericString);
		registerError(report, buff);
	}
	return result;
}
//END: Numeric conversions

//START: dispatch handlers
void handleObjectName(char* line, char** output, struct ErrorReport* errReport) {
	char buff[PARSE_BUFF_SIZE];
	if (output && *output != NULL) {
		sprintf(buff, "Multiple declarations of object name: '%s'", line);
		registerError(errReport, buff);
	}
	else if (numberOfTokens(line) != 2) {
		sprintf(buff, "Wrong number of items for the object name: '%s'", line);
		registerError(errReport, buff);
	}
	strtok(line, PARSE_WHITESPACE);
	*output=duplicateStr(strtok(NULL, PARSE_WHITESPACE));
}

void handleGroupName(char* line, char** output, struct ErrorReport* errReport) {
	char buff[PARSE_BUFF_SIZE];
	if (output && *output != NULL) {
		sprintf(buff, "Multiple declarations of group name: '%s'", line);
		registerError(errReport, buff);
	}
	else if (numberOfTokens(line) != 2) {
		sprintf(buff, "Wrong number of items for the object group: '%s'", line);
		registerError(errReport, buff);
	}
	strtok(line, PARSE_WHITESPACE);
	*output=duplicateStr(strtok(NULL, PARSE_WHITESPACE));
}

//Insert 3 float values into a Float2DArray struct, ie: Vertex and Normal data
void handleDepth3(char* line, struct Float2DArray* arr, struct ErrorReport* errReport) {
	char buff[PARSE_BUFF_SIZE];
	if (numberOfTokens(line) != 4) {
		sprintf(buff, "Improper grammar '%s': expected 3 numbers", line);
		registerError(errReport, buff);
	}
	
	strtok(line, PARSE_WHITESPACE);
	char* first = strtok(NULL, PARSE_WHITESPACE);
	char* second = strtok(NULL, PARSE_WHITESPACE);
	char* third = strtok(NULL, PARSE_WHITESPACE);
	float x = simpleNumericConversion(first, errReport);
	float y = simpleNumericConversion(second, errReport);
	float z = simpleNumericConversion(third, errReport);
	insert3Float2D(arr, x, y, z);
}

//Insert 2 float values into a Float2DArray struct, ie: Texture data
void handleDepth2(char* line, struct Float2DArray* arr, struct ErrorReport* errReport) {
	char buff[PARSE_BUFF_SIZE];
	if (numberOfTokens(line) != 3) {
		sprintf(buff, "Improper grammar '%s': expected 2 numbers", line);
		registerError(errReport, buff);
	}

	strtok(line, PARSE_WHITESPACE);
	char* first = strtok(NULL, PARSE_WHITESPACE);
	char* second = strtok(NULL, PARSE_WHITESPACE);
	float x = simpleNumericConversion(first, errReport);
	float y = simpleNumericConversion(second, errReport);
	insert2Float2D(arr, x, y);
}

//Process a line of face data and insert it into a FaceArray struct
void handleFace(char* line, struct FaceArray* arr, struct ErrorReport* errReport, size_t* vertexSize, size_t* textureSize, size_t* normalSize) {
	struct FaceData temp = {0};
	temp.state = getFaceState(line, errReport);
	temp._substrings = numberOfTokens(line);
	if (temp._substrings > 0) { temp._substrings -= 1; }

	if (temp.state == FaceInvalid) {
		//The error was registered in the "getFaceState" function
		return;
	}

	//Create a copy of the line for use if an error is detected
	char lineCopy[PARSE_BUFF_SIZE];
	strcpy(lineCopy, line);
	char buff[PARSE_BUFF_SIZE*2];

	strtok(line, PARSE_WHITESPACE_AND_SLASH);
	for (size_t x = 0; x < temp._substrings; x++) {
		temp.vertices[x] = simpleIntegerConversion(strtok(NULL, PARSE_WHITESPACE_AND_SLASH), errReport);
		if (!errReport->hasError && temp.vertices[x] > *vertexSize) {
			sprintf(buff, "Line '%s' has an invalid vertex index of %d, there are only %lu verticies in the file", lineCopy, temp.vertices[x], *vertexSize);
			registerError(errReport, buff);
			return;
		}

		if (temp.state == WithTexture || temp.state == BothTextureAndNormal) {
			temp.textures[x] = simpleIntegerConversion(strtok(NULL, PARSE_WHITESPACE_AND_SLASH), errReport);
			if (!errReport->hasError && temp.textures[x] > *textureSize) {
				sprintf(buff, "'%s' has an invalid texture index of '%d', there are only %lu textures in the file", lineCopy, temp.textures[x], *textureSize);
				registerError(errReport, buff);
				return;
			}
		}

		if (temp.state == WithNormal || temp.state == BothTextureAndNormal) {
			temp.normals[x] = simpleIntegerConversion(strtok(NULL, PARSE_WHITESPACE_AND_SLASH), errReport);
			if (!errReport->hasError && temp.normals[x] > *normalSize) {
				sprintf(buff, "'%s' has an invalid normal index of '%d', there are only %lu normals in the file", lineCopy, temp.normals[x], *normalSize);
				registerError(errReport, buff);
				return;
			}
		}

		if (errReport->hasError) {
			return;
		}
	}

	conditionalResizeFaceArray(arr);
	arr->faces[arr->size] = temp;
	arr->size++;
}
//END: dispatch handlers


//o g v vt vn f #
enum ParserToken { ObjectName, GroupName, Vertex, Texture, Normal, Face, Comment, Other };

//Checks the first characters to return correct enum value, assumes no leading whitespace
enum ParserToken determineLineToken(const char* line) {
	if (line == NULL) return Other;
	
	char* locationO = strstr(line, "o");
	char* locationG = strstr(line, "g");
	char* locationV = strstr(line, "v");
	char* locationVt = strstr(line, "vt");
	char* locationVn = strstr(line, "vn");
	char* locationF = strstr(line, "f");
	char* locationNum = strstr(line, "#");

	if      (locationO != NULL && (line-locationO) == 0)     return ObjectName;
	else if (locationG != NULL && (line-locationG == 0))     return GroupName;
	else if (locationVt != NULL && (line-locationVt == 0))   return Texture;
	else if (locationVn != NULL && (line-locationVn == 0))   return Normal;
	else if (locationV != NULL && (line-locationV == 0))     return Vertex;
	else if (locationF != NULL && (line-locationF == 0))     return Face;
	else if (locationNum != NULL && (line-locationNum == 0)) return Comment;
	else return Other;
}

//Dispatches a given string of file input to it's appropriate handler function
void dispatchLine(char* line, char** objectName, char** groupName, 
	struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData, struct ErrorReport* errReport) {
	enum ParserToken lineCategory = determineLineToken(line);

	switch (lineCategory) {
		case ObjectName: handleObjectName(line, objectName, errReport); break;
		case GroupName: handleGroupName(line, groupName, errReport); break;
		case Vertex: handleDepth3(line, vertexData, errReport); break;
		case Texture: handleDepth2(line, textureData, errReport); break;
		case Normal: handleDepth3(line, normalData, errReport); break;
		case Face: handleFace(line, faceData, errReport, &vertexData->size, &textureData->size, &normalData->size); break;
		
		default: break;
	}
}















//           ______        _      _  _       
//           | ___ \      | |    | |(_)      
//           | |_/ /_   _ | |__  | | _   ___ 
//           |  __/| | | || '_ \ | || | / __|
//           | |   | |_| || |_) || || || (__ 
//           \_|    \__,_||_.__/ |_||_| \___|
//
//
// ______                    _    _                    
// |  ___|                  | |  (_)                   
// | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___ 
// |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
// | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ |
// \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
//
// ====================================================

void parseOBJFile (FILE* openFile, char* buff,
	char** objectName, char** groupName, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData,
	struct ErrorReport* errReport) {
	char currentLetter = '\0';

	//Fill the buffer with the current line of file input, ignoring leading whitespace
	do {
		if (strlen(buff)==0 && isWhitespace(currentLetter))
			currentLetter = '\0';
		buff[strlen(buff)] = currentLetter;
		currentLetter = getc(openFile);
	} while (currentLetter != EOF && currentLetter != '\n');

	//Strip trailing whitespace
	while (isWhitespace(buff[strlen(buff)-1])) {
		buff[strlen(buff)-1] = '\0';
	}

	//Send the buffer to the appropriate handler function
	errReport->lineNumber++;
	dispatchLine(buff, objectName, groupName, vertexData, textureData, normalData, faceData, errReport);
	
	//Zero out the buffer for the next recursive call
	for (size_t x = 0; x < PARSE_BUFF_SIZE-1; x++) {
		buff[x] = '\0';
	}

	//Continue recursion until the end of the file has been reached or an error has been detected
	if (currentLetter != EOF && !errReport->hasError) {
		parseOBJFile(openFile, buff, objectName, groupName, vertexData, textureData, normalData, faceData, errReport);
	}
}

void printAllFaceInformation(struct FaceArray* faceData, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData) {
	char buff[PARSE_BUFF_SIZE];

	for (size_t faceIndex = 0; faceIndex < faceData->size; faceIndex++) {
		struct FaceData* currentFace = faceData->faces+faceIndex;
		printf("f%lu v  ", faceIndex+1);
		for (size_t x = 0; x < currentFace->_substrings; x++) {
			get3DepthArrayOutput(vertexData, currentFace->vertices[x], buff);
			printf("%s%5s", buff, "");
		}
		printf("\n");

		if (currentFace->state == WithTexture || currentFace->state == BothTextureAndNormal) {
			printf("f%lu vt ", faceIndex+1);
			for (size_t x = 0; x < currentFace->_substrings; x++) {
				get2DepthArrayOutput(textureData, currentFace->textures[x], buff);
				printf("%s%16s", buff, "");
			}
			printf("\n");
		}
		
		if (currentFace->state == WithNormal || currentFace->state == BothTextureAndNormal) {
			printf("f%lu vn ", faceIndex+1);
			for (size_t x = 0; x < currentFace->_substrings; x++) {
				get3DepthArrayOutput(normalData, currentFace->normals[x], buff);
				printf("%s%5s", buff, "");
			}
			printf("\n");
		}
	}
}

void setupDataAllocations(struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData) {
	setupFloat2D(vertexData, 3);
	setupFloat2D(textureData, 2);
	setupFloat2D(normalData, 3);
	setupFaceArray(faceData);
}

void terminateDataAllocations(char* objectName, char* groupName,  struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData) {
	if (objectName) free(objectName);
	if (groupName) free(groupName);
	terminateFloat2D(vertexData);
	terminateFloat2D(textureData);
	terminateFloat2D(normalData);
	terminateFaceArray(faceData);
}

#include <stdio.h>
int parseOBJContents(char* partialFileName, char** objectName, char** groupName, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData,
	struct ErrorReport* errReport) {

	//Append .obj to the partial file name
	char fullFileName[PARSE_BUFF_SIZE];
	strcpy(fullFileName, partialFileName);
	strcat(fullFileName, ".obj");

	//Open the specified file and setup for diagnostics
	setErrorFileName(errReport, fullFileName);
	FILE* openFile = fopen(fullFileName, "r");
	char* fileExtension = strrchr(fullFileName, '.');

	//Check if the file exists and is a .obj
	if (openFile == NULL) {
		registerError(errReport, "Not a file or does not exist");
		return EXIT_FAILURE;
	}
	else if (!fileExtension || (fileExtension && strcmp(fileExtension, ".obj") != 0)) {
		registerError(errReport, "File must be in the .obj format");
		fclose(openFile);
		return EXIT_FAILURE;
	}
	
	//Begin parsing the file
	char buff[1024] = "";
	parseOBJFile(openFile, buff, objectName, groupName, vertexData, textureData, normalData, faceData, errReport);
	fclose(openFile);
	
	return errReport->hasError ? EXIT_FAILURE : EXIT_SUCCESS;
}



void displayOBJContents(char** objectName, char** groupName, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData,
	struct ErrorReport* errReport) {
	
	//Display parsed information to the user
	if (objectName != NULL) {
		printf("Object name: '%s'\n", *objectName);
	}
	if (groupName != NULL) {
		printf("Group name:  '%s'\n", *groupName);
	}
	printAllFaceInformation(faceData, vertexData, textureData, normalData);
}