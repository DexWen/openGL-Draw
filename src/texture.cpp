#include<iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#pragma comment(lib,"FreeImage.lib")

// Other Libs
#include "Shader.h"
#include "FreeImage.h"

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window 尺寸
const GLuint WIDTH = 800, HEIGHT = 600;
int main()
{
    //I初始化GLFW
    glfwInit();
    FreeImage_Initialise(TRUE);//初始化FreeImage

    //设置全部GLFW要求的设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    //创造一个GLFWwindow 项目 我们可以使用GLFW的功能
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    //设置回调函数的注册
    glfwSetKeyCallback(window, key_callback);

    //glew设置为true  让glew知道使用现在的方法去恢复指针和扩展
    glewExperimental = GL_TRUE;
    //初始化glew 去设置opengl功能指针
    glewInit();
    //定义视图的尺寸
    glViewport(0, 0, WIDTH, HEIGHT);
    //定义和编译我们的着色器程序
    Shader ourShader("F:/work/myGit/openGL-Draw/src/shader/shader.vs", "F:/work/myGit/openGL-Draw/src/shader/shader.frag");
    //设置顶点数据（和缓冲）和属性指针
    GLfloat vertices[] = {
        // 位置          // 颜色           // 纹理坐标
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // 解绑 VAO

    // Load image, create texture and generate mipmaps 加载图像并且创建纹理和产生编码
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    int width, height;
    FIBITMAP *dib(0);

    // unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    //1 获取图片格式
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType("F:/work/myGit/openGL-Draw/src/material/gtaBg.jpg", 0);

    //2 加载图片

    if(FreeImage_FIFSupportsReading(fifmt))
        dib = FreeImage_Load(fifmt, "F:/work/myGit/openGL-Draw/src/material/gtaBg.jpg",0);
    printf("bit: %d\n", FreeImage_GetBPP(dib));//灰度
    printf("type: %d\n",FreeImage_GetImageType(dib));//返回类型
    printf("bit: %d\n",FreeImage_GetColorsUsed(dib));//调色板的大小
    printf("bit: %d\n",FreeImage_GetDIBSize(dib));//大小
    //if the image failed to load, return failure
    if(!dib)
        cout<<55<<endl;
    //3 转化为rgb 24色
    dib = FreeImage_ConvertTo24Bits(dib);

    //4 获取数据指针
    BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);


    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    cout<<"width:"<<width<<endl;
    cout<<"height:"<<height<<endl;


    // 加载和创建纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // 设置为2D纹理
    // Set the texture wrapping parameters  设置纹理包装参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters  设置纹理过滤的参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    FreeImage_Unload(dib);
    FreeImage_DeInitialise();
    glGenerateMipmap(GL_TEXTURE_2D);



    glBindTexture(GL_TEXTURE_2D, 0); // 解除绑定的纹理

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render  渲染
        // Clear the colorbuffer 清理颜色缓存
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Bind Texture 绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture);

        // Activate shader 设置活动的着色器
        ourShader.Use();

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
