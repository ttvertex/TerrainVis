#include "HeightMap.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "glutils.h"

using namespace std;

HeightMap::HeightMap(GLFWwindow* window, const char* filename){
	this->window = window;
	this->filename = filename;
	this->model = mat4(1.0f);
	this->view = mat4(1.0f);
	this->projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);
}

void HeightMap::initScene(){
	camera = new Camera(window);
	loadImage();
	genMesh(rawImage);
	genBuffers();

	// load shaders
	try {
		prog.compileShader("shader/basic.vert");
		prog.compileShader("shader/basic.frag");
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
	prog.printActiveAttribs();
}

void HeightMap::loadImage(){
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
	mesh->index = new vector<unsigned int>();

	//genMesh(bits);
	//genBuffers();
	rawImage = bits;

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

}

int mat2vecIndex(int r, int c, int nc){
	return (r * nc + c);
}
/*
	Generate the mesh and normals
*/
void HeightMap::genMesh(BYTE* bits){
	float scaleFactorX = 1.0f / height;
	float scaleFactorY = 1.0f / 255.0f;
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
			//mesh->vertices->push_back(glm::vec3(-0.5f + fScaleC, 0.0f, -0.5f + fScaleR));
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
	//for (int i = 0; i < mesh->index->size(); i+=3){
	//	vec3 v1 = mesh->vertices->at(mesh->index->at(i));
	//	vec3 v2 = mesh->vertices->at(mesh->index->at(i + 1));
	//	vec3 v3 = mesh->vertices->at(mesh->index->at(i + 2));
	//	
	//	vec3 n1 = glm::normalize(cross(v1 - v2, v1 - v3));
	//	mesh->normals->push_back(n1);
	//}
	//////

	for (int i = 0; i < mesh->vertices->size(); i++){
		mesh->normals->push_back(vec3());
	}
	
	for (int i = 0; i < mesh->index->size(); i+=3){
		int index[] = { mesh->index->at(i), mesh->index->at(i + 1), mesh->index->at(i + 2)};
		vec3 v1 = mesh->vertices->at(index[0]);
		vec3 v2 = mesh->vertices->at(index[1]);
		vec3 v3 = mesh->vertices->at(index[2]);

		vec3 n = glm::cross(v1 - v2, v1 - v3);
		mesh->normals->at(index[0]) += n;
		mesh->normals->at(index[1]) += n;
		mesh->normals->at(index[2]) += n;
	}
	for (int i = 0; i < mesh->normals->size(); i++){ // normalization
		mesh->normals->at(i) = glm::normalize(mesh->normals->at(i));
	}

	////transform into vertex_t struct
	//vert_t = new vector<vertex_t>();
	//for (int i = 0; i < mesh->vertices->size(); i++){
	//	vec3 v = mesh->vertices->at(i);
	//	vec3 n = mesh->normals->at(i);
	//	vertex_t t = { v, n };
	//	vert_t->push_back(t);
	//}
}

void HeightMap::genBuffers(){
	float side = 1.0f;
	float side2 = side / 2.0f;

	float v[24 * 3] = {
		// Front
		-side2, -side2, side2,
		side2, -side2, side2,
		side2, side2, side2,
		-side2, side2, side2,
		// Right
		side2, -side2, side2,
		side2, -side2, -side2,
		side2, side2, -side2,
		side2, side2, side2,
		// Back
		-side2, -side2, -side2,
		-side2, side2, -side2,
		side2, side2, -side2,
		side2, -side2, -side2,
		// Left
		-side2, -side2, side2,
		-side2, side2, side2,
		-side2, side2, -side2,
		-side2, -side2, -side2,
		// Bottom
		-side2, -side2, side2,
		-side2, -side2, -side2,
		side2, -side2, -side2,
		side2, -side2, side2,
		// Top
		-side2, side2, side2,
		side2, side2, side2,
		side2, side2, -side2,
		-side2, side2, -side2
	};

	float n[24 * 3] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	float tex[24 * 2] = {
		// Front
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Right
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Back
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Left
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Bottom
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Top
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLuint el[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	//glGenVertexArrays(1, &vaoID);
	//glBindVertexArray(vaoID);

	//unsigned int handle[4];
	//glGenBuffers(4, handle);

	//glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), v, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(0);  // Vertex position

	//glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), n, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(1);  // Vertex normal

	//glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), tex, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(2);  // texture coords

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), el, GL_STATIC_DRAW);

	//glBindVertexArray(0);

	//glGenVertexArrays(1, &vaoID);
	//glBindVertexArray(vaoID);

	//unsigned int handle[4];
	//glGenBuffers(3, handle);

	//glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), v, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(0);  // Vertex position

	//glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	//glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), n, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(1);  // Vertex normal

	////glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	////glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), tex, GL_STATIC_DRAW);
	////glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	////glEnableVertexAttribArray(2);  // texture coords

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), el, GL_STATIC_DRAW);

	//glBindVertexArray(0);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices->size() * sizeof(glm::vec3), (GLvoid*)&(*mesh->vertices)[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals->size() * sizeof(glm::vec3), (GLvoid*)&(*mesh->normals)[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index->size() * sizeof(GLuint), (GLvoid*)&(*mesh->index)[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void HeightMap::update(double deltaTime){
	handleInput();
	camera->Update(deltaTime);

	view = camera->Look();
	model = glm::translate(vec3(0.0f, 0.0f, 1.0f)); // push back
	mvpMat = projection * view * model;
	prog.setUniform("ModelViewMatrix", mvpMat);
}

