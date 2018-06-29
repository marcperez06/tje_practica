varying vec2 v_uv;
uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
	vec4 color = texture2D(u_texture, v_uv);
	gl_FragColor = u_color * color;
}
