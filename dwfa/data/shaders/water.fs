
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_extra_texture;
uniform float u_time;
uniform vec3 u_camera_pos;

void main()
{

	float time = u_time * 0.9;
	
	vec2 uv = v_uv;
	uv = v_world_position.xz * 0.003;
	//uv.x += sin(u_time + (uv.x)) * 0.05; 
	//uv.x += u_time * 0.5;
	
	vec2 uvDeltaMoveX = uv + vec2(sin(time * 0.3) + 1, sin(time * 0.1));
	vec2 uvDeltaMoveY = uv + vec2(sin(time * 0.1), sin(time * 0.3));

	vec4 color = texture2D(u_normal_texture, uv * 0.00001);
	
	//uv = v_world_position.xz * 0.001;
	//uv.y += u_time * 0.5; 

	vec3 N = normalize(v_normal);
	N = texture2D(u_normal_texture, uvDeltaMoveX).xzy * vec3(2.0) - vec3(0.055);
	N += texture2D(u_normal_texture, uvDeltaMoveY).xzy * vec3(2.0) - vec3(0.055);
	N = normalize(N);
	
	vec3 E = v_world_position - u_camera_pos;
	float E_distance = E.length();
	E = E / E_distance;
	E = normalize(E);
	
	//compute eye reflected vector
	vec3 R = reflect(E, N);
	
	//compute the yaw using arc tangent 
	float yaw = atan(R.x,R.z) / 6.28318531;
	yaw += -0.25; //in case the 0deg doesn�t match our Z axis, we can rotate 90 degrees

	//compute the pitch using arc sinus
	float pitch = asin(R.y) / 1.57079633; //0 means bottom, 1 means top

	//build the UV vector for hemisphere (in case pitch is negative, clamp it to 0)
	//vec2 uv_reflection = vec2(yaw, clamp(pitch, 0.0, 1.0) );
	vec2 uv_reflection = vec2(yaw, abs(pitch));

	//read the sky texture (ignoring mipmaps to avoid problems)
	vec4 sky_color = texture2DLod(u_extra_texture, uv_reflection, 0.0);
	//vec4 sky_color = texture2D(u_extra_texture, uv_reflection);

	float fresnel = clamp(1.0 - dot(E, N), 0.0, 1.0);
	
	color = (1 - fresnel) * color + (fresnel * sky_color);
	
	float distance = length(u_camera_pos - v_world_position);
	float fogMaxDistance = 200.0;
	float fogMinDistance = 900.0;
	float fogDiferenceDistance = fogMaxDistance - distance;
	float fogDiferenceMaxMin = fogMaxDistance - fogMinDistance;
	float fogFactor = clamp(1.0 - (fogDiferenceDistance / fogDiferenceMaxMin) , 0.0, 1.0);
	fogFactor = pow(fogFactor, 1.0);

	float red = 161.0 / 255.0;
	float green = 192.0 / 255.0;
	float blue = 203.0 / 255.0;
	vec4 fogColor = vec4(red, green, blue, 0.10);
	
	vec4 final_color = mix(color, fogColor, fogFactor);
	final_color = final_color - vec4(0.25, 0.25, 0.25, 0.0);
	final_color.a = 0.8;
	
	gl_FragColor = final_color * u_color;
}
