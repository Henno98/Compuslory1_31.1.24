// source 
//https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


////vertex shader source 
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"uniform mat4 model; // model matrix\n"
//"out vec3 fragColor;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"   // Change color based on x, y, and z positions\n"
//"   float colorValueX = (aPos.x + 1.0) / 2.0; // Map x to [0, 1]\n"
//"   float colorValueY = (aPos.y + 1.0) / 2.0; // Map y to [0, 1]\n"
//"   float colorValueZ = (aPos.z + 1.0) / 2.0; // Map z to [0, 1]\n"
//"   fragColor = vec3(colorValueX, colorValueY, colorValueZ); // RGB based on x, y, z\n"
//"}\0";
//
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"in vec3 fragColor;\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(fragColor, 1.0);\n"
//"}\n\0";
//vertex shader source 
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"

"out vec3 FragColor;\n"

"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"FragColor = aColor;\n"
"}\n\0";
//// fragmenet shader source 
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 FragColor;\n"
"out vec4 FinalColor;\n"
"void main()\n"
"{\n"
"FinalColor = vec4(FragColor, 1.0);\n"
"}\n\0";


struct Vertex
{

    float x, y, z, r, g, b, u, v;

};

float f(float x)
{
    return sin(x);
}
float Derivative(float x)
{
    return ( - cos(x));

}
float SpiralX(float x, float radius)
{
    return (cos(x * (M_PI / 180)) * radius);
}
float SpiralY(float x, float radius)
{
    return (sin(x * (M_PI/180)) * radius);
}

float XYPlane(float x, float y)
{
    return (pow(x,2)) + y;

}



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For compatibility on macOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rectangle window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
   
    ofstream Export;
    string filnavn("VertexPoints");
    string SpiralPoints("SpiralPoints");
    string XYPlanePoints("XYPlanePoints");

  

    
    vector<Vertex> Punkter;
    vector<Vertex> DerivativePunkter;
    vector<Vertex> Spiral;
    vector<Vertex> XYPlanevector;
   

    float lower = -500;
    float max = 500;


    float resolution = 1.f;
    float n = (max - lower) / resolution;
    float h = (max - lower) / n;

    Export.open(filnavn);
    Export << "Total points are " << 1000 << " Points Are x y z r g b" << endl;
    for (float i = lower; i < max * 2; i += h)
    {
        float x = i * h;
        float red = 1.f;
        float green = 1.f;

        if (f(x - 1) > f(x)) {
            Punkter.push_back(Vertex{ x / 10,f(x) / 10,0,red,0,0 });
            DerivativePunkter.push_back((Vertex{ x / 10,Derivative(x) / 10,0,red,0,0 }));
        }
        if (f(x - 1) < f(x)) {
            Punkter.push_back(Vertex{ x / 10,f(x) / 10,0,0,green,0 });
            DerivativePunkter.push_back((Vertex{ x / 10,Derivative(x) / 10,0,0,green,0 }));
        }
        
    }
	for (int i = 0; i < Punkter.size(); i++)
    {

            Export << Punkter[i].x << " " << Punkter[i].y << " 0 " << Punkter[i].r << " " << Punkter[i].g << " " << Punkter[i].b << endl;
	}
    Export.close();

    
    Export.open(SpiralPoints);
    Export << "Total amount og points set at "<<1000<<" Values for the spiral in Order: x y z r g b" << endl;
    float Angle = 0.1f;
    for (float i = lower; i <= max; i += h)
    {
        Angle += 0.01f;
        float x = i * h;


        Spiral.push_back(Vertex{ SpiralX(x, Angle) / 10,SpiralY(x, Angle) / 10,Angle / 10,1,0,1 });

    }
    for(int i = 0; i < Spiral.size(); i++)
    {
        Export << Spiral[i].x << " " << Spiral[i].y << " " << Spiral[i].z << " " << Spiral[i].r << " " << Spiral[i].g << " " << Spiral[i].b << endl;
    
    }
    Export.close();


    Export.open(XYPlanePoints);
    for(float X = lower ; X <= max ; X ++ )
    {
        for (float Y = lower; Y <= max; Y ++)
        {
            XYPlanevector.push_back(Vertex{ X,Y,XYPlane(X,Y) ,0,1,1});
        }
    }

    Export << "Total amount og points set at " << 1000 << " Values for the spiral in Order: x y z" << endl;
    for (int i = 0; i <= 120; i++)
    {
        Export << XYPlanevector[i].x << " " << XYPlanevector[i].y << " " << XYPlanevector[i].z << endl;
    }
    Export.close();


    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Spiral.size()*sizeof(Vertex), Spiral.data(), GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    // 4. then set the vertex attributes pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // apply rotation around the z-axis
      //  float speed = glfwGetTime(); // time-based rotation
      //  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), speed, glm::vec3(1.0f, 0.5f, 0.5f));
       // glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(rotation));
      
        glEnable(GL_LINE_SMOOTH);
        glDrawArrays(GL_LINE_STRIP, 0, Spiral.size());

      glfwSwapBuffers(window);
    	glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        std::cout << "A key pressed ..." << std::endl;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    std::cout << " windows resized with " << width << " Height " << height << std::endl;
}