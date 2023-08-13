#include "Shader.h"

Shader::Shader(const char* vspath, const char* fspath, const char* gspath, std::vector<std::string> rep_from, std::vector<std::string> rep_to)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vspath, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
		{
			VertexShaderCode += "\n" + Line;
		}
		VertexShaderStream.close();
		if (rep_from.size() != 0) {
			for (int i = 0; i < rep_from.size(); i++) {
				VertexShaderCode = std::regex_replace(VertexShaderCode, std::regex(rep_from[i]), rep_to[i]);
			}
		}
	}
	else
	{
		std::cout << "cannot open " << vspath << std::endl;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fspath, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
		{
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
		if (rep_from.size() != 0) {
			for (int i = 0; i < rep_from.size(); i++) {
				FragmentShaderCode = std::regex_replace(FragmentShaderCode, std::regex(rep_from[i]), rep_to[i]);
			}
		}
	}
	else
	{
		std::cout << "cannot open " << fspath << std::endl;
	}

	std::string GeometryShaderCode;
	if (gspath != nullptr) {
		std::ifstream GeometryShaderStream(gspath, std::ios::in);
		if (GeometryShaderStream.is_open())
		{
			std::string Line = "";
			while (getline(GeometryShaderStream, Line))
			{
				GeometryShaderCode += "\n" + Line;
			}
			GeometryShaderStream.close();
			if (rep_from.size() != 0) {
				for (int i = 0; i < rep_from.size(); i++) {
					GeometryShaderCode = std::regex_replace(GeometryShaderCode, std::regex(rep_from[i]), rep_to[i]);
				}
			}
		}
		else
		{
			std::cout << "cannot open " << gspath << std::endl;
		}

	}


	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Compile Shaders
	std::cout << "Compiling shader: " << vspath << std::endl;
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}

	std::cout << "Compiling shader: " << fspath << std::endl;
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}

	if (gspath != nullptr) {
		std::cout << "Compiling shader: " << gspath << std::endl;
		char const* GeometrySourcePointer = GeometryShaderCode.c_str();
		glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
		glCompileShader(GeometryShaderID);

		glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0)
		{
			std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
			std::cout << &GeometryShaderErrorMessage[0] << std::endl;
		}
	}

	//Link Program
	std::cout << "Linkng Program" << std::endl;
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, VertexShaderID);
	if (gspath != nullptr)
		glAttachShader(mProgramID, GeometryShaderID);
	glAttachShader(mProgramID, FragmentShaderID);
	glLinkProgram(mProgramID);

	glGetProgramiv(mProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(mProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDetachShader(mProgramID, VertexShaderID);
	if (gspath != nullptr)
		glDetachShader(mProgramID, GeometryShaderID);
	glDetachShader(mProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	if (gspath != nullptr)
		glDeleteShader(GeometryShaderID);
	glDeleteShader(FragmentShaderID);
}


void Shader::ConfigureAttrib(const char* name, GLuint buffer, GLuint n)
{
	GLuint id;
	id = glGetAttribLocation(mProgramID, name);
	glEnableVertexAttribArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(id, n, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Shader::ConfigureTexture(const char* name, GLuint texture, GLuint unit)
{
	GLuint id;
	id = glGetUniformLocation(mProgramID, name);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(id, unit);
}

void Shader::ConfigureCubeTexture(const char* name, GLuint texture, GLuint unit)
{
	GLuint id;
	id = glGetUniformLocation(mProgramID, name);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glUniform1i(id, unit);
}

void Shader::ConfigureTextureArray(const char* name, GLuint texture, GLuint unit)
{
	GLuint id;
	id = glGetUniformLocation(mProgramID, name);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glUniform1i(id, unit);
}

void Shader::ConfigureCubeArray(const char* name, GLuint texture, GLuint unit)
{
	GLuint id;
	id = glGetUniformLocation(mProgramID, name);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture);
	glUniform1i(id, unit);
}



