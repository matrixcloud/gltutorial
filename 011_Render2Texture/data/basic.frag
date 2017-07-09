in vec2 uv;
in vec3 normal_cameraspace;
in vec3 lightDir_cameraspace;
in vec3 position_worldspace;
in vec3 eyeDir_cameraspace;

out vec3 color;
uniform sampler2D texSampler;
uniform vec3 lightPosition_worldspace;

void main(void)
{
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50.0f;
    vec3 materialDiffuseColor = texture2D(texSampler, uv).rgb;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3, 0.3, 0.3);

    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightDir_cameraspace);

    float cosTheta = clamp(dot(n, l), 0, 1);
    float distance = length(lightPosition_worldspace - position_worldspace);

    //计算镜面反射
    vec3 e = normalize(eyeDir_cameraspace);
    vec3 r = reflect(-l, n);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    color = materialAmbientColor +
            materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
            materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
}
