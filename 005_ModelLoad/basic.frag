in vec2 uv;
out vec3 color;
uniform sampler2D texSampler;

void main(void)
{
   color = texture2D(texSampler, uv).rgb;
}
