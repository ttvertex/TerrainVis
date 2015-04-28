//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureManager.h"

TextureManager* TextureManager::m_inst(0);

TextureManager* TextureManager::Inst()
{
	if(!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif

	UnloadAllTextures();
	m_inst = 0;
}

int mat2vecIndex(int r, int c, int nc){
	return (r * nc + c);
}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//if this texture ID is in use, unload the current texture
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));

	cout << "(" << width << "," << height << ")" << endl;
	//compute vertex and normals
	vector<vec3> vertices;
	vector<vec3> normals;
	float scale = 0.05f;
	for (int i = 0; i < width -1; i++){
		for (int j = 0; j < height-1; j++){
			// four corners' vertex
			//t1
			//vec3 v00t1(i, bits[mat2vecIndex(i, j, width)] / 255.0f * scale, j); // x, height, z
			//vec3 v10t1(i + 1, bits[mat2vecIndex(i + 1, j, width)] / 255.0f* scale, j); // x, height, z
			//vec3 v11t1(i + 1, bits[mat2vecIndex(i + 1, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
			////t2
			//vec3 v11t2(i + 1, bits[mat2vecIndex(i + 1, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
			//vec3 v01t2(i, bits[mat2vecIndex(i, j + 1, width)] / 255.0f* scale, j + 1); // x, height, z
			//vec3 v00t2(i, bits[mat2vecIndex(i, j, width)] / 255.0f* scale, j); // x, height, z
			float fi = (float) i;
			float fj = (float) j;
			vec3 v00t1 = vec3(fi, 0, fj) * scale; // x, height, z
			vec3 v10t1 = vec3(fi + 1, 0, fj) * scale; // x, height, z
			vec3 v11t1 = vec3(fi + 1, 0, fj + 1) * scale; // x, height, z
			//t2
			vec3 v11t2 = vec3(fi, 0, fj + 1) * scale; // x, height, z
			vec3 v01t2 = vec3(fi, 0, fj + 1) * scale; // x, height, z			
			vec3 v00t2 = vec3(fi, 0, fj) * scale; // x, height, z			
			

			vec3 n1 = glm::normalize(cross(v00t1 - v10t1, v00t1 - v11t1));
			vec3 n2 = glm::normalize(cross(v01t2 - v00t2, v01t2 - v11t2));
			//t1
			vertices.push_back(v00t1);
			vertices.push_back(v10t1);
			vertices.push_back(v11t1);
			//t2
			vertices.push_back(v11t2);
			vertices.push_back(v01t2);
			vertices.push_back(v00t2);

			normals.push_back(n1);
			normals.push_back(n2);
		}
	}

	hmap.normals = normals;
	hmap.vertices = vertices;
	hmap.w = width;
	hmap.h = height;

	cout << "vertexes: " << vertices.size() << endl;
	cout << "triangles/normals: " << normals.size() << endl;

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if(m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else
		result = false;

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_texID.end())
		UnloadTexture(i->first);

	//clear the texture map
	m_texID.clear();
}

heightmap_t TextureManager::genMesh(){
	return hmap;
}