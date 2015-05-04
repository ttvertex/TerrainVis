#include "HeightMap.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "glutils.h"
using namespace std;

vec3 lightPos = vec3();

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))


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
	genBuffers();
	lightPos = vec3(3.0f, 3.0f, 3.0f);
	// load shaders
	try {
		phongProg.compileShader("shader/phong.vert");
		phongProg.compileShader("shader/phong.frag");
		phongProg.link();
		phongProg.use();

	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
	phongProg.printActiveAttribs();
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

	genMesh(bits);

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
	///////////////
	// gen index 
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
	////////////////////
	//normals
	for (int i = 0; i < mesh->vertices->size(); i++){
		mesh->normals->push_back(vec3());
	}
	for (int k = 0; k < 10; k++){ // smooth normals
		for (int i = 0; i < mesh->index->size(); i += 3){
			int index[] = { mesh->index->at(i), mesh->index->at(i + 1), mesh->index->at(i + 2) };
			vec3 v1 = mesh->vertices->at(index[0]);
			vec3 v2 = mesh->vertices->at(index[1]);
			vec3 v3 = mesh->vertices->at(index[2]);

			vec3 n = glm::cross(v1 - v2, v1 - v3);
			mesh->normals->at(index[0]) += n;
			mesh->normals->at(index[1]) += n;
			mesh->normals->at(index[2]) += n;
		}
	}
	for (int i = 0; i < mesh->normals->size(); i++){ // normalization
		mesh->normals->at(i) = glm::normalize(mesh->normals->at(i));
	}
	////
}

void HeightMap::genBuffers(){
	//dummy colors
	vector<vec3> colors;
	for (int i = 0; i < mesh->vertices->size(); i++){
		//colors.push_back(vec3(0.4f, (float)i / mesh->vertices->size(), 0.4f));
		//colors.push_back(vec3(0.647059f, 0.164706f, 0.164706f));//brown
		colors.push_back(vec3(1.0f, 0.0f,0.0f));//brown
	}

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices->size() * sizeof(glm::vec3), (GLvoid*)&(*mesh->vertices)[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), (GLvoid*)&colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex colors

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals->size() * sizeof(glm::vec3), (GLvoid*)&(*mesh->normals)[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(2);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index->size() * sizeof(GLuint), (GLvoid*)&(*mesh->index)[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
/*
	Smooth. NOT WORKING YET. MUST INCREMENT WITH row_step & ptr_inc
*/
void HeightMap::smooth(int k, int steps, BYTE* imageData){
	for (int i = 0; i < steps; i++){
		/* Rows, left to right */
		for (int x = 1; x < height; x++){
			for (int z = 0; z < width; z++){
				imageData[mat2vecIndex(x, z, width)] = imageData[mat2vecIndex(x - 1, z, width)]
					* (1 - k) 
					+ imageData[mat2vecIndex(x, z, width)] * k;
			}
		}
		/* Rows, right to left*/
		for (int x = height - 2; x > -1; x--){
			for (int z = 1; z < width; z++){
				imageData[mat2vecIndex(x, z, width)] = imageData[mat2vecIndex(x + 1, z, width)]
					* (1 - k)
					+ imageData[mat2vecIndex(x, z, width)] * k;
			}
		}
		/* Columns, bottom to top */
		for (int x = 0; x < height; x++){
			for (int z = 1; z < width; z++){
				imageData[mat2vecIndex(x, z, width)] = imageData[mat2vecIndex(x, z - 1, width)]
					* (1 - k)
					+ imageData[mat2vecIndex(x, z, width)] * k;
			}
		}

		/* Columns, top to bottom */
		for (int x = 0; x < height; x++){
			for (int z = width - 2; z > -1; z--){
				imageData[mat2vecIndex(x, z, width)] = imageData[mat2vecIndex(x, z + 1, width)]
					* (1 - k)
					+ imageData[mat2vecIndex(x, z, width)] * k;
			}
		}
	}
}

void HeightMap::update(double deltaTime){
	handleInput();
	camera->Update(deltaTime);
	view = camera->Look();
	view *= glm::translate(vec3(0.0f, 0.0f, 3.0f));
	glm::mat4 mv = view * model;
	
	vec4 worldLight = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	// matrices
	phongProg.setUniform("ModelViewMatrix", mv);
	phongProg.setUniform("MVP", projection * mv); //ModelViewProjection
	phongProg.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); // Normal Matrix
	// light info
	phongProg.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f); // Diffuse light intensity
	phongProg.setUniform("Light.Position", view * worldLight); // Light position in eye coords.
	phongProg.setUniform("Light.La", 0.4f, 0.4f, 0.4f);  // Ambient light intensity
	phongProg.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f); // Specular light intensity
	// material info
	phongProg.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f); // Ambient reflectivity
	phongProg.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f); // Diffuse reflectivity
	phongProg.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f); // Specular reflectivity
	phongProg.setUniform("Material.Shininess", 100.0f); // Specular shininess factor
}

void HeightMap::handleInput(){
	
	if (glfwGetKeyOnce(window, 'Q')){
		wireframe = !wireframe;
		if (wireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

}
void HeightMap::resize(int x, int y){
	
}
void HeightMap::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, mesh->index->size(), GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));

}

