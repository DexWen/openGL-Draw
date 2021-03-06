﻿#include<iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#pragma comment(lib,"FreeImage.lib")

// Other Libs
#include <other/Shader.h>
#include <FI/FreeImage.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
using namespace std;
// Window 尺寸
const GLuint VIEW_WIDTH = 800, VIEW_HEIGHT = 600;
/*
  1. 测试最基本的加载图片和shader文件
  2. 测试片段着色器的 texture 混色
*/
int main(){
    //I初始化GLFW
    glfwInit();
	FreeImage_Initialise(TRUE);//初始化FreeImage

    //设置全部GLFW要求的设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    //创造一个GLFWwindow 项目 我们可以使用GLFW的功能
    GLFWwindow* window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, "Dex-View", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    //设置回调函数的注册
    glfwSetKeyCallback(window, key_callback);

    // glew设置为true 让glew知道使用现在的方法去恢复指针和扩展
    glewExperimental = GL_TRUE;
    // 初始化glew 去设置opengl功能指针
	glewInit();
    //定义视图的尺寸
    glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    //定义和编译我们的着色器程序
    Shader ourShader("E:/myGit/openGL-Draw/src/shader/shader.vs", "E:/myGit/openGL-Draw/src/shader/shader.frag");
    //设置顶点数据（和缓冲）和属性指针
    GLfloat vertices[] = {
        // 位置          // 颜色           // 纹理坐标
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
        1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left
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

    /*
    1. 第一个参数指定我们要配置的顶点属性。在顶点着色器中使用location = 0定义了position顶点属性的位置值,
    它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
    2. 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
    3. 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
    4. 定义我们是否希望数据被标准化(Normalize)。
        如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
        我们把它设置为GL_FALSE。
    5. 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
        由于下个组位置数据在3个GLfloat之后，我们把步长设置为3 * sizeof(GLfloat)。
        要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
        我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
        一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，
        (译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节)。
    6. 最后一个参数的类型是GLvoid*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。
       由于位置数据在数组的开头，所以这里是0。*/
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

    int width, height;

	//1 获取图片格式
	const char* imagePath = "E:/myGit/openGL-Draw/src/material/gtaBg2.jpg";
	FIBITMAP *dib(0);
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(imagePath, 0);
	//2 加载图片
	if (FreeImage_FIFSupportsReading(fifmt))
		dib = FreeImage_Load(fifmt, imagePath, 0);
	printf("bit: %d\n", FreeImage_GetBPP(dib));//灰度
	printf("type: %d\n", FreeImage_GetImageType(dib));//返回类型
	printf("bit: %d\n", FreeImage_GetColorsUsed(dib));//调色板的大小
	printf("bit: %d\n", FreeImage_GetDIBSize(dib));//大小
	  //if the image failed to load, return failure
	if (!dib) {
		printf("read Image Error");
		return 0;
	}
		
	//3 转化为rgb 24色
	dib = FreeImage_ConvertTo24Bits(dib);

	//4 获取数据指针
	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);


	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	cout << "width:" << width << endl;
	cout << "height:" << height << endl;

    // 加载和创建纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // 设置为2D纹理

    // glGetUniformLocation(ourShader.Program, "transform") 
    // 获取Shaddr的片段着色器或者顶点着色器里面的参数 "transform"

    // Set the texture wrapping parameters  设置纹理包装参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters  设置纹理过滤的参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* 
    1. 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理
        (任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
    2. 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
    3. 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
    4. 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
    6. 下个参数应该总是被设为0（历史遗留问题）。
    7. 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
    9. 最后一个参数是真正的图像数据。
    当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。
    目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）。
    或者，直接在生成纹理之后调用glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。*/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	//SOIL_free_image_data(image);
	FreeImage_Unload(dib);
	FreeImage_DeInitialise();
   
    // 假设我们有一个包含着上千物体的大房间，每个物体上都有纹理。有些物体会很远，
    // 但其纹理会拥有与近处物体同样高的分辨率。由于远处的物体可能只产生很少的片段
    // OpenGL从高分辨率纹理中为这些片段获取正确的颜色值就很困难，
    // 因为它需要对一个跨过纹理很大部分的片段只拾取一个纹理颜色。
    // 在小物体上这会产生不真实的感觉，更不用说对它们使用高分辨率纹理浪费内存的问题了。
    // OpenGL使用一种叫做多级渐远纹理(Mipmap)的概念来解决这个问题，
    // 它简单来说就是一系列的纹理图像，后一个纹理图像是前一个的二分之一。
    // 多级渐远纹理背后的理念很简单：距观察者的距离超过一定的阈值，OpenGL会使用不同的多级渐远纹理，
    // 即最适合物体的距离的那个。由于距离远，解析度不高也不会被用户注意到。
    // 同时，多级渐远纹理另一加分之处是它的性能非常好。让我们看一下多级渐远纹理是什么样子的：
     
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
