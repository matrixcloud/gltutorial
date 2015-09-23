#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;
uniform vec3 lightPosition_worldspace;

out vec2 uv;
out vec3 normal_cameraspace;
out vec3 lightDir_cameraspace;
out vec3 position_worldspace;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0f);
    uv = vertexUV;

    normal_cameraspace = vec4(v * m * vec4(vertexNormal_modelspace, 0)).xyz;

    vec3 lightPosition_cameraspace = vec4(v * vec4(lightPosition_worldspace, 1)).xyz;
    vec3 vertexPosition_cameraspace = vec4(v * m * vec4(vertexNormal_modelspace, 1)).xyz;
    vec3 eyeDir_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;
    lightDir_cameraspace = lightPosition_cameraspace + eyeDir_cameraspace;

    position_worldspace = vec4(m * vec4(vertexPosition_modelspace, 1)).xyz;
}
