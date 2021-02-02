#ifndef UTILS_H
#define UTILS_H
 
// #define GLEW_STATIC
#include <GL/glew.h>
//#include <SOIL2/SOIL2.h>
#include "../SOIL2/SOIL2.h"
#include <string>
#include <fstream>
#include <iostream>
 
namespace Utils { 
    std::string readShader(const char *filePath) {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);
        std::string line = "";
 
        while(getline(fileStream, line)) {
            content.append(line + "\n");
        }
 
        fileStream.close();
        return content;
    }
    bool checkOpenGLError() {
        bool foundError = false;
        int glErr = glGetError();
        while (glErr != GL_NO_ERROR) {
            std::cout << "glError: " << glErr << std::endl;
            foundError = true;
            glErr = glGetError();
        }
        return foundError;
    }

    void printShaderLog(GLuint shader) {
        int len = 0;
        int chWrittn = 0;
        char* log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char*)malloc(len);
            glGetShaderInfoLog(shader, len, &chWrittn, log);
            std::cout << "Shader Info Log: " << log << std::endl;
            free(log);
        }
    }

    void printProgramLog(int prog) {
        int len = 0;
        int chWrittn = 0;
        char* log;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char*)malloc(len);
            glGetProgramInfoLog(prog, len, &chWrittn, log);
            std::cout << "Program Info Log: " << log << std::endl;
            free(log);
        }
    }
    unsigned int createShaderProgram(const char *file_vs, const char *file_fs ) {
        GLint vertCompiled;
        GLint fragCompiled;
        GLint linked;

        std::string str_src_vs = readShader(file_vs);
        std::string str_src_fs = readShader(file_fs);
 
        const char *src_vs = str_src_vs.c_str();
        const char *src_fs = str_src_fs.c_str();
 
        unsigned int vs, fs; 
        vs = glCreateShader(GL_VERTEX_SHADER);
        fs = glCreateShader(GL_FRAGMENT_SHADER);
 
        glShaderSource(vs, 1, &(src_vs), NULL);
        glShaderSource(fs, 1, &(src_fs), NULL);
 
        glCompileShader(vs);
        checkOpenGLError();
        glGetShaderiv(vs, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1) {
            std::cout << "vertex compilation failed" << std::endl;
            printShaderLog(vs);
        }

        glCompileShader(fs);
        glCompileShader(fs);
        checkOpenGLError();
        glGetShaderiv(fs, GL_COMPILE_STATUS, &fragCompiled);
        if (fragCompiled != 1) {
            std::cout << "fragment compilation failed" << std::endl;
            printShaderLog(fs);
        }
 
        unsigned int program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        checkOpenGLError();
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != 1) {
            std::cout << "linking failed" << std::endl;
            printProgramLog(program);
        }
 
        return program;
    }

    unsigned int loadTexture(const char *texImagePath) {
        GLuint textureRef;
        textureRef = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if (textureRef == 0) 
            std::cout << "didnt find texture file " << texImagePath << std::endl;
        // ----- mipmap/anisotropic section
        glBindTexture(GL_TEXTURE_2D, textureRef);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
            GLfloat anisoset = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoset);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoset);
        }
        // ----- end of mipmap/anisotropic section
        return textureRef;
    }
}
#endif
