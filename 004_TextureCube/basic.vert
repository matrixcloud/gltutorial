#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
uniform mat4 mvp;

out vec2 uv;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0f);
    uv = vertexUV;
}
