#include "higui.h"

namespace higui
{
	Shader::Shader(std::string shader_name) : Shader(
		(shader_name + ".vert").c_str(),
		(shader_name + ".frag").c_str()
	) 
	{ }


	Shader::Shader(const char* vertex_path, const char* fragment_path)
	{
		std::string vertex_source;
		std::string fragment_source;

		std::ifstream vertex_file;
		std::ifstream fragment_file;

		vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertex_file.open(vertex_path);
			fragment_file.open(fragment_path);

			std::stringstream vertex_stream;
			std::stringstream fragment_stream;

			vertex_stream << vertex_file.rdbuf();
			fragment_stream << fragment_file.rdbuf();

			vertex_file.close();
			fragment_file.close();

			vertex_source = vertex_stream.str();
			fragment_source = fragment_stream.str();
		}
		catch (std::ifstream::failure e)
		{
#ifdef HIGUI_DEBUG_MODE
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
#endif
		}
		const char* vertex_c_str = vertex_source.c_str();
		const char* fragment_c_str = fragment_source.c_str();

		uint32_t vertex_object;
		uint32_t fragment_object;

		vertex_object = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_object, 1, &vertex_c_str, NULL);
		glCompileShader(vertex_object);

#ifdef HIGUI_DEBUG_MODE
		int success;
		char info_log[512];
		glGetShaderiv(vertex_object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex_object, 512, NULL, info_log);

			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << info_log << std::endl;
		}
#endif

		fragment_object = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_object, 1, &fragment_c_str, NULL);
		glCompileShader(fragment_object);

#ifdef HIGUI_DEBUG_MODE
		glGetShaderiv(fragment_object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment_object, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << info_log << std::endl;
		}
#endif

		this->ID = glCreateProgram();
		glAttachShader(ID, vertex_object);
		glAttachShader(ID, fragment_object);
		glLinkProgram(ID);

#ifdef HIGUI_DEBUG_MODE
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << info_log << std::endl;
		}
#endif

		glDeleteShader(vertex_object);
		glDeleteShader(fragment_object);
	}

	void Shader::use()
	{
		glUseProgram(ID);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
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

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


	void Shader::Delete()
	{
		glDeleteProgram(ID);
	}


	Shader* ShaderManager::shader(std::string name)
	{
		auto it = ShaderManager::shaders.find(name);

		if (it != ShaderManager::shaders.end())
		{
			return it->second;
		}
		else
		{
#ifdef HIGUI_DEBUG_MODE
			std::cout << "ERROR::SHADER_MANAGER::SHADER_NOT_FOUND" << std::endl;
#endif
			throw std::runtime_error("ShaderManager cannot find shader with name: " + name);
		}
	}

	void ShaderManager::RegisterShader(std::string shader_name, const char* vertex_path, const char* fragment_path)
	{
		Shader* shader = new Shader(vertex_path, fragment_path);
		ShaderManager::shaders.insert(std::make_pair(shader_name, shader));
	}

	void ShaderManager::RegisterShader(std::string shader_name)
	{
		ShaderManager::RegisterShader(
			shader_name,
			(shader_name + ".vert").c_str(),
			(shader_name + ".frag").c_str()
			);
	}

	void ShaderManager::Delete()
	{
		for (auto& shader : shaders)
		{
			shader.second->Delete();
			delete shader.second;
		}
	}
}