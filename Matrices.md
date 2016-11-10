## TODO：0.0.1 Matrices：^[1]

### <b>WORKFLOW</b>

### <b>TIP</b>
#### *Make Object*
 - <s>GL_QUADS</s>
 - glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

#### *Render Cube*

##### *LoadShader()*
 - gProgram->setUniform("camera", camera);

##### *vertex-shader.txt*
 - uniform mat4 camera;
 - gl_Position = camera * vec4(vert, 1);

##### *Projection Matrix*
 - uniform mat4 projection;
 - Unlike normal multiplication, order is important in matrix multiplication.

##### *Depth Buffering*
 - back sides of the cube rendered over the top of front sides

##### *OpenGL settings*
 - glEnable(GL_DEPTH_TEST)
 - glDepthFunc(GL_LESS)

##### *Clear depth buffer*
 - every time we render new frame
 - glClear( | GL_DEPTH_BUFFER_BIT)

#### *Rotate*
 - each object has a model matrix
 - whole scene same camera and projection matrices
 - gl_Position = projection * camera * model * vec4(vert, 1);

---
^[1] : <http://www.tomdalling.com/blog/modern-opengl/03-matrices-depth-buffering-animation/>

### <b>Q&A</b>
 - const glm::vec2 SCREEN_SIZE(340, 770)，为什么显示不全？
 - camera = glm::lookAt((6,6,6), (0,0,0), (0,1,0)) ，为什么显示不全？
 - OpenGL Shader error 1282 (invalid operation) GL_INVALID_VALUE or GL_INVALID_OPERATION