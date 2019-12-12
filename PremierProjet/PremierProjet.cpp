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

GLShader g_BasicShader;
GLuint VAO;
GLuint VBO;
GLuint IBO;

mat4 matrix;
mat4 scaleMat;
mat4 translationMat;
mat4 perspectiveProjectionMatrix;

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

	// Create and define shader programs
	g_BasicShader.LoadVertexShader("BasicVertex.vs");
	g_BasicShader.LoadFragmentShader("BasicFragment.fs");
	g_BasicShader.Create();

	uint32_t basicProgram = g_BasicShader.GetProgram();
	glUseProgram(basicProgram);

	/*std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string wn, err;*/

	Vector2 v(2, 2);
	Vector2 v1(1, 4);
	v = v + v1;

	std::cout << "X = " << v.get_x() << std::endl;
	std::cout << "Y = " << v.get_y() << std::endl;
	

	//if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &wn, &err, "data/icosahedron.obj"))
	//	throw std::runtime_error(wn + err);

	//for (const auto& shape : shapes) {
	//	for (const auto& index : shape.mesh.indices) {
	//		Vertex vertex = {};

	//		vertices.push_back(vertex);
	//		indices.push_back(indices.size());
	//	}
	//}


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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, vertex, GL_STATIC_DRAW);
	
	// Define location attribute
	int location = glGetAttribLocation(basicProgram, "a_position");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	// Define color attribute
	int color = glGetAttribLocation(basicProgram, "a_color");
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, r));

	


	// Define indices datas
	static const GLuint indices[] = {
		0,1,2
	};

	// Create and Bind Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Fill the buffer with the indices array datas
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	

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
	g_BasicShader.Destroy();
	delete(matrix.data);
	delete(scaleMat.data);
	delete(translationMat.data);
	delete(perspectiveProjectionMatrix.data);
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
	glClear(GL_COLOR_BUFFER_BIT);

	float near = -1.f;
	float far = 1.f;
	float right = 640;
	float left = 0;
	float top = 0;
	float bottom = -480;


	uint32_t basicProgram = g_BasicShader.GetProgram();
	glUseProgram(basicProgram);
	
	float currentTime = (float)glfwGetTime();
	int timeLocation = glGetUniformLocation(basicProgram, "u_time");
	glUniform1f(timeLocation, currentTime);


	scaleMat.scale(300);
	matrix.rotate(currentTime);
	translationMat.translate(250, -250, -0);
	perspectiveProjectionMatrix.orthographique(left, right, bottom, top, near, far);
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
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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