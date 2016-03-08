#include "common.h"
#include "component_camera.h"
#include "glm/glm.hpp"
#include "mesh.h"
#include "pc_renderer.h"
#include "pc_shaderprogram.h"
#include "renderer.h"
#include "resource.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 PC_Renderer::vp_;


void CheckGL() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("An OGL error has occured: %u\n", err);
  }
}

bool PC_Renderer::Init() {
  // preload basic shaders
  Storage<ShaderProgram>::Get("basic");
  glViewport(0, 0, DEFAULT_RESOLUTION);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  return false;
}

// bounding box 
void PC_Renderer::DrawBoundingBox()
{
	glPushMatrix();
	int i, j;

	glColor4f(0.6f, 0.6f, 0.6f, 0);

	glBegin(GL_LINES);

	// front face
	for (i = -4; i <= 4; i++)
	{
		glVertex3f(i, -4, -4);
		glVertex3f(i, -4, 4);
	}
	for (j = -4; j <= 4; j++)
	{
		glVertex3f(-4, -4, j);
		glVertex3f(4, -4, j);
	}

	// back face
	for (i = -4; i <= 4; i++)
	{
		glVertex3f(i, 4, -4);
		glVertex3f(i, 4, 4);
	}
	for (j = -4; j <= 4; j++)
	{
		glVertex3f(-4, 4, j);
		glVertex3f(4, 4, j);
	}

	// left face
	for (i = -4; i <= 4; i++)
	{
		glVertex3f(-4, i, -4);
		glVertex3f(-4, i, 4);
	}
	for (j = -4; j <= 4; j++)
	{
		glVertex3f(-4, -4, j);
		glVertex3f(-4, 4, j);
	}

	// right face
	for (i = -4; i <= 4; i++)
	{
		glVertex3f(4, i, -4);
		glVertex3f(4, i, 4);
	}
	for (j = -4; j <= 4; j++)
	{
		glVertex3f(4, -4, j);
		glVertex3f(4, 4, j);
	}

	glEnd();
	glPopMatrix();
}

bool PC_Renderer::Shutdown() { return false; }

void PC_Renderer::SetViewMatrix(const glm::mat4 &vpm) {
  glm::mat4 Projection = glm::perspective(
      glm::radians(45.0f), (float)DEFAULT_RESOLUTION_X / (float)DEFAULT_RESOLUTION_Y, 0.1f, 1000.0f);
  vp_ = Projection * vpm;
}

void PC_Renderer::RenderMesh(const Mesh &m, const mat4 &modelMatrix) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  CheckGL();

  std::string shadername = "basic";
  if (m.shaderPref != "") {
    shadername = m.shaderPref;
    if (Storage<ShaderProgram>::Get(shadername) == nullptr) {
      LOG(logWARNING) << "Can't find shader " << shadername << "for model";
      shadername = "basic";
    }
  }

  const auto prog = Storage<ShaderProgram>::Get(shadername)->program;
  glUseProgram(prog);
  CheckGL();

  GLint vpIn = glGetUniformLocation(prog, "viewprojection");
  GLint mvpIn = glGetUniformLocation(prog, "modelprojection");
  GLint invmvpIn = glGetUniformLocation(prog, "invMvp");
  CheckGL();

  glUniformMatrix4fv(vpIn, 1, false, glm::value_ptr(vp_));
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(modelMatrix));
  if (invmvpIn != -1) {
    mat4 inv = inverse(vp_ * modelMatrix);
    glUniformMatrix4fv(invmvpIn, 1, false, glm::value_ptr(inv));
  }
  CheckGL();

  // Bind to VAO
  glBindVertexArray(m.gVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m.gVBO);
  if (m.hasIndicies) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.gIBO);
  }
  // DRAW
  if (m.line == true) {
    if (m.strip == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_LINE_STRIP, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_LINE_STRIP, 0, m.numVerts);
      }
    } else {
      if (m.hasIndicies) {
        glDrawElements(GL_LINES, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_LINES, 0, m.numVerts);
      }
    }
  } else {
    if (m.fan == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLE_FAN, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLE_FAN, 0, m.numVerts);
      }
    } else if (m.strip == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m.numVerts);
      }
    } else {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLES, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLES, 0, m.numVerts);
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  glBindVertexArray(0);
  glUseProgram(NULL);
}

