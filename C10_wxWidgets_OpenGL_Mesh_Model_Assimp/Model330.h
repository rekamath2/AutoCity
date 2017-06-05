//#pragma once
#ifndef MODEL330_H            // Added by Calvin.    2014-12-18
#define MODEL330_H            // Added by Calvin.    2014-12-18

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessary OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <SOIL.h>         // Removed by Calvin.  2014-12-17
//#include <SOIL/SOIL.h>      // Modified by Calvin. 2014-12-17
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh330.h"

//GLint TextureFromFile(const wxString path, wxString directory);

class Model
{
    //Shader m_shader;		// No need for GLSL 3.30

public:
    /*  Functions   */
    Model() {};
    // Constructor, expects a filepath to a 3D model.
    //Model(GLchar* path)

    //Model(const wxString path, Shader shader) // For GLSL 1.30
    Model(const wxString path)
    {
        //m_shader = shader;				// No need for GLSL 3.30
        //this->loadModel(path, shader);	// No need for GLSL 3.30
        this->loadModel(path);		// For GLSL 3.30
    }

    // Draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);

        //this->meshes[1].Draw(shader);
    }


    /*  Model Data  */
    vector<Mesh> meshes;
    wxString directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    //void loadModel(wxString path, Shader shader)  // No need for GLSL 3.30
    bool loadModel(wxString path)                   // For GLSL 3.30
    {
        bool errorFlag = false;
        //m_shader = shader;  // No need for GLSL 3.30

        // Read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.ToStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);
        // Check for errors
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            errorFlag = true;
            return false;
        }
        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);

        return true;
    }

private:
    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // Process each mesh located at the current node
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene.
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // Data to fill
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly
                              // convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            //printf("Position : (%10f, %10f, %10f)\n", vector.x, vector.y, vector.z);
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            //printf("Normal   : (%10f, %10f, %10f)\n", vector.x, vector.y, vector.z);

            // Texture Coordinates
            if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                //printf("TexCoords: (%10f, %10f)\n\n", vec.x, vec.y);
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }
        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // Process materials
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN

            // 1. Diffuse maps
            vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. Specular maps
            vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        // Return a mesh object created from the extracted mesh data
        //return Mesh(vertices, indices, textures, m_shader);	// No need for GLSL 3.30
        return Mesh(vertices, indices, textures);				// For GLSL 3.30
    }

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, wxString typeName)
    {
        vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            //cout << "texture = " << str.C_Str() << endl;
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            for (GLuint j = 0; j < textures_loaded.size(); j++)
            {
                if(textures_loaded[j].path == str)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // If texture hasn't been loaded already, load it
                Texture texture;
                wxString filename = wxString(str.C_Str());
                texture.id = TextureFromFile(filename, this->directory);
                texture.type = typeName;
                texture.path = str;
                //cout << "texture = " << str.C_Str() << endl;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unneceserily load duplicate textures.
            }
        }
        return textures;
    }

    GLint TextureFromFile(const wxString path, wxString directory)   // If using GLuint will get "error: ambiguous old declaration", but this does not cause error in Qt.  Calvin.  2015-01-21
    {
        //Generate texture ID and load texture data
        wxString filename = path;
        filename = directory + '/' + filename;
        GLuint textureID;
        glGenTextures(1, &textureID);
        /*
        int width,height;
        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

        if (image != NULL) std::cout << "Reading texture '" << filename << "' successfully." << endl;   // Added by Calvin.  2014-12-17
        else std::cout << "Error: texture filename '" << filename << "' is not loaded." << endl;        // Added by Calvin.  2014-12-17

        // Assign texture to ID
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
        */

        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ------====== TEXTURE 1 ====== ------
        // Load, create texture and generate mipmaps
        wxBitmap bmp1;

        //if (!bmp1.LoadFile(_("../../Images/wall.jpg"))) // Check your file path
        if (!bmp1.LoadFile(filename)) // Check your file path  <--------------------------
        {
            cout << "Loading image file #1 was failed !!" << endl;
            return 0;
        }
        else
            cout << "Loading image file '" << path.ToStdString() << "' was successful !!" << endl;

        wxImage image1;
        image1 = bmp1.ConvertToImage().Mirror(false);  // Flip image vertically
        //image1 = bmp1.ConvertToImage().Mirror(true);  // Flip image vertically

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,           // Upload texture1
                     image1.GetWidth(), image1.GetHeight(),
                     0, GL_RGB, GL_UNSIGNED_BYTE, image1.GetData());
        glGenerateMipmap(GL_TEXTURE_2D);                // Create mipmap

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done

        //cout << "filename = " << filename.ToStdString() << endl;
        //cout << "textureID = " << textureID << endl;

        return textureID;
    }
};

#endif	// MODEL330_H		// Added by Calvin.  2014-12-18
