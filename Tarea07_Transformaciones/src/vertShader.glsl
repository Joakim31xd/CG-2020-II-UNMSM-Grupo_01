#version 410

// Input vertex data and color data 
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
uniform mat4 model;

// Values that stay constant for the whole mesh.


// Output fragment data 
out vec3 fragmentColor;

void main()
{
	gl_Position = model * vec4(1 * vertexPosition.x, 1 * vertexPosition.y, vertexPosition.z, 1.0);		
	fragmentColor = vertexColor;
}