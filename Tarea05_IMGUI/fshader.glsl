#version 430 core

//layout(location=0) out vec4 color;
uniform vec3 colorxd;
// uniform mat4 transformation_matrix;

void main() {
	gl_FragColor.rgb = colorxd;
    gl_FragColor.a = 1.0f; 

}