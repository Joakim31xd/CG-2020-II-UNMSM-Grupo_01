#version 430

// Input vertex data and color data 
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform mat4 view_matrix;
// Values that stay constant for the whole mesh.


// Output fragment data 
out vec3 fragmentColor;

void main()
{
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vertexPosition.x,vertexPosition.y,vertexPosition.z, 1.0);		
	fragmentColor = vertexColor;
}