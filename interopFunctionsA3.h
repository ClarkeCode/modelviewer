//Name: Robert Clarke
//Student Number: 1120919
#ifndef CUSTOM_FUNCTIONS_A3
#define CUSTOM_FUNCTIONS_A3

#include "customFunctionsA1.h"
#include "embeddedPythonA2.h"

//Generate 'loaddata.js' with the specified data in the same directory as the executable
void generateLoadDataJS(
	struct Float2DArray* vertexData, 
	struct Float2DArray* normalData, 
	struct Float2DArray* textureData, 
	struct FaceArray* faceData, 
	struct PPMContent* ppmContent);

#endif