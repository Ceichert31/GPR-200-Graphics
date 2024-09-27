#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../ew/external/stb_image.h"

namespace GraphicsLib {
	Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode)
	{
		//Generate texture holder
		glGenTextures(1, &m_id);

		//Bind to texture holder
		glBindTexture(GL_TEXTURE_2D, m_id);

		//Load texture image
		int nrChannels;

		//Flip texture
		stbi_set_flip_vertically_on_load(true);

		//Load texture
		unsigned char* data = stbi_load(filePath, &m_width, &m_height, &nrChannels, 0);

		if (data) {

			GLenum renderFormat;
			switch (nrChannels)
			{
			case 1:
				renderFormat = GL_RED;
				break;
			case 3:
				renderFormat = GL_RGB;
				break;
			case 4:
				renderFormat = GL_RGBA;
				break;
			}

			GLenum filterSetting;
			switch (filterMode)
			{
			case 1:
				filterSetting = GL_NEAREST;
				break;
			case 2:
				filterSetting = GL_LINEAR;
				break;
			case 3:
				filterSetting = GL_LINEAR_MIPMAP_LINEAR;
				break;
			}

			GLenum wrapSetting;
			switch (wrapMode)
			{
			case 1:
				wrapSetting = GL_REPEAT;
				break;
			case 2:
				wrapSetting = GL_CLAMP_TO_BORDER;
				break;
			}

			glBindTexture(GL_TEXTURE_2D, m_id);

			//Generate image with previously loaded image data
			glTexImage2D(GL_TEXTURE_2D, 0, renderFormat, m_width, m_height, 0, renderFormat, GL_UNSIGNED_BYTE, data);

			//Generate mipmap
			glGenerateMipmap(GL_TEXTURE_2D);

			//Mipmap settings
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterSetting);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterSetting);

			//Set parameters for texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSetting);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapSetting);

			//Free data to recover memory
			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << filePath << std::endl;
		}
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}