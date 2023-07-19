#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

namespace higui
{
	class Shader
	{
	public:
		uint32_t ID;
		Shader(const char* vertex_path, const char* fragment_path);
		Shader(std::string shader_name);

		void use();
		void Delete();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;

		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;

		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};

	class ShaderManager
	{
	public:
		static Shader* shader(const std::string& shader_name);

		static void RegisterShader(const std::string& shader_name, const char* vertex_path, const char* fragment_path);
		static void RegisterShader(const std::string& shader_name);
		static void Delete();

	private:
		static std::unordered_map<std::string, Shader*> shaders;
	};

	inline std::unordered_map<std::string, Shader*> ShaderManager::shaders;
}
#endif