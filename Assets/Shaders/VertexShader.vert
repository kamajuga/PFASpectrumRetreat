 #version 330 core

layout(location = 0) in vec2 aUV;
layout(location = 1) in vec3 aPosition;
layout(location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 v_NormalMatrix;
uniform mat4 v_MVP;
uniform mat4 v_Model;	

void main()
{
	FragPos = vec3(v_Model * vec4(aPosition, 1.0));

	gl_Position = v_MVP  * vec4(aPosition, 1.0);
	texCoord = aUV;
	Normal = normalize((v_NormalMatrix * vec4(aNormal, 0.0)).xyz);;
}