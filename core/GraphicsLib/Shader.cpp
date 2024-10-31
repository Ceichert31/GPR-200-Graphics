#include "Shader.h"
#include "../ew/external/glad.h"

namespace GraphicsLib {

	Shader::Shader(const char* vertexPath, const char* fragmentPath) {

		//Source code
		std::string vertexBuffer, fragmentBuffer;

		//Files to read from
		std::ifstream vertShaderFile;
		std::ifstream fragShaderFile;

		//ifstream exceptions
		vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vertShaderFile.open(vertexPath);
			fragShaderFile.open(fragmentPath);

			std::stringstream vertStream, fragStream;

			//Output to string stream
			vertStream << vertShaderFile.rdbuf();
			fragStream << fragShaderFile.rdbuf();

			//Close
			vertShaderFile.close();
			fragShaderFile.close();

			//Convert to string
			vertexBuffer = vertStream.str();
			fragmentBuffer = fragStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR:SHADER:FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		//Convert from buffer to c-string
		const char* vertShaderCode = vertexBuffer.c_str();
		const char* fragShaderCode = fragmentBuffer.c_str();

		//Shader setup
		unsigned int vertexShader, fragmentShader;

		//Debug setup
		int success;
		char infoLog[512];

		//Assign vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Load source code into shader
		glShaderSource(vertexShader, 1, &vertShaderCode, NULL);

		//Compile vertex shader
		glCompileShader(vertexShader);

		//Return shader iv
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		//If failed, print logs
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
		}

		//Assign fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Load source code into shader
		glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);

		//Compile fragment shader
		glCompileShader(fragmentShader);

		//Return shader iv
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		//If failed, print logs
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
		}

		//Assign program ID
		ID = glCreateProgram();

		//Attach shaders to program
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);

		//link program to ID
		glLinkProgram(ID);

		//Get program compile status
		glGetProgramiv(ID, GL_LINK_STATUS, &success);

		//If failed, print logs
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
		}

		//Deallocate shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::use()
	{
		glUseProgram(ID);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void Shader::setVector3(const std::string& name, glm::vec3 vec3) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
	}
	void Shader::setVector4(const std::string& name, float valueOne, float valueTwo, float valueThree, float valueFour) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), valueOne, valueTwo, valueThree, valueFour);
	}
	void Shader::setMatrix4(const std::string& name, glm::mat4 matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		//&value[0][0]
	}
}