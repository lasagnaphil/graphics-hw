#version 330 core

in vec3 Barycentric;

void main() {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
/*
    if (any(lessThan(Barycentric, vec3(0.02)))) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
    */
}
