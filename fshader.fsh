struct materialProperty
{
    vec3 diffuseColor;
};

uniform sampler2D u_diffuseMap;
uniform materialProperty u_materialProperty;
uniform bool u_isUsingDiffuseMap;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 diffMatColor = texture2D(u_diffuseMap, v_texcoord);
    float ambientFactor = 0.5;
    float edgeFactor = 0.2; // Change this value to see the effect

    if (u_isUsingDiffuseMap == false)
        diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);

    vec4 ambientColor = diffMatColor * ambientFactor;
    resultColor += ambientColor;

    // Calculate the light direction
    float edge = max(dot(normalize(v_normal), normalize(-v_position.xyz)), 0.0);
    vec4 edgeColor = vec4(1.0, 1.0, 1.0, 1.0) * edge * edgeFactor;
    resultColor += edgeColor;

    gl_FragColor = resultColor;
}
