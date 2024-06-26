attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec3 a_normal;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
void main(void)
{
    mat4 modelViewMatrix = u_viewMatrix * u_modelMatrix;

    gl_Position = u_projectionMatrix * modelViewMatrix * a_position;

    v_texcoord = a_texcoord;
    v_normal = normalize(vec3(modelViewMatrix * vec4(a_normal, 0.0)));
    v_position = modelViewMatrix * a_position;
}
