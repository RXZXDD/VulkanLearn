#version 450
layout (location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;//projection * view * model
	mat3 normalMatrix;
}push;

void main() {
	//outColor = vec4(1.0, 0.0, 0.0, 1.0);
	vec3 directionToLight = normalize(ubo.lightPosition - fragPosWorld.xyz);
	float attenuation = 1.0 / dot(directionToLight,directionToLight);

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 diffuseLight = max(dot(normalize(fragNormalWorld), directionToLight),0) * lightColor;

	//float lightIntensity = AMBIENT + max(dot(normalize(fragNormalWorld), DIRECTION_TO_LIGHT),0);
	vec3 fragColor = (diffuseLight + ambientLight) * lightColor;

	outColor = vec4(fragColor, 1.0);
}