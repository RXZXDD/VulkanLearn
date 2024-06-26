#version 450
layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;


layout(push_constant) uniform Push {
	mat4 modelMatrix;//projection * view * model
	mat3 normalMatrix;
}push;

void main() {
	//outColor = vec4(1.0, 0.0, 0.0, 1.0);
	outColor = vec4(fragColor * vec3(1.0, 0.0,0.0), 1.0);
}