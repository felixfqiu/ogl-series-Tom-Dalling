## TODO：0.0.1 Textures：^[1]

### <b>WORKFLOW</b>
 1. LoadTexture Bitmap.h -> Texture.h -> Prgram.h
 2. vertex-shader.txt -> fragment-shader.txt
 3. Render gProgram->setUniform
 4. Render glActiveTexture/glBindTexture

### <b>TIP</b>
#### Render
 - bind texture and set "tex" uniform in fragment shader
 - between bind program and VAO ?

#### Creation
 - 2D images, bitmap Class ?
 - width/height should be a power of two

#### Coordinates
 - vertex-shader.txt
 - fragment-shader.txt
 - "tex"

#### Image Units
 - <s>just send texture to shader</s>
 - bind texture to a texture unit
 - then send <b>index of the texture unit</b> to shader

### <b>Q&A</b>
 - glActiveTexture(GL_TEXTURE); glActiveTexture(GL_TEXTURE0);
 - glUniform1d(uniform("tex"), v0);

---
^[1] : <http://tomdalling.com/blog/modern-opengl/02-textures/>