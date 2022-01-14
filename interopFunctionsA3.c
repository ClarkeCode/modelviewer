//Name: Robert Clarke
//Student Number: 1120919

#include <stdio.h>
#include "interopFunctionsA3.h"
#define BUFF_SIZE 1024

//Note: returned string must be freed by the caller
char* produceVertexJSArray(struct Float2DArray* vertexData, struct FaceArray* faceData, size_t* vertexCount) {
	size_t capacity = 10;
	char* accumulator = (char*) malloc(sizeof(char)*capacity);
	strcpy(accumulator, "[");
	char buff[BUFF_SIZE];

	for (size_t faceIndex = 0; faceIndex < faceData->size; faceIndex++) {
		struct FaceData* currentFace = &faceData->faces[faceIndex];
		for (size_t x = 0; x < currentFace->_substrings; x++) {
			sprintf(buff, (faceIndex==0 && x==0 ? "%f, %f, %f" : ", %f, %f, %f"), 
				vertexData->_array[currentFace->vertices[x]][0], vertexData->_array[currentFace->vertices[x]][1], vertexData->_array[currentFace->vertices[x]][2] );
			(*vertexCount)++;
			capacity = strlen(accumulator) + strlen(buff) + 1;
			accumulator = realloc(accumulator, capacity);
			strcat(accumulator, buff);
		}
	}

	capacity++;
	accumulator = realloc(accumulator, capacity);
	strcat(accumulator, "]");
	return accumulator;
}

//Note: returned string must be freed by the caller
char* produceNormalJSArray(struct Float2DArray* normalData, struct FaceArray* faceData) {
	size_t capacity = 10;
	char* accumulator = (char*) malloc(sizeof(char)*capacity);
	strcpy(accumulator, "[");
	char buff[BUFF_SIZE];

	for (size_t faceIndex = 0; faceIndex < faceData->size; faceIndex++) {
		struct FaceData* currentFace = &faceData->faces[faceIndex];
		for (size_t x = 0; x < currentFace->_substrings; x++) {
			sprintf(buff, (faceIndex==0 && x==0 ? "%f, %f, %f" : ", %f, %f, %f"), 
				normalData->_array[currentFace->normals[x]][0], normalData->_array[currentFace->normals[x]][1], normalData->_array[currentFace->normals[x]][2] );

			capacity = strlen(accumulator) + strlen(buff) + 1;
			accumulator = realloc(accumulator, capacity);
			strcat(accumulator, buff);
		}
	}

	capacity++;
	accumulator = realloc(accumulator, capacity);
	strcat(accumulator, "]");
	return accumulator;
}

//Note: returned string must be freed by the caller
char* produceTextureJSArray(struct Float2DArray* textureData, struct FaceArray* faceData) {
	size_t capacity = 10;
	char* accumulator = (char*) malloc(sizeof(char)*capacity);
	strcpy(accumulator, "[");
	char buff[BUFF_SIZE];

	for (size_t faceIndex = 0; faceIndex < faceData->size; faceIndex++) {
		struct FaceData* currentFace = &faceData->faces[faceIndex];
		for (size_t x = 0; x < currentFace->_substrings; x++) {
			sprintf(buff, (faceIndex==0 && x==0 ? "%f, %f" : ", %f, %f"), 
				textureData->_array[currentFace->textures[x]][0], textureData->_array[currentFace->textures[x]][1]);

			capacity = strlen(accumulator) + strlen(buff) + 1;
			accumulator = realloc(accumulator, capacity);
			strcat(accumulator, buff);
		}
	}

	capacity++;
	accumulator = realloc(accumulator, capacity);
	strcat(accumulator, "]");
	return accumulator;
}

//Note: returned string must be freed by the caller
//Results are 0-indexed per updated instructions
char* producePointIndexJSArray(struct FaceArray* faceData) {
	size_t capacity = 10;
	char* accumulator = (char*) malloc(sizeof(char)*capacity);
	strcpy(accumulator, "[");
	char buff[BUFF_SIZE];

	size_t pointIndex = 0;

	for (size_t faceIndex = 0; faceIndex < faceData->size; faceIndex++) {
		struct FaceData* currentFace = &faceData->faces[faceIndex];
		for (size_t x = 0; x < currentFace->_substrings; x++) {
			sprintf(buff, (faceIndex==0 && x==0 ? "%lu" : ", %lu"), 
				pointIndex + x);

			capacity = strlen(accumulator) + strlen(buff) + 1;
			accumulator = realloc(accumulator, capacity);
			strcat(accumulator, buff);
		}
		pointIndex += currentFace->_substrings;
	}

	capacity++;
	accumulator = realloc(accumulator, capacity);
	strcat(accumulator, "]");
	return accumulator;
}

