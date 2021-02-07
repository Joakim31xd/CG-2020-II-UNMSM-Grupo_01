#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute
layout (location = 2) in vec3 normal; // normal_position attribute

out vec2 texCoords;
// To be passed to fragment shader
out vec3 norm;
out vec3 fragment_position;
out vec3 diffuse_light;

uniform mat4 model;
uniform mat4 modelview;
uniform mat4 projection_perspective;

// matrices required for lighting
uniform vec3 lamp_pos; 
uniform vec3 light_color; 

void main()
{
	gl_Position = projection_perspective * modelview * vec4(position, 1.0);
	texCoords = vec2(tex_coords.x, 1.0-tex_coords.y); // to invert the texture

	fragment_position = vec3(model * vec4(position, 1.0f));
	norm = normalize(vec3(model * vec4(normal, 1.0f))); // norm should not be calculated like this if there is non-uniform scaling or translation.

	// Diffused Lighting
	vec3 light_vector = normalize(lamp_pos - fragment_position);
	float dist = distance(fragment_position, lamp_pos);
	float diffuse_value = 1/pow(dist, 2) * max(dot(norm, light_vector), 0.0f);	
	diffuse_light = diffuse_value * light_color;
}