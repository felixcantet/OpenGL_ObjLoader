//
//
//

// GLEW_STATIC force le linkage statique
// c-a-d que le code de glew est directement injecte dans l'executable
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vertex.h"
#include "Mat4.h"
#include "Vector2.h"

// les repertoires d'includes sont:
// ../libs/glfw-3.3/include			fenetrage
// ../libs/glew-2.1.0/include		extensions OpenGL
// ../libs/stb						gestion des images (entre autre)

// les repertoires des libs sont (en 64-bit):
// ../libs/glfw-3.3/lib-vc2015
// ../libs/glew-2.1.0/lib/Release/x64

// Pensez a copier les dll dans le repertoire x64/Debug, cad:
// glfw-3.3/lib-vc2015/glfw3.dll
// glew-2.1.0/bin/Release/x64/glew32.dll		si pas GLEW_STATIC

// _WIN32 indique un programme Windows
// _MSC_VER indique la version du compilateur VC++
#if defined(_WIN32) && defined(_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")			// glew32.lib si pas GLEW_STATIC
#pragma comment(lib, "opengl32.lib")
#elif defined(__APPLE__)
#elif defined(__linux__)
#endif

#include <iostream>
#include "../common/GLShader.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"#include "Material.h"

GLShader g_BasicShader;
GLuint VAO;
GLuint VBO;
GLuint IBO;
GLuint texID;

mat4 matrix;
mat4 scaleMat;
mat4 translationMat;
mat4 perspectiveProjectionMatrix;

Material material;

// List of computed vertices
std::vector<Vertex> vertices;
// List of computed indices
std::vector<uint32_t> indices;

// Possible Optimization 
void CheckVertex(Vertex &v)
{
	// If vertices already exist in buffer
	for (int i = 0; i < vertices.size(); i++) {
		if (v.x == vertices.at(i).x)
			if (v.y == vertices.at(i).y)
				if (v.z == vertices.at(i).z)
					if (v.r == vertices.at(i).r)
						if (v.g == vertices.at(i).g)
							if (v.b == vertices.at(i).b)
								if (v.nx == vertices.at(i).nx)
									if (v.ny == vertices.at(i).ny)
										if (v.nz == vertices.at(i).nz)
										{
											// Add indices of the existing one
											indices.push_back(i);
											return;
										}

	}
	// Else add new vertex and add this vertex to indices
	vertices.push_back(v);
	indices.push_back(vertices.size() - 1);
}


void LoadModel(std::string modelPath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string wn, err;

	// Load file datas
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &wn, &err, modelPath.c_str()))
		throw std::runtime_error(wn + err);

	// Write datas to vertices
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};
			// read position
			if (attrib.vertices.size() > 0) {
				vertex.x = attrib.vertices[3 * index.vertex_index + 0];
				vertex.y = attrib.vertices[3 * index.vertex_index + 1];
				vertex.z = attrib.vertices[3 * index.vertex_index + 2];
			}

			// read normals
			if (attrib.normals.size() > 0) {
				vertex.nx = attrib.normals[3 * index.normal_index + 0];
				vertex.ny = attrib.normals[3 * index.normal_index + 1];
				vertex.nz = attrib.normals[3 * index.normal_index + 2];
			}

			if (attrib.texcoords.size() > 0) {
				vertex.u = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.v = 1 - attrib.texcoords[2 * index.texcoord_index + 1];
			}

			// Use position as color
			vertex.r = vertex.x;
			vertex.g = vertex.y;
			vertex.b = vertex.z;



			// Check if vertex already exist and fill buffer
			CheckVertex(vertex);
		}
		printf("# of materials = %d\n", (int)materials.size());
	}

	/* material
		materials.at(0).diffuse[0],
		materials.at(0).diffuse[1],
		materials.at(0).diffuse[2],
		materials.at(0).specular[0],
		materials.at(0).specular[1],
		materials.at(0).specular[2],
		materials.at(0).shininess);*/

	material.diffuse_r = materials.at(0).diffuse[0];
	material.diffuse_g = materials.at(0).diffuse[1];
	material.diffuse_b = materials.at(0).diffuse[2];
	
	material.specular_r = materials.at(0).specular[0];
	material.specular_g = materials.at(0).specular[1];
	material.specular_b = materials.at(0).specular[2];
	material.shininess = materials.at(0).shininess;

}
bool Initialize()
{
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "erreur d'initialisation de GLEW!"
			<< std::endl;
	}

	// Create and bind the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	int texWidth = 2048;
	int texHeight = 2048;
	uint8_t* data = stbi_load("../data/None_Base_Color.png", &texWidth, &texHeight, nullptr, STBI_rgb_alpha);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glActiveTexture(GL_TEXTURE0);

	// Create and define shader programs
	g_BasicShader.LoadVertexShader("BasicVertex.vs");
	g_BasicShader.LoadFragmentShader("BasicFragment.fs");
	g_BasicShader.Create();

	uint32_t basicProgram = g_BasicShader.GetProgram();
	glUseProgram(basicProgram);

	// Load Models and fill buffers
	LoadModel("../data/export.obj");

	// Test to load multiple object in same buffer
	//LoadModel("../data/suzanne.obj");

	// Define vertex positions and color
	static const Vertex vertex[3] =
	{
		{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}

	};



	// Generate and bind the Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Fill the Buffer with the vertex array datas
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, vertex, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// Loader : sizeof(Vertex) * vertices.size(), &vertices[0]
	auto textureLocation = glGetAttribLocation(basicProgram, "u_material.diffuse");
	glUniform1i(textureLocation, 1);
	//glVertexAttribPointer 

	auto specularLocation = glGetAttribLocation(basicProgram, "u_material.specular");
	glUniform3f(specularLocation, material.specular_r, material.specular_g, material.specular_b);

	auto shininessLocation = glGetAttribLocation(basicProgram, "u_material.shininess");
	glUniform1f(shininessLocation, material.shininess);

	int texCoords = glGetAttribLocation(basicProgram, "a_texcoords");
	glEnableVertexAttribArray(texCoords);
	glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, u));

	// Define location attribute
	int location = glGetAttribLocation(basicProgram, "a_position");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	int normals = glGetAttribLocation(basicProgram, "a_normals");
	glEnableVertexAttribArray(normals);
	glVertexAttribPointer(normals, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, nx));

	// Define color attribute
	int color = glGetAttribLocation(basicProgram, "a_color");
	glEnableVertexAttribArray(color);
	//glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, r));
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, r));
	// Loader : color, 3, sizeof(Vertex), offsetof(Vertex, r)



	 //Define indices datas
	//static const GLuint indices[] = {
	//	0,1,2
	//};

	// Create and Bind Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Fill the buffer with the indices array datas
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// Loader : sizeof(&indices), &indices[0]

	// Lock Buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;

