in vec2 uv;
in vec3 normal_cameraspace;
in vec3 lightDir_cameraspace;
in vec3 position_worldspace;

out vec3 color;
uniform sampler2D texSampler;
uniform vec3 lightPosition_worldspace;

void main(void)
{
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50.0f;
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightDir_cameraspace);

    vec3 materialDiffuseColor = texture2D(texSampler, uv).rgb;
    float cosTheta = clamp(dot(n, l), 0, 1);
    float distance = length(lightPosition_worldspace - position_worldspace);
    color = materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance);
}
