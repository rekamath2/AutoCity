//#pragma once				// Removed by Calvin.  2014-12-18
#ifndef MESH330_H              // Added by Calvin.    2014-12-18
#define MESH330_H              // Added by Calvin.    2014-12-18

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessary OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/types.h>   // Added by Calvin.  2015-08-23 (not an error, 2015!!)

#include "Shader.h"

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    wxString type;
    aiString path;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    // Shader m_shader;			// No need for GLSL 3.30

    /*  Functions  */
    // Constructor
    //Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Shader shader)	// No need for GLSL 3.30
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)		// For GLSL 3.30
    {
        this->vertices = vertices;
        this->indices  = indices;
        this->textures = textures;

        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        //m_shader = shader;			// No need for GLSL 3.30
        //this->setupMesh(m_shader);	// No need for GLSL 3.30
        this->setupMesh();		// No need for GLSL 3.30
    }

    // Render the mesh
    void Draw(Shader shader)
    {
        // Bind appropriate textures
        GLuint diffuseNr  = 1;
        GLuint specularNr = 1;

        for(GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            stringstream ss;
            wxString number;
            wxString name = this->textures[i].type;
            if(name == "texture_diffuse")
                ss << diffuseNr++; // Transfer GLuint to stream
            else if(name == "texture_specular")
                ss << specularNr++; // Transfer GLuint to stream
            number = wxString(ss.str());
            // Now set the sampler to the correct texture unit
            //glUniform1f(glGetUniformLocation(shader.Program, (name + number).c_str()), i);  // Assign sampler2D number to fragment shader.  Calvin.  2015-01-22
            glUniform1i(glGetUniformLocation(shader.Program_ID, (name + number).c_str()), i);    // Change to "1i" instead of "1f".  Calvin.  2015-01-22
            //cout << i << ": " << (name + number).c_str() << endl;                           // Added by Calvin.  2014-12-18
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0); // Always good practice to set everything back to defaults once configured.

        // Draw mesh
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);   // <---- OOXX
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);           // <---- OOXX
        glBindVertexArray(0);
    }

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    //void setupMesh(Shader shader)		// No need for GLSL 3.30
    void setupMesh()					// For GLSL 3.30
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        //cout << "VAO ID: " << VAO << ", VBO ID: " << VBO << ", EBO ID: " << EBO << endl;
        //cout << "vertices.size():" << vertices.size() << ", indices.size():" << indices.size() << endl;

        //glBindVertexArray(this->VAO);     // <---- OOXX
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

/*
		// ---------- For GLSL 1.30 ----------
        glBindVertexArray(this->VAO);       // <---- OOOXX
        // Set the vertex attribute pointers
        GLint Loc;
        // ***** Vertex Positions *****
        // GLSL position attribute
        Loc = glGetAttribLocation( shader.Program_ID, "position" );
        glEnableVertexAttribArray(Loc);
        glVertexAttribPointer(Loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        cout << "position Loc = " << Loc << endl;
        // ***** Vertex Normals *****
        Loc = glGetAttribLocation( shader.Program_ID, "normal" );
        glEnableVertexAttribArray(Loc);
        glVertexAttribPointer(Loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        cout << "normal Loc = " << Loc << endl;
        // ***** Vertex Texture Coords *****
        Loc = glGetAttribLocation( shader.Program_ID, "texCoords" );
        glEnableVertexAttribArray(Loc);
        glVertexAttribPointer(Loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        cout << "texCoords Loc = " << Loc << endl;
		// -----------------------------------
*/
		// ---------- For GLSL 3.30 ----------
		glBindVertexArray(this->VAO);       // <---- OOOXX
        // Set the vertex attribute pointers
        // Note: location numbers must be assigned in GLSL program

        // ***** Vertex Positions *****
        // "0" maps to "position" location in GLSL program
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // ***** Vertex Normals *****
        // "1" maps to "normal" location in GLSL program
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);
        // ***** Vertex Texture Coords *****
        // "2" maps to "texCoords" location in GLSL program
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
		// -----------------------------------

        glBindVertexArray(0);
    }
};

#endif // MESH330_H    // Added by Calvin.  2014-12-18
