
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;//设置采样器
void main()
{
    //texture是采样函数第一个参数是纹理采样器。第二个参数是纹理坐标
    color = texture(ourTexture, TexCoord);
}