void HeightMap::handleInput(){

}
void HeightMap::resize(int x, int y){
	
}
typedef struct
{
	GLfloat v[3]; //xyz
	GLfloat n[3]; //xyz
}vertex_t;
vector<vertex_t> vertices;
vector<int> indices;
void gen(uint qual){
	vertex_t v;

	double tetaStep = (2 * glm::pi<float>()) / (double)qual;
	double fiiStep = glm::pi<float>() / (double)qual;

	int tetacount = 0;
	int fiicount = 0;

	/// Generates vetrice data
	for (double teta = 0.0; teta <= (2 * glm::pi<float>()); teta += tetaStep)
	{
		tetacount++;
		fiicount = 0;
		for (double fii = 0.0; fii < glm::pi<float>(); fii += fiiStep)
		{
			v.v[0] = cos(teta) * sin(fii);
			v.v[1] = sin(teta) * sin(fii);
			v.v[2] = cos(fii);
			v.n[0] = v.v[0];
			v.n[1] = v.v[1];
			v.n[2] = v.v[2];

			vertices.push_back(v);
			fiicount++;
			if (fiicount >= qual)
				break;
		}
		v.v[0] = cos(teta) * sin(glm::pi<float>());
		v.v[1] = sin(teta) * sin(glm::pi<float>());
		v.v[2] = cos(glm::pi<float>());
		v.n[0] = v.v[0];
		v.n[1] = v.v[1];
		v.n[2] = v.v[2];

		vertices.push_back(v);
		if (tetacount > qual)
			break;
	}

	/// Make indexes
	int qualMaisUm = qual + 1;
	for (int i = 0; i < qual - 1; i++)
	{
		for (int j = 0; j < qualMaisUm - 1; j++)
		{
			indices.push_back(i       * qualMaisUm + j);
			indices.push_back((i + 1) * qualMaisUm + j + 1);
			indices.push_back((i + 1) * qualMaisUm + j);

			indices.push_back(i       * qualMaisUm + j);
			indices.push_back(i       * qualMaisUm + j + 1);
			indices.push_back((i + 1) * qualMaisUm + j + 1);
		}
	}
	for (int i = 0; i < qual; i++)
	{
		indices.push_back((qual - 1)  * qualMaisUm + i);
		indices.push_back(0 + i + 1);
		indices.push_back(0 + i);

		indices.push_back((qual - 1) * qualMaisUm + i);
		indices.push_back((qual - 1) * qualMaisUm + i + 1);
		indices.push_back(0 + i + 1);
	}
}

void HeightMap::render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, mesh->index->size(), GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}

