attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;

uniform vec3 u_camera_pos;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{		
	//calcule the vertex in object space
	v_position = a_vertex;

	//store the color in the varying var to use it from the pixel shader
	v_color = a_color;

	//store the texture coordinates
	v_uv = a_uv;

	//calcule the position of the vertex using the matrices
	gl_Position = v_position;
}