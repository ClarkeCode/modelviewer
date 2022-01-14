//Name: Robert Clarke
//Student Number: 1120919
#include <stdio.h> //printf
#include <string.h> //strcmp
#include <stdbool.h>

#include "customFunctionsA1.h"
#include "embeddedPythonA2.h"
#include "interopFunctionsA3.h"
#include "dbFunctionsA4.h"
#define DISPLAY_OBJ_CONTENT
#define DISPLAY_PPM_CONTENT

int main (int argc, char* argv[]) {
	if (argc <= 1) {
		printf("No files specified, try again. eg: './a1 myfile'\n");
		return EXIT_FAILURE;
	}
	bool jsonOutput = strcmp(argv[1], "--json") == 0;
	if (jsonOutput && argc <= 2) {
		printf("No files specified, try again. eg: './a1 --json myfile'\n");
		return EXIT_FAILURE;
	}
	char* fileName = argv[jsonOutput ? 2 : 1];

#ifdef DISPLAY_OBJ_CONTENT
	struct ErrorReport report = {0};
	char* objectName = NULL;
	char* groupName = NULL;
	struct Float2DArray vertexData;
	struct Float2DArray textureData;
	struct Float2DArray normalData;
	struct FaceArray faceData;

	setupDataAllocations(&vertexData, &textureData, &normalData, &faceData);
	parseOBJContents(fileName, &objectName, &groupName, &vertexData, &textureData, &normalData, &faceData, &report);
	// displayOBJContents(&objectName, &groupName, &vertexData, &textureData, &normalData, &faceData, &report);
#endif


#ifdef DISPLAY_PPM_CONTENT
	struct ErrorReport ppmReport = {0};
	struct PPMContent ppmFile = {0};
	extractPPMContents(fileName, &ppmFile, &ppmReport);
#endif

#if defined(DISPLAY_OBJ_CONTENT) && defined(DISPLAY_PPM_CONTENT)
	if (!jsonOutput)
		generateLoadDataJS(&vertexData, &normalData, &textureData, &faceData, &ppmFile);
	else {
		char* json = generateJSONText(&vertexData, &normalData, &textureData, &faceData, &ppmFile);
		printf("%s", json);
		if (json) free(json);
	}
#endif



	//Cleanup and exit
	
	if (!jsonOutput && report.hasError) {
		printErrorReport(&report);
	}
	terminateDataAllocations(objectName, groupName, &vertexData, &textureData, &normalData, &faceData);


	if (!jsonOutput && ppmReport.hasError) {
		printErrorReport(&ppmReport);
	}
	terminatePPMContent(&ppmFile);

	return (!jsonOutput && (report.hasError || ppmReport.hasError)) ? EXIT_FAILURE : EXIT_SUCCESS;
}