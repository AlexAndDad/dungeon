#version 330

/*

texture:

   <----- offset ---- ><---------------------- size -------------------->
   <------------------------- glyph render area ------------------------>
*/

/// The x,y position to draw the glyph (bottom left)
uniform vec2 position;

/// The scaling factor
uniform vec2 scale;

/// The size of the bitmap in texels
uniform vec2 size;

/// The offset within the glyph of the first texel, in texels
uniform vec2 offset;

/// The co-ordinates within the texture of each vertex of the glyph
/// The shader is designed such that these will mark out the edges of the texture
layout (location = 0) in vec2 coord;

/// Emit the texture coordinate to the fragment shader
out vec2 texcoord;

void main(void) {
  gl_Position = vec4((offset.x + size.x * coord.x) * scale.x,
                     (-(-position.y - offset.y) - size.y * coord.y) * scale.y,
                      -0.1, 1);
  texcoord = coord;
}
