
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float texture_size;

void main()
{
	vec2 uv = v_uv;
	vec4 color = texture2D(u_texture, uv) * 0.55;
	color += texture2D(u_texture, uv, 2.0) * 0.45;

	float average = (color.x + color.y + color.z) / 2.0;
	color = color * (average * 1.85);
	//color = vec4(average);
	
	float distance = 1.0 - length(uv - vec2(0.5,0.5));
	distance -= 0.1;
	color *= distance;
	color += 0.09;
	gl_FragColor = color;
	
	// Pintar en escala de grises;
	//float average = (color.x + color.y + color.z) / 3.0;
	//color = vec4(average);
	
	// Efecto borracho
	//uv.x = sin(u_time + uv.y * 20.0) * 0.1;
	//uv.y = sin(u_time + uv.x * 20.0) * 0.1;
}
