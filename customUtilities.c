//Name: Robert Clarke
//Student Number: 1120919
#include <stdio.h>
#include "customUtilities.h"

void registerError(struct ErrorReport* report, const char* message) {
	strcpy(report->errorDescription, message);
	report->hasError = 1;
}

void printErrorReport(struct ErrorReport* report) {
	if (report->hasError) {
		printf("Error in '%s' at line #%lu - %s\n", report->fileName, report->lineNumber, report->errorDescription);
	}
}

void setErrorFileName(struct ErrorReport* report, const char* fileName) {
	strcpy(report->fileName, fileName);
}
