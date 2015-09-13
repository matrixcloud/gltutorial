#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 color;
uniform mat4 mvp;

out vec3 fragColor;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0f);
    fragColor = color;
}

