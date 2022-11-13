#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;

Program program[3];
int selectProg = 0;
std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS];
int selectMat = 0;

struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
	bool spotlight = 0;	// Determines whether light is a spotlight
	glm::vec3 A;	// Direction vector
	float theta = 20;	// Angle bw A and cone edge
	float alpha = 0.5;
} lights[NUM_LIGHTS];
bool selectLight = 0;

void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelInvTrans = glm::inverse(glm::transpose(modelMatrix));

	program[selectProg].Bind();
	program[selectProg].SendUniformData(modelMatrix, "model");
	program[selectProg].SendUniformData(modelInvTrans, "modelInvTrans"); // Use inverse transpose model to get correct normals
	program[selectProg].SendUniformData(viewMatrix, "view");
	program[selectProg].SendUniformData(projectionMatrix, "projection");

	// Send material and light data
	program[selectProg].SendUniformData(materials[selectMat].ka, "ka");
	program[selectProg].SendUniformData(materials[selectMat].kd, "kd");
	program[selectProg].SendUniformData(materials[selectMat].ks, "ks");
	program[selectProg].SendUniformData(materials[selectMat].s, "s");
	program[selectProg].SendUniformData(lights[0].position, "lights[0].position");
	program[selectProg].SendUniformData(lights[0].color, "lights[0].color");
	program[selectProg].SendUniformData(lights[0].spotlight, "lights[0].spotlight");
	program[selectProg].SendUniformData(lights[0].A, "lights[0].A");
	program[selectProg].SendUniformData(lights[0].theta, "lights[0].theta");
	program[selectProg].SendUniformData(lights[0].alpha, "lights[0].alpha");
	program[selectProg].SendUniformData(lights[1].position, "lights[1].position");
	program[selectProg].SendUniformData(lights[1].color, "lights[1].color");
	program[selectProg].SendUniformData(lights[1].spotlight, "lights[1].spotlight");
	program[selectProg].SendUniformData(lights[1].A, "lights[1].A");
	program[selectProg].SendUniformData(lights[1].theta, "lights[1].theta");
	program[selectProg].SendUniformData(lights[1].alpha, "lights[1].alpha");

	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program[selectProg].Unbind();

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	
	//Change material
	case 'm':
		selectMat++;
		if (selectMat > NUM_MATERIALS - 1) { selectMat = 0; }
		std::cout << "Material " << selectMat << std::endl;
		break;
	case 'M':
		selectMat--;
		if (selectMat < 0) { selectMat = NUM_MATERIALS - 1; }
		std::cout << "Material " << selectMat << std::endl;
		break;
	
	// Change light position
	case 'l':
		selectLight = !selectLight;
		std::cout << "Light " << selectLight << std::endl;
		break;
	case 'L':
		selectLight = !selectLight;
		std::cout << "Light " << selectLight << std::endl;
		break;
	case 'x':
		lights[selectLight].position.x++;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;
	case 'X':
		lights[selectLight].position.x--;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;
	case 'y':
		lights[selectLight].position.y++;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;
	case 'Y':
		lights[selectLight].position.y--;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;
	case 'z':
		lights[selectLight].position.z++;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;
	case 'Z':
		lights[selectLight].position.z--;
		std::cout << "Light " << selectLight << " position: (" << lights[selectLight].position.x << ", " << lights[selectLight].position.y << ", " << lights[selectLight].position.z << ")\n";
		break;

	// Enable spotlight
	case 's':
		lights[selectLight].spotlight = !lights[selectLight].spotlight;
		std::cout << "Spotlight enable " << lights[selectLight].spotlight << std::endl;
		break;

	// Select between Gouraud, Phong, and Silhouette shading
	case '1':
		selectProg = 0;
		std::cout << "Shader 1: Gouraud\n";
		break;
	case '2':
		selectProg = 1;
		std::cout << "Shader 2: Phong\n";
		break;
	case '3':
		selectProg = 2;
		std::cout << "Shader 3: Silhouette\n";
		break;

	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	// Set material and light data
	materials[0].ka = glm::vec3{ 0.2, 0.2, 0.2 };
	materials[0].kd = glm::vec3{ 0.8, 0.7, 0.7 };
	materials[0].ks = glm::vec3{ 1.0, 1.0, 1.0 };
	materials[0].s = 10.0;
	materials[1].ka = glm::vec3{ 0.0, 0.2, 0.2 };
	materials[1].kd = glm::vec3{ 0.5, 0.7, 0.2 };
	materials[1].ks = glm::vec3{ 0.1, 1.0, 0.1 };
	materials[1].s = 100.0;
	materials[2].ka = glm::vec3{ 0.2, 0.2, 0.2 };
	materials[2].kd = glm::vec3{ 0.1, 0.3, 0.9 };
	materials[2].ks = glm::vec3{ 0.1, 0.1, 0.1 };
	materials[2].s = 1.0;

	lights[0].position = glm::vec3{ 0.0, 0.0, 3.0 };
	lights[0].color = glm::vec3{ 0.5, 0.5, 0.5 };
	lights[0].A = -lights[0].position;
	lights[1].position = glm::vec3{ 0.0, 3.0, 0.0 };
	lights[1].color = glm::vec3{ 0.2, 0.2, 0.2 };
	lights[1].A = -lights[1].position;
	
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Ethan Martinez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	
	program[0].SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
	program[0].Init();
	program[0].SendAttributeData(posBuff, "vPositionModel");
	program[0].SendAttributeData(norBuff, "vNormalModel");

	program[1].SetShadersFileName("../shaders/shader2.vert", "../shaders/shader2.frag");
	program[1].Init();
	program[1].SendAttributeData(posBuff, "vPositionModel");
	program[1].SendAttributeData(norBuff, "vNormalModel");

	program[2].SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");
	program[2].Init();
	program[2].SendAttributeData(posBuff, "vPositionModel");
	program[2].SendAttributeData(norBuff, "vNormalModel");
}


int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}