size_t producePPMWidth(struct PPMContent* content) { return content->width; }
size_t producePPMHeight(struct PPMContent* content) { return content->height; }

//Note: returned string must be freed by the caller
char* produceRGBAJSArray(struct PPMContent* content) {
	size_t capacity = 10;
	char* accumulator = (char*) malloc(sizeof(char)*capacity);
	strcpy(accumulator, "[");
	char buff[BUFF_SIZE];

	for (size_t height = 0; height < content->height; height++) {
		for (size_t width = 0; width < content->width; width++) {	
			sprintf(buff, (height==0 && width==0 ? "%d, %d, %d, %d" : ", %d, %d, %d, %d"), 
				content->red[width][height], content->green[width][height], content->blue[width][height], 255);
			
			capacity = strlen(accumulator) + strlen(buff) + 1;
			accumulator = realloc(accumulator, capacity);
			strcat(accumulator, buff);
		}
	}

	capacity++;
	accumulator = realloc(accumulator, capacity);
	strcat(accumulator, "]");
	return accumulator;
}


//Using string literal concatenation to make a large format-string
const char* fileFormatText = 
"function getVertexCount() { return %lu; }\n"
"function getdistance() { return -3.0; }\n\n"
"function loadvertices() { return %s; }\n\n"
"function loadnormals() { return %s; }\n\n"
"function loadtextcoords() { return  %s; }\n\n"
"function loadvertexindices() { return %s; }\n\n"
"function loadwidth() { return %lu; }\n"
"function loadheight() { return %lu; }\n"
"function loadtexture() { return( new Uint8Array(%s) ); }\n"
;
#define CFREE(ptr) if(ptr) free(ptr)

#include <sys/stat.h>
//Generate 'loaddata.js' with the specified data in the same directory as the executable
void generateLoadDataJS(struct Float2DArray* vertexData, 
	struct Float2DArray* normalData, 
	struct Float2DArray* textureData, 
	struct FaceArray* faceData, 
	struct PPMContent* ppmContent) {

	size_t vertexCount = 0;
	char* vertices = produceVertexJSArray(vertexData, faceData, &vertexCount);
	char* normals = produceNormalJSArray(normalData, faceData);
	char* textures = produceTextureJSArray(textureData, faceData);
	char* vertexDrawOrder = producePointIndexJSArray(faceData);
	size_t width = producePPMWidth(ppmContent);
	size_t height = producePPMHeight(ppmContent);
	char* rgba = produceRGBAJSArray(ppmContent);

	FILE* pFile = fopen("./loaddata.js", "w+");
	if (pFile)
	fprintf(pFile,
		fileFormatText, vertexCount,
		vertices, normals, textures, vertexDrawOrder,
		width, height,
		rgba);
	
	CFREE(vertices);
	CFREE(normals);
	CFREE(textures);
	CFREE(vertexDrawOrder);
	CFREE(rgba);
	fclose(pFile);
	chmod("./loaddata.js", 438);
}



const char* jsonFormatText = 
	"{\n"
	"	\"vertexCount\": %lu,\n"
	"	\"verticies\": %s,\n"
	"	\"normals\": %s,\n"
	"	\"textcoords\": %s,\n"
	"	\"vertexIndicies\": %s,\n" //Might be unnecessary, can just be computed by going (0 - vertexCount)
	"	\"width\": %lu,\n"
	"	\"height\": %lu,\n"
	"	\"rgba\": %s\n"
	"}\n"
	;

char* generateJSONText(
	struct Float2DArray* vertexData, 
	struct Float2DArray* normalData, 
	struct Float2DArray* textureData, 
	struct FaceArray* faceData, 
	struct PPMContent* ppmContent) {

	size_t vertexCount = 0;
	char* vertices = produceVertexJSArray(vertexData, faceData, &vertexCount);
	char* normals = produceNormalJSArray(normalData, faceData);
	char* textures = produceTextureJSArray(textureData, faceData);
	char* vertexDrawOrder = producePointIndexJSArray(faceData);
	size_t width = producePPMWidth(ppmContent);
	size_t height = producePPMHeight(ppmContent);
	char* rgba = produceRGBAJSArray(ppmContent);

	
	char* output = malloc(strlen(vertices)+strlen(normals)+strlen(textures)+strlen(vertexDrawOrder)+strlen(rgba) + BUFF_SIZE);
	sprintf(output, jsonFormatText,
		vertexCount, vertices, normals, textures, vertexDrawOrder, width, height, rgba);

	CFREE(vertices);
	CFREE(normals);
	CFREE(textures);
	CFREE(vertexDrawOrder);
	CFREE(rgba);

	return output;
}