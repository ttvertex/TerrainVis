#include "VBOSphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "glutils.h"

VBOSphere::VBOSphere(int quality){
	this->quality = quality;	
}

void VBOSphere::initScene(){
	genVertex();
	genBuffers();

}

void VBOSphere::update(double deltaTime){

}

void VBOSphere::render(){
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);

}

void VBOSphere::resize(int x, int y){

}

void VBOSphere::genVertex(){
	// i've left the vertex_t struct just to keep things simple..
	vertex_t v;

	double tetaStep = (2 * glm::pi<float>()) / (double)quality;
	double fiiStep = glm::pi<float>() / (double)quality;

	int tetacount = 0;
	int fiicount = 0;
	int i = 0;
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

			//vertices.push_back(v);
			vertices.push_back(v.v[0]);
			vertices.push_back(v.v[1]);
			vertices.push_back(v.v[2]);

			normals.push_back(v.n[0]);
			normals.push_back(v.n[1]);
			normals.push_back(v.n[2]);

			fiicount++;
			if (fiicount >= quality)
				break;
		}
		v.v[0] = cos(teta) * sin(glm::pi<float>());
		v.v[1] = sin(teta) * sin(glm::pi<float>());
		v.v[2] = cos(glm::pi<float>());
		v.n[0] = v.v[0];
		v.n[1] = v.v[1];
		v.n[2] = v.v[2];


		//vertices.push_back(v);
		vertices.push_back(v.v[0]);
		vertices.push_back(v.v[1]);
		vertices.push_back(v.v[2]);

		normals.push_back(v.n[0]);
		normals.push_back(v.n[1]);
		normals.push_back(v.n[2]);

		if (tetacount > quality)
			break;
	}

	/// Make indexes
	int qualMaisUm = quality + 1;
	for (int i = 0; i < quality - 1; i++)
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
	for (int i = 0; i < quality; i++)
	{
		indices.push_back((quality - 1)  * qualMaisUm + i);
		indices.push_back(0 + i + 1);
		indices.push_back(0 + i);

		indices.push_back((quality - 1) * qualMaisUm + i);
		indices.push_back((quality - 1) * qualMaisUm + i + 1);
		indices.push_back(0 + i + 1);
	}

	//init colors
	for (int i = 0; i < vertices.size(); i++){
		colors.push_back(1.0f);
		colors.push_back((GLfloat)i / vertices.size());
		colors.push_back(0.3f);
	}
}

void VBOSphere::genBuffers(){

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[4];
	glGenBuffers(4, handle);
	
	/////// TEST: this works as well =)
	//std::vector<glm::vec3> verts;
	//for (int i = 0; i < vertices.size(); i += 3){
	//	verts.push_back(glm::vec3(vertices.at(i), vertices.at(i + 1), vertices.at(i + 2)));
	//}
	//glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), (GLvoid*)&verts[0], GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	//glEnableVertexAttribArray(0);  // Vertex position
	/////////////

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), (GLvoid*)&colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex color

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), (GLvoid*)&normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}