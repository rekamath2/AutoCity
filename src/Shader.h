#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

using namespace std;

class Shader
{
public:
    GLuint Program_ID;

    Shader() {};
    ~Shader() {};

    bool loadShader(GLchar* vertexPath, GLchar* fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        string vertexCode;
        string fragmentCode;
        bool   errorFlag;

        try
        {
            errorFlag = false;
            cout << "---> Reading '" << vertexPath << "' and '"
                 << fragmentPath << "' ..." << endl;
            // Open files
            ifstream vShaderFile(vertexPath);
            ifstream fShaderFile(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(exception e)
        {
            cout << "***> ERROR: Failed to read shader program file(s) :-("
                 << endl;
            errorFlag = true;
        }

        if ((vertexCode.length() == 0) || (fragmentCode.length() == 0))
        {
            errorFlag = true;
            if (vertexCode.length() == 0)
                cout << "***> ERROR: Reading '" <<  vertexPath
                     << "' has failed!" << endl;
            if (fragmentCode.length() == 0)
                cout << "***> ERROR: Reading '" <<  fragmentPath
                     << "' has failed!" << endl;
            cout << endl;
        }
        else cout << "---> Shader program files were read successfully :-)" << endl;
        //----------------------------------------------------

        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();

        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint  success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            cout << "***> ERROR: Compiling vertex program has failed: \n"
                 << infoLog << endl;
            errorFlag = true;
        }
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "***> ERROR: Compiling fragment program has failed: \n"
                 << infoLog << endl;
            errorFlag = true;
        }
        // Shader Program
        this->Program_ID = glCreateProgram();
        glAttachShader(this->Program_ID, vertex);
        glAttachShader(this->Program_ID, fragment);
        glLinkProgram(this->Program_ID);
        // Print linking errors if any
        glGetProgramiv(this->Program_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program_ID, 512, NULL, infoLog);
            cout << "***> ERROR: Linking shader programs has failed: \n"
                 << infoLog << endl;
            errorFlag = true;
        }
        // Delete the shaders as they're linked into our program now
        // and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (!errorFlag)
            cout << "===> Shaders were Compiled and Linked successfully :-)\n"
                 << endl;
        else cout << endl;

        return !errorFlag;
    }
    // Uses the current shader
    void useProgram()
    {
        glUseProgram(this->Program_ID);
    }
};

#endif // SHADER_H