#ifdef WIN32
	wglSwapIntervalEXT(1);
#endif
	return true;
}




void Terminate() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteTextures(1, &texID);
	glDeleteBuffers(1, &VAO);
	g_BasicShader.Destroy();
	delete[](matrix.data);
	delete[](scaleMat.data);
	delete[](translationMat.data);
	delete[](perspectiveProjectionMatrix.data);
}

void Shutdown()
{
}

void Display(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Defini le viewport en pleine fenetre
	glViewport(0, 0, width, height);

	// Idem mais en OpenGL moderne
	// defini une liste de sommets, 2D, de type float
	const float triangles[] = {
		-0.5f, -0.5f,
		-0.5f, +0.5f,
		+0.5f, +0.5f,
		// second triangle
		-0.5f, -0.5f,
		+0.5f, +0.5f,
		+0.5f, -0.5f
	};


	// Specifie la structure des donnees envoyees au GPU
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), triangles);
	// indique que les donnees sont sous forme de tableau
	glEnableVertexAttribArray(0);
	// dessine deux triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Render(GLFWwindow* window)
{
	// etape a. A vous de recuperer/passer les variables width/height
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	// etape b. Notez que glClearColor est un etat, donc persistant
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	float near = 1.f;
	float far = 1000.f;
	float right = 640;
	float left = 0;
	float top = 0;
	float bottom = -480;
	float fov = 60;
	float aspect = 1.333;
	uint32_t basicProgram = g_BasicShader.GetProgram();
	glUseProgram(basicProgram);

	float currentTime = (float)glfwGetTime();
	int timeLocation = glGetUniformLocation(basicProgram, "u_time");
	glUniform1f(timeLocation, currentTime);


	scaleMat.scale(1);
	//matrix.rotate(currentTime);
	matrix.setUpRotationMatrix(currentTime * 20, 0.f, 40.f, 0.f);
	translationMat.translate(0, 0, -5);
	//perspectiveProjectionMatrix.orthographique(left, right, bottom, top, near, far);
	//perspectiveProjectionMatrix.orthogra_aphique(left, right, bottom, top, near, far);
	perspectiveProjectionMatrix.perspective(fov, aspect, near, far);
	int matLocation = glGetUniformLocation(basicProgram, "u_matrix");
	glUniformMatrix4fv(matLocation, 1, false, scaleMat.data);

	int scaleLocation = glGetUniformLocation(basicProgram, "u_rotMatrix");
	glUniformMatrix4fv(scaleLocation, 1, false, matrix.data);

	int translateLocation = glGetUniformLocation(basicProgram, "u_translateMatrix");
	glUniformMatrix4fv(translateLocation, 1, false, translationMat.data);

	int projectionLocation = glGetUniformLocation(basicProgram, "u_projectionMatrix");
	glUniformMatrix4fv(projectionLocation, 1, false, perspectiveProjectionMatrix.data);


	// Render Vertex Array 
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texID);

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// 
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// toutes nos initialisations vont ici
	Initialize();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		Render(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// ne pas oublier de liberer la memoire etc...
	Shutdown();

	glfwTerminate();
	return 0;
}