#ifndef TEXTURE_H
#define TEXTURE_H

//#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../ew/external/glad.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "../ew/external/stb_image.h"

namespace GraphicsLib {
    class Texture2D {
    public:
        Texture2D(const char* filePath, int filterMode, int wrapMode);
        ~Texture2D();
        void Bind(unsigned int slot = 0); //Bind to a specific texture unit
    private:
        unsigned int m_id; //GL texture handle
        int m_width, m_height;
    };
}
#endif