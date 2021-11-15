#version 410 core

in vec3 vert_normal_view;
in vec3 vert_color;

uniform float specular_coef;
uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform float window_width;
uniform float window_height;
uniform mat4 projection_matrix;
uniform vec4 light_pos;
uniform vec3 light_col;


out vec4 out_color;

vec3 VSPositionFromDepth(vec2 texcoord, float z)
{
    // Get x/w and y/w from the viewport position
    vec4 positionNDC = vec4(2 * vec3(texcoord, z) - 1, 1.f);
    // Transform by the inverse projection matrix
    vec4 positionVS = inverse(projection_matrix) * positionNDC;
    // Divide by w to get the view-space position
    return positionVS.xyz / positionVS.w;  
}

vec3 shade(vec3 N, vec3 L, vec3 V, 
           vec3 color, float Ka, float Kd, float Ks,
           vec3 lightCol, float shininess){

    vec3 final_color = color * Ka * lightCol;	//ambient

    float lambertTerm = dot(N,L);		//lambert

    if(lambertTerm > 0.0) {
        final_color += color * Kd * lambertTerm * lightCol; 	//diffuse

        vec3 R = reflect(-L,N);
        float specular = pow(max(dot(R,V), 0.0), shininess);
        final_color +=  Ks * lightCol * specular;	//specular
    }

    return final_color;
}

void main(){
    vec2 texcoord = gl_FragCoord.xy / vec2(window_width, window_height);

    vec4 tex_color = texture(color, texcoord);
    vec4 tex_normal = texture(normal, texcoord);
    vec4 tex_depth = texture(depth, texcoord);


    vec3 positionFromDepth = VSPositionFromDepth(texcoord, tex_depth.x);
    vec3 l = vec3(light_pos) - positionFromDepth;
    out_color.rgb = shade(normalize(tex_normal.xyz), normalize(l), -normalize(positionFromDepth),
                          tex_color.xyz, 0.2, 0.7, tex_color.a, light_col/max(1,dot(l,l)*0.05), 5);
    //out_color.rgb = positionFromDepth.rgb;
    out_color.a = 1;
}