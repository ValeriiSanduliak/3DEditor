struct materialProperty
{
    vec3 diffuseColor;
};

uniform sampler2D u_diffuseMap;
uniform materialProperty u_materialProperty;
uniform bool u_isUsingDiffuseMap;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 diffMatColor = texture2D(u_diffuseMap, v_texcoord);

    if(u_isUsingDiffuseMap == false) diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
    vec4 diffColor = diffMatColor;
    resultColor += diffColor ;
    gl_FragColor = resultColor;
}
