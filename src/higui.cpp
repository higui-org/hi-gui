#include "higui.h"

namespace higui
{
	namespace internal {
		GlobalInitializer::GlobalInitializer() {
			// basic attribute tags
			AttributeInt::Register("int");
			AttributeFloat::Register("float");
			AttributeString::Register("string");
			AttributeString::Register("str");

			// dock
			AttributeDock::Register("dock");

			// basic markup tags
			DivTag::Register("div");
		}

		GlobalInitializer global_initializer{};
	}
}