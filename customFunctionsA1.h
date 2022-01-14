//Name: Robert Clarke
//Student Number: 1120919
#ifndef CUSTOM_FUNCTIONS_A1
#define CUSTOM_FUNCTIONS_A1

#include <string.h> //size_t, strcpy, etc
#include <stdio.h> //FILE, printf, sprintf
#include <stdlib.h> //strtof, malloc, etc
#include "customUtilities.h"


// ______      _     _ _        _____ _                   _       
// | ___ \    | |   | (_)      /  ___| |                 | |      
// | |_/ /   _| |__ | |_  ___  \ `--.| |_ _ __ _   _  ___| |_ ___ 
// |  __/ | | | '_ \| | |/ __|  `--. \ __| '__| | | |/ __| __/ __|
// | |  | |_| | |_) | | | (__  /\__/ / |_| |  | |_| | (__| |_\__ |
// \_|   \__,_|_.__/|_|_|\___| \____/ \__|_|   \__,_|\___|\__|___/
//
//
//                        _   _____                          
//                       | | |  ___|                         
//         __ _ _ __   __| | | |__ _ __  _   _ _ __ ___  ___ 
//        / _` | '_ \ / _` | |  __| '_ \| | | | '_ ` _ \/ __|
//       | (_| | | | | (_| | | |__| | | | |_| | | | | | \__ |
//        \__,_|_| |_|\__,_| \____/_| |_|\__,_|_| |_| |_|___/
//
// ==============================================================

//A 1-indexed array
struct Float2DArray {
	size_t capacity;
	size_t size;
	size_t depth;
	float** _array;
};

//Unset, OnlyVertex, WithTexture, WithNormal, BothTextureAndNormal, FaceInvalid
enum FaceStates { Unset, OnlyVertex, WithTexture, WithNormal, BothTextureAndNormal, FaceInvalid };

struct FaceData {
	//Either 3 or 4
	size_t _substrings;
	enum FaceStates state;
	int vertices[4];
	int textures[4];
	int normals[4];
};

//A 0-indexed array
struct FaceArray {
	size_t capacity;
	size_t size;
	struct FaceData* faces;
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

//Display formatted face data to the user
void printAllFaceInformation(struct FaceArray* faceData, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData) ;

//Perform required allocations and setup operations
void setupDataAllocations(struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData);

//Deallocate any previously allocated memory
void terminateDataAllocations(char* objectName, char* groupName,  struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData);

//Parse specified .obj file
//Note: argument string should omit the '.obj' extension
int parseOBJContents(char* partialFileName, char** objectName, char** groupName, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData,
	struct ErrorReport* errReport);

//Display formatted OBJ file contents to the user
void displayOBJContents(char** objectName, char** groupName, struct Float2DArray* vertexData, struct Float2DArray* textureData, struct Float2DArray* normalData, struct FaceArray* faceData,
	struct ErrorReport* errReport);
#endif