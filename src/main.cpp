
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

	Dock::RegisterAs("dock");
	IntAttributeValue::RegisterAs("int");
	FloatAttributeValue::RegisterAs("float");
	StringAttributeValue::RegisterAs("str");

	Attribute dock("dock");
	Attribute integer("int", "666");
	Attribute floating("float");
	Attribute str("str");

	std::cout << dock << std::endl;
	std::cout << integer << std::endl;
	std::cout << floating << std::endl;
	std::cout << str << std::endl;

	std::cout << std::endl << std::endl;

	dock = DockPosition::Right;
	integer = 40;
	floating = 66.66666f;
	str = "Slava Ukraini!";

	std::cout << dock << std::endl;
	std::cout << integer << std::endl;
	std::cout << floating << std::endl;
	std::cout << str << std::endl;

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