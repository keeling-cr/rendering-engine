## Nica

### 背景
- 跨平台游戏引擎
- 丰富物料
- 高性能


### TODO:
1. v8 memory leak
    确定先每个fucntion 没有加 handlescope 是否合理
2. debug 调试crash
3. binding 代码优化
    - remove wrapper info
    - optimize wrappable object
4. jsruntime 提供生命周期钩子，确定每个时期需要做的事情
5. persistent 对象提供垃圾回收机制
6. 统一 v8object 和 wrapper 对象
7. functiontemplate builder constructur parent class provide base static constructor
8. v8 头文件引用规范
9. 支持 messagepump

## DEMO 形态

1. 支持 texture/renderbuffer/framebuffer/shader
2. 简单画一个三角形

### OpenglES API:
1. glCreateShader
2. glShaderSource
3. glCompileShader
4. glGetShaderiv
5. glGetShaderInfoLog
6. glCreateProgram
7. glAttachShader
8. glLinkProgram
9. glGetProgramiv
10. glGetProgramInfoLog
11. glDeleteShader
12. glGetAttribLocation
13. glClearColor
14. glViewport
15. glGenBuffers
16. glBindBuffer
17. glBufferData
18. glVertexAttribPointer
19. glEnableVertexAttribArray
20. glUseProgram
21. glDrawArrays
22. glDeleteBuffers

### WebGL API:
1. createBuffer
2. bindBuffer
3. bufferData
4. createShader
5. shaderSource
6. compileShader
7. createProgram
8. attachShader
9. linkProgram
10. useProgram
11. enableVertexAttribArray
12. clearColor
13. enable
14. viewport
15. drawElements

### WebGL data
1. context
    - ARRAY_BUFFER
    - ELEMENT_ARRAY_BUFFER
    - STATIC_DRAW
    - VERTEX_SHADER
    - FRAGMENT_SHADER
    - FLOAT
    - DEPTH_TEST
    - COLOR_BUFFER_BIT
    - TRIANGLES
    - UNSIGNED_SHORT

### Angle
1. EGLWindow
2. egl_autogen
    - egl_get_config
    - egl_get_current_display
3. gles_autogen
4. library_loader
