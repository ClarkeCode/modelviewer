//Name: Robert Clarke
//Student Number: 1120919
#ifndef CUSTOM_FUNCTIONS_A2
#define CUSTOM_FUNCTIONS_A2
#include <stddef.h>
#include "customUtilities.h"

struct PPMContent {
	size_t width, height, depth;
	int** red;
	int** green;
	int** blue;

};

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

//Frees allocated memory from the struct
void terminatePPMContent(struct PPMContent* content);

//Extracts the contents of the PPM file into the struct using embedded Python
//Note: argument string should omit the '.ppm' extension
void extractPPMContents(char* partialFileName, struct PPMContent* content, struct ErrorReport* report);

//Display formatted PPM file contents to the user
void displayPPMContents(struct PPMContent* content);

#endif