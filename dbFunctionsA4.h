//Name: Robert Clarke
//Student Number: 1120919
#ifndef CUSTOM_FUNCTIONS_A4
#define CUSTOM_FUNCTIONS_A4

#include "customFunctionsA1.h"
#include "embeddedPythonA2.h"

//Returned string must be freed by the caller
char* generateJSONText(
	struct Float2DArray* vertexData, 
	struct Float2DArray* normalData, 
	struct Float2DArray* textureData, 
	struct FaceArray* faceData, 
	struct PPMContent* ppmContent);

#endif