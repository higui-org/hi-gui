#include "higui.h"

namespace higui
{
	namespace internal {
		GlobalInitializer::GlobalInitializer() {
			IntAttributeTag::RegisterAs("int");
			FloatAttributeTag::RegisterAs("float");
			StringAttributeTag::RegisterAs("string");
			StringAttributeTag::RegisterAs("str");
			Dock::RegisterAs("dock");
		}

		GlobalInitializer global_initializer;
	}
}