void PC_Renderer::LoadMesh(Mesh *msh) {
  if (msh->loadedLocal) {
    return;
  }
  assert(msh->loadedMain);

  // Generate VAO
  glGenVertexArrays(1, &msh->gVAO);

  // Bind VAO
  glBindVertexArray(msh->gVAO);

  { // Vertex
    // Generate VBO
    glGenBuffers(1, &(msh->gVBO));

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gVBO));
    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->vertexData.size() * sizeof(vec3), &msh->vertexData[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,            // index
                          3,            // size
                          GL_FLOAT,     // type
                          GL_FALSE,     // normalised
                          sizeof(vec3), // stride
                          NULL          // pointer/offset
                          );
    CheckGL();
  }

  if (msh->hasColours) { // colours data
    // Generate VBO
    glGenBuffers(1, &(msh->gCOLOURBO));
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gCOLOURBO));
    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->colours.size() * sizeof(unsigned int), &msh->colours[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,                    // index
                          4,                    // size
                          GL_UNSIGNED_BYTE,     // type
                          GL_TRUE,              // normalised
                          sizeof(unsigned int), // stride
                          NULL                  // pointer/offset
                          );
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    CheckGL();
  }

  if (msh->hasUvs) {

    // Generate BO
    glGenBuffers(1, &(msh->gUVBO));
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gUVBO));

    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->uvs.size() * sizeof(vec2), &msh->uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,            // index
                          2,            // size
                          GL_FLOAT,     // type
                          GL_FALSE,     // normalised
                          sizeof(vec2), // stride
                          NULL          // pointer/offset
                          );
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    CheckGL();
  }


  if (msh->hasIndicies) {
    glGenBuffers(1, &(msh->gIBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, msh->gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, msh->indices.size() * sizeof(uint32_t), &msh->indices[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    CheckGL();
  }

  // Unblind VAO
  glBindVertexArray(NULL);
  CheckGL();
  msh->loadedLocal = true;
}


GLuint PC_Renderer::loadTexture(const char * imagepath){


	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;


	FILE * file = fopen(imagepath, "rb");
	 if (!file){ printf("Image could not be opened\n"); return 0; }

	 if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		      printf("Not a correct BMP file\n");
		      return false;
		 
	 }

	 // Read ints from the byte array
	  dataPos = *(int*)&(header[0x0A]);
	  imageSize = *(int*)&(header[0x22]);
	  width = *(int*)&(header[0x12]);
	  height = *(int*)&(header[0x16]);

	  // Some BMP files are misformatted, guess missing information
	  if (imageSize == 0){
		  imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	  }

	  if (dataPos == 0){
		  dataPos = 54; // The BMP header is done that way
          }


	  // Create a buffer
	   data = new unsigned char[imageSize];
	  
		   // Read the actual data from the file into the buffer
		   fread(data, 1, imageSize, file);
	  
		   //Everything is in memory now, the file can be closed
		   fclose(file);
			
		   // Create one OpenGL texture
		   GLuint textureID;
		   glGenTextures(1, &textureID);

                   // "Bind" the newly created texture : all future texture functions will modify this texture
		   glBindTexture(GL_TEXTURE_2D, textureID);

		   // Give the image to OpenGL
		   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

                   return 0;
      }

void PC_Renderer::ClearFrame() {
  static float count = 0.0f;
  count += 0.01f;
  float r = (sin((0.1f * count) + 0) * 127.0f) + 50.0f;
  float g = (sin((0.1f * count) + 2) * 127.0f) + 50.0f;
  float b = (sin((0.1f * count) + 4) * 127.0f) + 50.0f;
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.f);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}