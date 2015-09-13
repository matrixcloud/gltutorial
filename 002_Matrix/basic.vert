#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0f);
}

