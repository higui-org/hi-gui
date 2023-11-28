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
		}

		GlobalInitializer global_initializer{};
	}
}