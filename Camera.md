## TODO：0.0.1 Camera：^[1]

### <b>WORKFLOW</b>

### <b>TIP</b>
#### *Camera Class*
 - position
 - orientation (horAngle/verAngle)
 - zoom
 - max/min viewing distance
 - aspect ratio of viewport/window

##### *orientation (lookAt)*
 - glm::rotate
 - glm::radians
 - glm::normalize
 - asinf
 - MaxVerticalAngle

##### *combine all attributes into matrix*
 - projection * view

##### *camera matrix is combination of four transformation*
 - glm::translate(glm::mat4(), -pos)
 - glm::rotate( , glm::radians(verAngle), glm::vec3(1,0,0))
 - glm::rotate( , glm::radians(horAngle), glm::vec3(0,1,0))
 - glm::perspective(glm::radians(fov), viewportAspectRatio, nearPlane, farPlane)

##### *camera integrating*
 - remove the projection shader variable
 - gProgram->setUniform("camera", gCamera.matrix());

#### *update the scene*
##### *keyboard input*
 - camera move pos, check 'W','A','S','D'
 - camera up and down, check 'E','F'

##### *invert camera orientation matrix*
 - directionFacing
 - glm::inverse(orientation() * glm::vec4(0, 0, -1, -1));
 - directionRight
 - glm::inverse(orientation() * glm::vec4(1, 0, 0, 1));
 - directionUp
 - glm::inverse(orientation() * glm::vec4(0, 1, 0, 1));

##### *mouse wheel*
 - glfwSetScrollCallback(gWindow, OnScroll);
 - gCamera.setFieldOfView

##### *mouse input*
 - glfwGetCusorPos
 - gCamera.offsetOrientation

---
^[1] : [Cameras, Vectors & Input](http://tomdalling.com/blog/modern-opengl/04-cameras-vectors-and-input/)

### <b>Q&A</b>
 - view = orientation * glm::translate(glm::mat4() - _pos) ?
 - directionFacing/Right/Up lookAt(verAngle/horAngle) in orientaion?
 - offsetOrientation ?