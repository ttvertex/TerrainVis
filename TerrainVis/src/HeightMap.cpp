#include "HeightMap.h"


HeightMap::HeightMap(const char* filename){
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);

	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		exit(EXIT_FAILURE);

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		exit(EXIT_FAILURE);

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	this->width = FreeImage_GetWidth(dib);
	this->height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (this->width == 0) || (this->height == 0))
		exit(EXIT_FAILURE);
	// How much to increase data pointer to get to next pixel data
	ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;

	mesh = new Mesh();
	mesh->vertices = new vector<vec3>();
	mesh->normals = new vector<vec3>();
	mesh->index = new vector<int>();

	genMesh(bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);	
}

int mat2vecIndex(int r, int c, int nc){
	return (r * nc + c);
}

void HeightMap::genMesh(BYTE* bits){
	float scaleFactorX = 1.0f / height;
	float scaleFactorY =  1.0f / 255.0f;
	float scaleFactorZ = 1.0f / width;
	
	// Length of one row in data
	unsigned int row_step = ptr_inc * width;
	/* --ptr_inc -  by how much we need to increase data pointer to move by one height value in data - 
		point is, that when we have for example 24 - bit image, for now we would only care for R value 
		as the value with intensity, and we need to move 3 bytes from current pointer position to point
		at next value, but if we have 8 -  it image(our case), we need to move by 1 byte only
	  --row_step - the width of one row in memory, it's just number of columns multiplied by ptr_inc */

	// vertices
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			//vec3 v = (vec3(-0.5f, 0.0f, -0.5f) + vec3(i * scaleFactorX, bits[mat2vecIndex((int)i, (int)j, width)] * scaleFactorY, j * scaleFactorZ));
			//vec3 v = (vec3(-0.5f,0.0f,-0.5f) + vec3(i*scaleFactorX, 0.0f, j* scaleFactorZ));
			//mesh->vertices->push_back(v);
			float fScaleC = float(j) / float(width - 1);
			float fScaleR = float(i) / float(height - 1);
			float fVertexHeight = float(*(bits + row_step * i + j * ptr_inc)) / 255.0f;
			mesh->vertices->push_back(glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR));
		}
	}
	
	// gen index - seems to be working
	for (int i = 0; i < height-1; i++){
		for (int j = 0; j < width-1; j++){
			int start = i * width + j;
			////t1
			mesh->index->push_back(mat2vecIndex(i, j, width));
			mesh->index->push_back(mat2vecIndex(i+1, j, width));
			mesh->index->push_back(mat2vecIndex(i+1, j+1, width));
			//t2
			mesh->index->push_back(mat2vecIndex(i+1, j+1, width));
			mesh->index->push_back(mat2vecIndex(i, j+1, width));
			mesh->index->push_back(mat2vecIndex(i, j, width));
		}
	}

	//normals
	for (int i = 0; i < mesh->index->size(); i+=3){
		vec3 v1 = mesh->vertices->at(mesh->index->at(i));
		vec3 v2 = mesh->vertices->at(mesh->index->at(i + 1));
		vec3 v3 = mesh->vertices->at(mesh->index->at(i + 2));
		
		vec3 n1 = glm::normalize(cross(v1 - v2, v1 - v3));
		mesh->normals->push_back(n1);
	}
}

Mesh* HeightMap::getMesh(){
	return mesh;
}