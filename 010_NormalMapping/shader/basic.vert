#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;


out vec2 uv;
out vec3 position_worldspace;
out vec3 lightDir_cameraspace;
out vec3 eyeDir_cameraspace;

out vec3 lightDir_tangentspace;
out vec3 eyeDir_tangentspace;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;
uniform mat3 mv3x3;
uniform vec3 lightPosition_worldspace;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0f);
    uv = vertexUV;
    position_worldspace = vec4(m * vec4(vertexPosition_modelspace, 1)).xyz;


    vec3 lightPosition_cameraspace = vec4(v * vec4(lightPosition_worldspace, 1)).xyz;
    vec3 vertexPosition_cameraspace = vec4(v * m * vec4(vertexNormal_modelspace, 1)).xyz;
    eyeDir_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;
    lightDir_cameraspace = lightPosition_cameraspace + eyeDir_cameraspace;

    // model to camera = ModelView
    vec3 vertexTangent_cameraspace = mv3x3 * vertexTangent_modelspace;
    vec3 vertexBitangent_cameraspace = mv3x3 * vertexBitangent_modelspace;
    vec3 vertexNormal_cameraspace = mv3x3 * vertexNormal_modelspace;

    mat3 TBN = transpose(mat3(
            vertexTangent_cameraspace,
            vertexBitangent_cameraspace,
            vertexNormal_cameraspace
    )); // You can use dot products instead of building this matrix and transposing it. See References for details.

    lightDir_tangentspace = TBN * lightDir_cameraspace;
    eyeDir_tangentspace =  TBN * eyeDir_cameraspace;
}
