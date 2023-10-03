#include "higui.h"

namespace higui
{
	namespace internal {
		GlobalInitializer::GlobalInitializer() {
			// basic attribute tags
			higui::attr::Int::Register("int");
			higui::attr::Float::Register("float");
			higui::attr::String::Register("string");
			higui::attr::String::Register("str");

			// alignment
			higui::attr::Alignment::Register("align");

			// basic markup tags
			DivTag::Register("div");
		}

		GlobalInitializer global_initializer{};
	}
}