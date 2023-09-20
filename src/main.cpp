
#include "attribute.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	using namespace higui;

	AttributeContainer container;

	container["str"] = "69";
	container["int"] = "169";
	container["dock"] = "blah-blah";

	container["dock"] = DockPosition::Right;		// set first founded "dock" to Position::Right

	for (const Attribute& attr : container)
	{
		std::cout << attr << std::endl;
	}

	system("pause");

	//std::cout << pos << std::endl;

	/*
    higui::DOM dom(window, "test.markup", "test.style");
    dom.markup.RegisterClass<higui::GUIObject>("object");
    dom.markup.RegisterClass<higui::DivElement>("div");
    dom.markup.Init();
    dom.shaders.RegisterShader("default", "default.vert", "default.frag");
        
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