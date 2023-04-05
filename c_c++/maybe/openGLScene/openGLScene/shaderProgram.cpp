#include "shaderProgram.h"

#include"program.h"
#include "errorHandling.h"

const int logSize = 512;

unsigned int createShaderProgram(unsigned int* shader, size_t shaderCount);
unsigned int CompileShader(const char* shaderSource, GLenum shaderType);

bool createShaderPrograms(shaderStruct* data)
{
	unsigned int basicShaders[2];
	TRY
	{
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aCol;\n"
			"out vec3 col;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
			"	col = aCol;\n"
			"}\n";
		const char* fragmentShaderSource = "#version 330 core\n"
			"in vec3 col;\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(col, 1.0f);\n"
			"}\n";

		basicShaders[0] = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
		if (basicShaders[0] == 0)
			THROW(USD_ERR_COMPILE_VERTEX_SHADER, "failed to compile vertex shader.");

		basicShaders[1] = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
		if (basicShaders[1] == 0)
			THROW(USD_ERR_COMPILE_FRAGMENT_SHADER, "failed to compile fragment shader.");

		unsigned int program = createShaderProgram(basicShaders, sizeof(basicShaders) / sizeof(unsigned int));
		if (program == 0)
			THROW(USD_ERR_LINK_BASIC_PROGRAM, "failed to link basic shader program");

		glDeleteShader(basicShaders[1]);
		glDeleteShader(basicShaders[0]);

		//glUseProgram(program);

		data->modelLocation = glGetUniformLocation(program, "model");
		if (data->modelLocation == -1)
			THROW(USD_ERR_LOCATION_NOT_FOUND, "failed to find \"model\" location");
		data->modelLocation = glGetUniformLocation(program, "view");
		if (data->modelLocation == -1)
			THROW(USD_ERR_LOCATION_NOT_FOUND, "failed to find \"view\" location");
		data->modelLocation = glGetUniformLocation(program, "projection");
		if (data->modelLocation == -1)
			THROW(USD_ERR_LOCATION_NOT_FOUND, "failed to find \"projection\" location");

		data->basicShader = program;

		return true;
	}
	CATCH(USD_ERR_LOCATION_NOT_FOUND)
	CATCH(USD_ERR_LINK_BASIC_PROGRAM)
		glDeleteShader(basicShaders[1]);
	CATCH(USD_ERR_COMPILE_FRAGMENT_SHADER)
		glDeleteShader(basicShaders[0]);
	CATCH(USD_ERR_COMPILE_VERTEX_SHADER)
		debugMessage(GET_MSG());
	debugMessage("%x", glGetError());
		return false;
}

void deleteShaderPrograms(shaderStruct data)
{
	glUseProgram(0);
	glDeleteProgram(data.basicShader);
}

unsigned int createShaderProgram(unsigned int* shader, size_t shaderCount)
{
	unsigned int shaderProgram = glCreateProgram();
	for (size_t i = 0; i < shaderCount; ++i)
		glAttachShader(shaderProgram, shader[i]);
	glLinkProgram(shaderProgram);

	for (size_t i = 0; i < shaderCount; ++i)
		glDetachShader(shaderProgram, shader[i]);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[logSize];
		glGetProgramInfoLog(shaderProgram, logSize, NULL, log);
		debugMessage(log);

		glDeleteProgram(shaderProgram);

		return 0;
	}

	return shaderProgram;
}

unsigned int CompileShader(const char* shaderSource, GLenum shaderType)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[logSize];
		glGetShaderInfoLog(shader, logSize, NULL, log);
		debugMessage(log);

		glDeleteShader(shader);

		return 0;
	}

	return shader;
}