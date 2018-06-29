
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_detail_texture;
uniform float u_time;
uniform vec3 u_camera_position;

void main()
{
	float distance = length(u_camera_position - v_world_position);
	float fogMaxDistance = 9000;
	float fogMinDistance = 100;
	float fogDiferenceDistance = fogMaxDistance - distance;
	float fogDiferenceMaxMin = fogMaxDistance - fogMinDistance;
	float fogFactor = clamp(1.0 - (fogDiferenceDistance / fogDiferenceMaxMin) , 0.0, 1.0);
	fogFactor = pow(fogFactor, 3);

	float red = 161.0 / 255.0;
	float green = 192.0 / 255.0;
	float blue = 203.0 / 255.0;
	vec4 fogColor = vec4(red, green, blue, 0.70);

	vec2 uv = v_uv;	
	vec4 color = u_color * texture2D( u_texture, uv );

	color = mix(color, fogColor, fogFactor);
	
	gl_FragColor = color;
}
