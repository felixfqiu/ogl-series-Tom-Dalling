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

---
^[1] : [【GLSL教程】（一）图形流水线](http://blog.csdn.net/racehorse/article/details/6593719)  
^[2] : [【GLSL教程】（二）在OpenGL中使用GLSL](http://blog.csdn.net/racehorse/article/details/6616256)