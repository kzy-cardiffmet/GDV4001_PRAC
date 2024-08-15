#version 450 core

layout (lines) in;
layout (line_strip, max_vertices = 4) out;

in GSPacket
{
	vec3 	n;
}inputVertex[];

out FSPacket
{
	vec4 c;
}outputVertex;

void main()
{
	vec4 p = vec4(0.0);
	vec3 normal = vec3(0.0);
	
	normal = inputVertex[0].n;
	p = (gl_in[0].gl_Position + gl_in[1].gl_Position)/2.0;
	gl_Position = p;
	outputVertex.c = vec4(0.0, 1.0, 0.0, 1.0);
	EmitVertex();
	
	normal 	= vec3(normalize(normal) * 1.0);
	gl_Position = p - vec4(vec3(normal), 0.0);
	outputVertex.c = vec4(0.0, 1.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();

	p = gl_in[0].gl_Position;
	gl_Position = p;
	outputVertex.c = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();

	p = gl_in[1].gl_Position;
	gl_Position = p;
	outputVertex.c = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();
	
}

