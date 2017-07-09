in vec2 uv;
in vec3 position_worldspace;
in vec3 lightDir_cameraspace;
in vec3 eyeDir_cameraspace;

in vec3 lightDir_tangentspace;
in vec3 eyeDir_tangentspace;

out vec3 color;


uniform sampler2D diffuseTextureSampler;
uniform sampler2D normalTextureSampler;
uniform sampler2D specularTextureSampler;
uniform mat4 v;
uniform mat4 m;
uniform mat3 mv3x3;
uniform vec3 lightPosition_worldspace;

void main(void)
{
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50.0f;


    vec3 materialDiffuseColor = texture2D(diffuseTextureSampler, uv).rgb;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = texture2D(specularTextureSampler, uv).rgb * 0.3;

    vec3 textureNormal_tangentspace = normalize(texture2D(normalTextureSampler, vec2(uv.x-uv.y)).rgb*2.0 - 1.0);

    float distance = length(lightPosition_worldspace - position_worldspace);
    vec3 n = textureNormal_tangentspace;
    vec3 l = normalize(lightDir_tangentspace);

    float cosTheta = clamp(dot(n, l), 0, 1);

    //计算镜面反射
    vec3 e = normalize(eyeDir_tangentspace);
    vec3 r = reflect(-l, n);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    color.rgb = materialAmbientColor +
            materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
            materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
}
