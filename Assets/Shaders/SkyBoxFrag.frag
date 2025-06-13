#version 330 core
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{    
    //FragColor = vec4(abs(normalize(TexCoords)), 1.0);
    FragColor = texture(skybox, TexCoords);
}