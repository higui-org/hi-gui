#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "higui.h"

/*
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

*/
int main()
{
	/*
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BioText - GUI test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	*/
	// higui
	using namespace higui;
	
	//Shader::Register("default", "default.vert", "default.frag");
    //DOM dom(window, "test.markup", "test.style");
    //dom.markup.Init();
      
	AttributeContainer container;

	container.add("int", 42);
	container.add("float", 3.14f);
	//container.add("str", "Hello, world!");
	AttributeDock d;
	d.pos = DockPosition::Top;
	d.ratio = 0.4f;
	container.add("dock", d);

	try {
		AttributeInt intValue = container.get<AttributeInt>("int");
		AttributeFloat floatValue = container.get<AttributeFloat>("float");
		//AttributeString stringValue = container.get<AttributeString>("str");
		AttributeDock dockValue = container.get<AttributeDock>("dock");

		std::cout << "int_attribute: " << intValue << std::endl;
		std::cout << "float_attribute: " << floatValue << std::endl;
		//std::cout << "string_attribute: " << stringValue << std::endl;
		std::cout << "dock_attribute: " << dockValue.toString() << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	/*
	// main cycle
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        dom.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    dom.Delete();

    glfwTerminate();
	*/

    return 0;


}