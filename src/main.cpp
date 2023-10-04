#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "higui.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	/*
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
	//Shader::Register("default", "default.vert", "default.frag");
    //DOM dom(window, "test.markup", "test.style");
    //dom.markup.Init();
	

	using namespace higui;

	AttributeContainer cont;
	cont["int"] = 34;
	cont["hello"] = 77;
	cont["nu take"] = "tak, ce je string";
	cont["chyslo pi"] = 3.14f;
	cont["a ce je cile chyslo"] = "int: 666";
	cont["align"] = attr::Alignment(Align::Bottom);
	cont["wasd"] = "align: top 25.6%";
	
	attr::Alignment& align = cont.value<attr::Alignment>("align");
	align.pos = Align::Right;
	align.ratio = 0.95223f;

	for (auto& attr : cont)
	{
		std::cout << attr << std::endl;
	}
	
	// main cycle
	/*
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        dom.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	*/
    //dom.Delete();

    //glfwTerminate();
	

    return 0;
}