#include "higui.h"

namespace higui
{
	namespace internal {
		GlobalInitializer::GlobalInitializer() {
			// basic attributes
			higui::attribute::Int::Register("int");
			higui::attribute::Float::Register("float");
			higui::attribute::String::Register("string");
			higui::attribute::String::Register("str");

			// attribute alignment
			higui::attribute::Alignment::Register("align");

			// basic tags
			tag::Object::Register("obj");
			tag::Div::Register("div");
		}

		GlobalInitializer global_initializer{};
	}
}