#include <iostream>

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/BaseParameter.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


 
static void error_callback(int error, const char* description)
{
    std::cout << "Error:\n" << description << std::endl;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main()
{
    ZigZag::VariantMap vm;

    std::cout << sizeof(ZigZag::VariantMap) << std::endl;

    ZigZag::Variant v(vm);

    // vm.printType<glm::vec4>();
    //vm.printType<glm::vec4>();
    ZigZag::Variant var(glm::mat4(1));
    glm::mat4 m = var.get<glm::mat4>();


    std::cout <<var.isNull() << std::endl;
    std::cout << var.get<bool>() << std::endl;
    std::cout << var.get<glm::mat3>()[0][0] << std::endl;
    var = true;
    std::cout << (int)var.getType() << std::endl;
    std::cout <<var.isNull() << std::endl;
    var = glm::mat4(100);
    std::cout <<var.isNull() << std::endl;

    var.clear();
    std::cout <<var.isNull() << std::endl;

    std::cout <<"haaaaaaaai" << std::endl;

    std::cout << "Hello World! " << sizeof(ZigZag::Variant) << std::endl;
    
    GLFWwindow* window;

    ZigZag::BaseOperator op1;
    ZigZag::BaseOperator op2;

    ZigZag::BaseParameter par1;
    ZigZag::BaseParameter par2;
    ZigZag::BaseParameter par3;

    par1.setParent(&op2);
    par2.setParent(&op2);
    par3.setParent(&op2);

    //op2.setParent(&op1);
    //op1.setParent(&op2);

    std::cout << op2.getChildParameters().size() << std::endl;

    //std::cout << op1.getChildren().size() << std::endl;
    //std::cout << op2.getChildren().size() << std::endl;
    //std::cout << op2.getChildren().size() << std::endl;

    glfwSetErrorCallback(error_callback);
 
 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw "error";
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
 
  
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();

    ZigZag::Object obj(nullptr, "hay");
    std::cout << "hello" << std::endl;

    return 0;
}
