#include "higui.h"

namespace hi
{
	BoolRoot::BoolRoot() {
		// basic attributes
		attribute::Int::Register("int");
		attribute::Float::Register("float");
		attribute::String::Register("string");
		attribute::String::Register("str");

		// attribute alignment
		attribute::Alignment::Register("align");

		capture_glfw_callbacks = true;
	}

	BoolRoot root;
}