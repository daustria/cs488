#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 768

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;

// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
struct VertexData {

	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};

struct ViewAdjustor {

	float left;
	float right; 
	float middle;

	float leftIncrement;
	float rightIncrement;
	float middleIncrement;

	ViewAdjustor() : left(0), right(0), middle(0), leftIncrement(0), rightIncrement(0), middleIncrement(0) {

	}

};



class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);

	void initCubeVertices();
	void initCubeIndices();

	void initMatrices();
	void updateModelMatrix();

	glm::vec4 transformVertexProjection(glm::vec3 vertex, bool print_data=false);

	// orthographic projection
	// left and right planes will be x=r, x=-r
	// top and bottom planes will be y=t, y=-t
	// near and far planes will be z=n, z=f where we imagine n,f < 0
	//
	// this also updates clipping planes
	void updateProjectionMatrix(float r, float t, float n, float f);

	void updateCameraMatrix(glm::vec3 eye, glm::vec3 gaze, glm::vec3 up);

	void updateViewingVolume(float l, float r, float t, float b, float n, float f);

	//clips the line a--b against the planes in the viewing volume. modifies a,b so that
	//it represents the shortened line if the line was clipped.
	void clipLine(glm::vec4 &a, glm::vec4 &b, bool print_data);
	
	//same as above except we just clip against the plane specifically defined by N dot (P - Q) = 0
	//where N is the plane_normal and plane_point is Q.
	
	void reset(); // initialize the relevant variables to their starting values

	ShaderProgram m_shader;/* Actual program that we attach the shaders to, and link them */

	// No uniform matrices will be on the shader. For instructional purposes we'll do the matrix manipulations ourselves on the CPU
	// (and explicitly construct the matrices)
	glm::mat4 m_model; // object space to world space
	glm::mat4 m_camera; // world space to camera space

	glm::mat4 m_orth_proj; //orthogonal projection matrix
	glm::mat4 m_persp; //perspective matrix

	glm::mat4 m_proj; // projection, product of orth_proj and perspective matrix

	glm::mat4 m_viewport; //canonical-view-volume to screenspace 
	glm::mat4 m_final; //stores the product of the above matrices

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

	static std::vector<glm::vec3> m_cubeVertices; //this is the data we'll transform to screen coordinates. starts in model coordinates
	static std::vector<int> m_cubeLineIndices; // holds indices of vertices, describing the order we draw the lines of the cube

	// For implementing the transformations along various coordinate systems..
	ViewAdjustor m_scaleAdj; 
	ViewAdjustor m_perspectiveAdj;
	
};

// divides all coordinates by the 4th, assumes it is non-zero
void homogenize4thChart(glm::vec4 &);
