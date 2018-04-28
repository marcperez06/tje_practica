
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform vec3 u_camera_position;

void main()
{
	float distance = length(u_camera_position - v_world_position);
	float fogFactor = clamp((1000 / distance), 0, 1);
	fogFactor = pow(fogFactor, 3);

	vec4 fogColor = vec4(1, 1, 1, 1);

	vec2 uv = v_uv;
	vec4 color = u_color * texture2D( u_texture, uv );

	//color = color * (fogFactor) + fogColor * (1 - fogFactor);
	color = mix(color, fogColor, 1-fogFactor);

	gl_FragColor = color;
}
