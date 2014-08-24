#include "render.h"

// position
glm::vec3 Render::position = glm::vec3(0, 0, 5);
// horizontal angle : toward -Z
float Render::horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float Render::verticalAngle = 0.0f;

float Render::speed = 3.0f; // 3 units / second
float Render::mouseSpeed = 0.025f;

Render::Render()
{

}

Render::~Render()
{

}


// Render all graphics
void Render::drawScreen(Window & window, sf::Time elapsedTime)
{
	window.clear();
	drawTests(window, elapsedTime);
	window.display();
}

void Render::drawTests(Window & window, sf::Time elapsedTime)
{
	Config* config = Config::Instance();
	// Compute new orientation
	horizontalAngle += mouseSpeed * elapsedTime.asSeconds() * float(config->readSetting<int>("width") / 2 - sf::Mouse::getPosition(window).x);
	verticalAngle += mouseSpeed * elapsedTime.asSeconds() * float(config->readSetting<int>("height") / 2 - sf::Mouse::getPosition(window).y);
	sf::Mouse::setPosition(sf::Vector2i(config->readSetting<int>("width") / 2, config->readSetting<int>("height") / 2), window);
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);
	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross(right, direction);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::string progHalt;
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		std::cin >> progHalt;
	}

	Shader test1;
	test1.init("vertshader.vtxshdr", "fragshader.frgshdr");
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Accept fragments closer to camera
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// Color array
	static const GLfloat g_color_buffer_data[] = {
		0.9f, 0.9f, 0.9f,
		0.9f, 0.9f, 0.9f,
		0.9f, 0.9f, 0.9f,
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f,
		0.4f, 0.4f, 0.4f,
		0.4f, 0.4f, 0.4f,
		0.4f, 0.4f, 0.4f,
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f,
		0.2f, 0.2f, 0.2f,
		0.4f, 0.4f, 0.4f,
		0.4f, 0.4f, 0.4f,
		0.4f, 0.4f, 0.4f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.9f, 0.9f, 0.9f,
		0.9f, 0.9f, 0.9f,
		0.9f, 0.9f, 0.9f
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	

	//ID the color buffer
	GLuint colorbuffer;

	//Gen a buffer for the color buffer
	glGenBuffers(1, &colorbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	
	

	GLuint programID = test1.id();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 ProjectionMatrix = glm::perspective(config->readSetting<float>("fov"), 16.0f / 9.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Draw the cube !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares


	glUseProgram(programID);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}