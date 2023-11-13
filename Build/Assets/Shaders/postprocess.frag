#version 430

#define INVERT_MASK			(1 << 0)
#define GRAYSCALE_MASK		(1 << 1)
#define COLORTINT_MASK		(1 << 2)
#define GRAIN_MASK			(1 << 3)
#define SCANLINE_MASK		(1 << 4)
#define RIPPLE_MASK         (1 << 5)



in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

//inversion/other effects
uniform float blend = 1;
//implemented with grayscale and for all shaders
uniform uint params = 0;
//grain
uniform float grainAmount; 
//color tint
uniform vec4 tint;
//scanLine
uniform float time;
uniform float scanLine; 
//water effect
uniform float rippleIntensity;

layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(vec4 color)
{
    // Inverts the color by subtracting the RGB values from 1. The alpha channel remains unchanged.
    return vec4(vec3(1) - color.rgb, color.a);
}
vec4 grayscale(in vec4 color)
{
    // Converts the color to grayscale using the luminance method, which accounts for human perception of different colors.
    float luminance = (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
    return vec4(vec3(luminance), color.a);
}
vec4 colortint(in vec4 color)
{
    // Applies a color tint to the input color by multiplying the RGB values with the RGB values of the tint color.
    // The alpha channel of the input color is preserved.
    return vec4(color.rgb * tint.rgb, color.a); 
}
float rand(vec2 co)
{
    // Generates a pseudo-random value based on a 2D coordinate using the sine function and dot product.
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
vec4 grain(float time)
{
   // Offsets the texture coordinates over time to create an animated noise effect.
   vec2 animatedCoord = ftexcoord + vec2(time * 0.1, time * 0.1); 
   // Generates noise based on the animated texture coordinates.
   float noise = rand(animatedCoord);
   // Returns the noise value as a grayscale color multiplied by the grain amount.
   return vec4(vec3(noise), 1.0) * grainAmount;
}


vec4 scanline(vec4 color, float yPos, float time)
{
    // lineStrength controls the visibility of the scanlines.
    float lineStrength = 2;
    // linesFrequency controls the density of the scanlines.
    float linesFrequency = 10.0;
    // movingYPos creates a vertical moving effect over time.
    float movingYPos = yPos + time;
    // scanlineEffect creates an oscillating pattern based on the y position.
    float scanlineEffect = abs(sin(movingYPos * linesFrequency * 3.14159));
    // Dim the color based on the scanline effect.
    vec3 dimmedColor = color.rgb * mix(1.0, lineStrength, scanlineEffect);
    return vec4(dimmedColor, color.a);
}
vec4 ripple(vec4 color, vec2 uv, float time)
{
    // Calculates the distance of each pixel from the center of the texture.
    float distance = length(uv - vec2(0.5, 0.5));
    // Modifies the texture coordinates based on a sine wave pattern to create a ripple effect.
    uv += sin(distance * rippleIntensity - time) * 0.05;
    // Samples the texture with the modified coordinates to apply the ripple effect.
    return texture(screenTexture, uv);
}


void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	// 0001 <- mask
	// 0001 <- params

	if (bool(params & INVERT_MASK))		postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK))	postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK))	postprocess = colortint(postprocess);
	if (bool(params & GRAIN_MASK))      postprocess += grain(time);
	if (bool(params & SCANLINE_MASK))   postprocess = scanline(postprocess, ftexcoord.y, time); 
    if (bool(params & RIPPLE_MASK))     postprocess = ripple(postprocess, ftexcoord, time);

    


	ocolor = mix(basecolor, postprocess, blend);
}
