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
	cout << "(" << width << "," << height << ")" << endl;
	float scaleFactor = 1.0f;
	
	// vertices
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			vec3 v = vec3(i, bits[mat2vecIndex((int)i, (int)j, width)], j) * scaleFactor;
			mesh->vertices->push_back(v);
		}
	}

	// gen index
	for (int i = 0; i < height-1; i++){
		for (int j = 0; j < width-1; j++){
			//t1
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
	cout << "indexsize" << mesh->index->size() << endl;
	cout << "vsize" << mesh->vertices->size() << endl;
	for (int i = 0; i < mesh->index->size(); i+=3){
		vec3 v1 = mesh->vertices->at(mesh->index->at(i));
		vec3 v2 = mesh->vertices->at(mesh->index->at(i + 1));
		vec3 v3 = mesh->vertices->at(mesh->index->at(i + 2));
		
		vec3 n1 = glm::normalize(cross(v1 - v2, v1 - v3));
		mesh->normals->push_back(n1);
	}
	//for (int i = 0; i < height; i++){
	//	for (int j = 0; j < width; j++){
	//		// square
	//		vec3 v00(0, bits[mat2vecIndex(i, j, width)], 0); // x, height, z
	//		vec3 v10(0, bits[mat2vecIndex(i + 1, j, width)], 0); // x, height, z
	//		vec3 v11(0, bits[mat2vecIndex(i + 1, j + 1, width)], 0); // x, height, z
	//		vec3 v01(0, bits[mat2vecIndex(i, j + 1, width)], 0); // x, height, z
	//		
	//		// normal per triangle face
	//		vec3 n1 = glm::normalize(cross(v00 - v10, v00 - v11));
	//		vec3 n2 = glm::normalize(cross(v01 - v00, v01 - v11));

	//		mesh->normals->push_back(n1);
	//		mesh->normals->push_back(n2);
	//	}
	//}

	//for (int i = 0; i < width - 1; i++){
	//	for (int j = 0; j < height - 1; j++){
	//		// four corners' vertex
	//		//t1
	//		//vec3 v00t1(i, bits[mat2vecIndex(i, j, width)] / 255.0f * scale, j); // x, height, z
	//		//vec3 v10t1(i + 1, bits[mat2vecIndex(i + 1, j, width)] / 255.0f* scale, j); // x, height, z
	//		//vec3 v11t1(i + 1, bits[mat2vecIndex(i + 1, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
	//		////t2
	//		//vec3 v11t2(i + 1, bits[mat2vecIndex(i + 1, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
	//		//vec3 v01t2(i, bits[mat2vecIndex(i, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
	//		//vec3 v00t2(i, bits[mat2vecIndex(i, j, width)] / 255.0f* scale, j); // x, height, z
	//		float fi = (float)i;
	//		float fj = (float)j;
	//		//t1
	//		vec3 v00t1 = vec3(fi, 0, fj) * scale; // x, height, z
	//		vec3 v10t1 = vec3(fi + 1, 0, fj) * scale; // x, height, z
	//		vec3 v11t1 = vec3(fi + 1, 0, fj + 1) * scale; // x, height, z
	//		//t2
	//		vec3 v11t2 = vec3(fi, 0, fj + 1) * scale; // x, height, z
	//		vec3 v01t2 = vec3(fi, 0, fj + 1) * scale; // x, height, z
	//		vec3 v00t2 = vec3(fi, 0, fj) * scale; // x, height, z


	//		vec3 n1 = glm::normalize(cross(v00t1 - v10t1, v00t1 - v11t1));
	//		vec3 n2 = glm::normalize(cross(v01t2 - v00t2, v01t2 - v11t2));
	//		//t1
	//		mesh->vertices->push_back(v00t1);
	//		mesh->vertices->push_back(v10t1);
	//		mesh->vertices->push_back(v11t1);
	//		//t2
	//		mesh->vertices->push_back(v11t2);
	//		mesh->vertices->push_back(v01t2);
	//		mesh->vertices->push_back(v00t2);

	//		mesh->normals->push_back(n1);
	//		mesh->normals->push_back(n2);
	//	}
	//}
}

Mesh* HeightMap::getMesh(){
	return mesh;
}