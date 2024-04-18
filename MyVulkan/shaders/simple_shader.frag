#version 450
layout (location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

struct PointLight {
    vec4 position;
    vec4 color;
};


layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;//projection * view * model
	mat3 normalMatrix;
}push;

void main() {

	vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 surfaceNormal = normalize(fragNormalWorld);

	for(int i = 0; i< ubo.numLights; i++) {
		PointLight light = ubo.pointLights[i];
		vec3 directionToLight = normalize(light.position.xyz - fragPosWorld.xyz);
		float attenuation = 1.0 / dot(directionToLight,directionToLight);
		float cosAngIncidence = max(dot(surfaceNormal, directionToLight),0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}



	vec3 fragColor = diffuseLight * fragColor;

	outColor = vec4(fragColor, 1.0);
}