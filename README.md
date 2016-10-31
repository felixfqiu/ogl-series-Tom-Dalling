# Feng's HelloOglSeries.vcxproject

ref Article 1 (see below)

## [161026c] Workflow Explaining
###LOAD
 - glShaderSource(_object, 1, (const GLchar**)&code, NULL);
 - gProgram = new tdogl::Program(shaders); // "vertex-shader.txt" + "fragment-shader.txt"

####Vertex
 - glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

####Shader
 - in (vertex-shader.txt)
 - out (fragment-shader.txt)

#####顶点shader可以编写代码实现如下功能：^[1]
 - 使用模型视图矩阵以及投影矩阵进行顶点变换
 - 法线变换及归一化
 - 纹理坐标生成和变换
 - 逐顶点或逐像素光照计算
 - 颜色计算

#####片断处理器运行片断shader，这个单元可以进行如下操作：^[1]
 - 逐像素计算颜色和纹理坐标
 - 应用纹理
 - 雾化计算
 - 如果需要逐像素光照，可以用来计算法线

#####错误处理：^[2]
 - 调试shader是很困难的
 - 需要从InfoLog中找到更多的信息

###RENDER
 1. UseProgram(gProgram->object());
 2. BindVertextArray(gVAO);
 3. DrawArray(GL_TRIANGLES, 0, 3);

## [161026b] VAO / VBO in LoadTriangle 
 - gen / bind / enable / unbind
 - GLint attrib(const GLchar* attribName) const;

## [161026a] Program::Program(const std::vector<Shader>& shaders)

## [161026x] LoadShaderAndProgram 
 - shaderFromFile()
 - vertex-shader.txt / fragment-shader.txt

###          glInit
 - glfwInit / Window / Context
 - glewInit

###          Shader::Shader(const Shader& other)

^[1] : [【GLSL教程】（一）图形流水线](http://blog.csdn.net/racehorse/article/details/6593719)  
^[2] : [【GLSL教程】（二）在OpenGL中使用GLSL](http://blog.csdn.net/racehorse/article/details/6616256)

- - -
# [About Tom Dalling](http://tomdalling.com/)

## List of articles

 1. [Getting Started in Xcode, Visual C++, and Linux](http://tomdalling.com/blog/modern-opengl/01-getting-started-in-xcode-and-visual-cpp/)
 2. [Textures](http://tomdalling.com/blog/modern-opengl/02-textures/)
 3. [Matrices, Depth Buffering, Animation](http://tomdalling.com/blog/modern-opengl/03-matrices-depth-buffering-animation/)
 4. [Cameras, Vectors & Input](http://tomdalling.com/blog/modern-opengl/04-cameras-vectors-and-input/)
 5. [Model Assets & Instances](http://tomdalling.com/blog/modern-opengl/05-model-assets-and-instances/)
 6. [Diffuse Point Lighting](http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/)
 7. [More Lighting: Ambient, Specular, Attenuation, Gamma](http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/)
 8. [Even More Lighting: Directional Lights, Spotlights, & Multiple Lights](http://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights/)

# Ports & Related Resources

 - Migration notes for iOS with OpenGLES 2.0: http://whackylabs.com/rants/?p=769
 - A C# port of the articles: https://github.com/ZeronSix/opengl-series-csharp

# Credits

Thanks to Martin (SpartanJ) Golini for the Linux ports.

Thanks to Sidharth Juyal of [Whacky Labs](http://whackylabs.com/) for
the iOS ports.

Thanks to Vyacheslav (ZeronSix) Zeronov for the C# port.

# License

Licensed under the Apache License, Version 2.0. See LICENSE.txt.
