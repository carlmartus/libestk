#version 100
precision mediump float;

attribute vec2 in_vertex;
attribute vec2 in_uv;

varying vec2 va_uv;

void main()
{
	va_uv = in_uv;
	gl_Position = vec4(in_vertex, 0.0, 1.0);
}

