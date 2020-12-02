
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture; //setting sampler

void main()
{
    // texture is sampler function ourTexture is sampler, second is texture coordination
    color = texture(ourTexture, TexCoord);
}