#version 120
#ifdef GL_ES
precision mediump float;
#endif
#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);
}

float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size-vec2(0.001),
                        _st);
    uv -= smoothstep(_size,
                    _size+vec2(0.001),
                    vec2(1.0)-_st);
    return uv.x*uv.y*uv.x;
}

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
}

float triangle(in vec2 st, in int N){
    float d = 0.0;
    
  // Remap the space to -1. to 1.
  st = st *2.000-1.;

  // Angle and radius from the current pixel
  float a = atan(st.x,st.y)+PI;
  float r = TWO_PI/float(N);

  // Shaping function that modulate the distance
  d = cos(floor(.5+a/r)*r-a)*length(st);
    return d;

}

float cross(in vec2 _st, float _size){
    
    return  box(_st, vec2(_size,_size/4.)) +
            box(_st, vec2(_size,_size/4.)) +
            box(_st, vec2(_size/4.,_size)) +
        	box(_st, vec2(_size/4.,_size)) +
        
        	box(_st, vec2(_size,_size/2.)) +
        	box(_st, vec2(_size/2.,_size)) +
        
        	triangle(_st, 3)/1.3+
        
        	circle(_st, 0.25)+
    		circle(_st, 0.1);
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    // To move the cross we move the space
    vec2 translate = vec2(-cos(u_time),-cos(u_time));
    st += translate*0.35;
    st -= vec2(0.5);
	st = rotate2d( 1.0/cos(u_time)*PI ) * st;
    st += vec2(0.5);
    st -= vec2(0.5);
    st = scale( vec2(sin(u_time)+1.0) ) * st;
    st += vec2(0.5);
    // Show the coordinates of the space on the background
     //color = vec3(st.x,st.y,1.0);
    color = vec3(1.000,0.207,0.092);

    // Add the shape on the foreground
    color += vec3(cross(st,0.35));

    gl_FragColor = vec4(color,0.9);
}