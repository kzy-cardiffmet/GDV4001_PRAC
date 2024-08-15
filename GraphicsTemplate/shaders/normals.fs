#version 450 core

layout(binding = 0) uniform sampler2D ColTexture;

in FSPacket
{
	vec4 c;
}inputVertex;

out vec4 FragColor; // Color that will be used for the fragment

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	// green 
    	FragColor = vec4(inputVertex.c);
}