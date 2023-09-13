#ifndef HI_MARKUP_H
#define HI_MARKUP_H

#include <stack>

#include "object.h"

namespace higui
{
	class Markup
	{
	public:
		Markup(const std::string& markup_file);
		~Markup();

		template <typename T>
		static void RegisterClass(const std::string& name);
		void Init();

	private:
		bool isMarkupValid();

		std::string ExtractTagBlock(size_t offset);
		std::string ExtractTagName(const std::string& tag_block);
		std::string ExtractAttributeValue(const std::string& tag_block, const std::string& attribute_name);
		std::unordered_map<std::string, std::string> ExtractAttributes(const std::string& tag_block);

		std::string markup;
		GUIObject* central_object;

		static std::unordered_map<std::string, std::function<void* ()>> class_factories;

		friend class DOM;
	};

	template <typename T>
	void Markup::RegisterClass(const std::string& name)
	{
		if (class_factories.find(name) != class_factories.end())
		{
			throw std::runtime_error("MarkupParser: the class being registered already exists");
			return;
		}
		class_factories[name] = []() { return new T(); };
	}
}

#endif // HI_MARKUP_H