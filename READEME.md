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