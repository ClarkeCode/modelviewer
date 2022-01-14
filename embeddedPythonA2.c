//Name: Robert Clarke
//Student Number: 1120919

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include "embeddedPythonA2.h"

int** allocInt2D(size_t width, size_t height) {
	int** arr = calloc(sizeof(int), sizeof(int) * width);
	for (size_t x = 0; x < width; x++) {
		arr[x] = calloc(sizeof(int), sizeof(int) * height);
	}
	return arr;
}
void freeInt2D(int** arr, size_t width) {
	for (size_t x = 0; x < width; x++) {
		if (arr[x]) free(arr[x]);
	}
	if (arr) free(arr);
}

void setupPPMContent(struct PPMContent* content, size_t width, size_t height, size_t depth, int* flatArr, size_t arrSize) {
	content->width  = width;
	content->height = height;
	content->depth  = depth;
	content->red    = allocInt2D(content->width, content->height);
	content->green  = allocInt2D(content->width, content->height);
	content->blue   = allocInt2D(content->width, content->height);

	//Place data values into the appropriate spot in the 2D array according to the width and height
	for (size_t width = 0; width < content->width; width++) {
		for (size_t height = 0; height < content->height; height++) {
			content->red[width][height]   = flatArr[0 + 3*width + (3*content->width)*height];
			content->green[width][height] = flatArr[1 + 3*width + (3*content->width)*height];
			content->blue[width][height]  = flatArr[2 + 3*width + (3*content->width)*height];
		}
	}
}

void terminatePPMContent(struct PPMContent* content) {
	freeInt2D(content->red, content->width);
	freeInt2D(content->green, content->width);
	freeInt2D(content->blue, content->width);
}

//Responsibility for the allocated flatArray passes to caller
void retrievePPMContents(char* partialFileName, size_t* width, size_t* height, size_t* depth, int** flatArray, size_t* flatArraySize, struct ErrorReport* report) {
	//Begin Python interpreter and try to read the desired PPM file
	Py_Initialize();

	PyObject *pName = PyUnicode_DecodeFSDefault("readppm");
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	PyObject *pFunc = PyObject_GetAttrString(pModule, "extractPPM");
	Py_DECREF(pModule);

	PyObject *pString = PyUnicode_FromString(partialFileName);
	PyObject *pArgs = PyTuple_Pack(1, pString);
	Py_DECREF(pString);
	PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
	Py_DECREF(pArgs);
	Py_DECREF(pFunc);
	
	//If a Python Exception was raised, stop and report that an error occurred
	{
		PyObject* pType;
		PyObject* pVal;
		PyObject* pTraceback;

		PyErr_Fetch(&pType, &pVal, &pTraceback);
		if (pType != NULL) {
			char buff[1024] = {0};
			strcpy(buff, partialFileName);
			strcat(buff, ".ppm");
			setErrorFileName(report, buff);
			registerError(report, "Python Exception Raised: Ensure that file exists");

			//Cleanup the Python interpreter
			Py_FinalizeEx();
			return;
		}
	}

	*width  = _PyLong_AsInt(PyTuple_GetItem(pValue, 0));
	*height = _PyLong_AsInt(PyTuple_GetItem(pValue, 1));
	*depth  = _PyLong_AsInt(PyTuple_GetItem(pValue, 2));

	PyObject* pList = PyTuple_GetItem(pValue, 3);

	//Extract the contents of the returned python list
	Py_ssize_clean_t listLength = PyList_Size(pList);
	int* outputList = calloc(sizeof(int), listLength);
	for (Py_ssize_clean_t x = 0; x < listLength; x++) {
		outputList[x] = _PyLong_AsInt(PyList_GetItem(pList, x));
	}

	*flatArray = outputList;
	*flatArraySize = (size_t)listLength;
	Py_DECREF(pValue);
	
	//Finalize the Python interpreter
	Py_FinalizeEx();
}

void extractPPMContents(char* partialFileName, struct PPMContent* content, struct ErrorReport* report) {
	size_t width = 0;
	size_t height = 0;
	size_t depth = 0;
	int* flatArray;
	size_t flatArraySize;

	char fullFileName[1024];
	strcpy(fullFileName, partialFileName);
	strcat(fullFileName, ".ppm");
	setErrorFileName(report, fullFileName);
	if (fopen(fullFileName, "r")) {
		retrievePPMContents(partialFileName, &width, &height, &depth, &flatArray, &flatArraySize, report);
	}
	else {
		registerError(report, "Not a file or does not exist");
	}

	if (!report->hasError) {
		setupPPMContent(content, width, height, depth, flatArray, flatArraySize);
	}
	if (flatArray) free(flatArray);
}

void displayPPMContents(struct PPMContent* content) {
	printf("%lu %lu %lu\n", content->width, content->height, content->depth);
	for (size_t height = 0; height < content->height; height++) {
		for (size_t width = 0; width < content->width; width++) {	
			printf("%3d %3d %3d   ", content->red[width][height], content->green[width][height], content->blue[width][height]);
		}
		printf("\n");
	}
}