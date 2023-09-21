#include "higui.h"

namespace higui
{
	namespace internal {
		GlobalInitializer::GlobalInitializer() {
			// basic attribute tags
			IntAttributeTag::Register("int");
			FloatAttributeTag::Register("float");
			StringAttributeTag::Register("string");
			StringAttributeTag::Register("str");

			// dock
			Dock::Register("dock");

			// basic markup tags
			DivTag::Register("div");
		}

		GlobalInitializer global_initializer;
	}
}