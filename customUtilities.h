//Name: Robert Clarke
//Student Number: 1120919
#ifndef CUSTOM_UTILITIES
#define CUSTOM_UTILITIES
#include <string.h>

struct ErrorReport {
	int hasError;
	char errorDescription[1024];
	char fileName[1024];
	size_t lineNumber;
};

//Register that an error has occurred and store the provided error message
void registerError(struct ErrorReport* report, const char* message);

//Display a detected error to the user
void printErrorReport(struct ErrorReport* report);

//Set the name of the report's file
void setErrorFileName(struct ErrorReport* report, const char* fileName);
#endif