# 场景八叉树和层次Z-buffer

## 介绍和背景

去除场景中看不见的物体有个更现代的学名叫**剔除**（Occlusion），为此我没有特化光栅化管线，
而是在相对完整的传统光栅化管线之外通过插件形式实现各项功能，也方便测试改进的效率。

### 目的和内容

- 实现了简单软光栅流水线
- 实现基础扫描线Zbuffer
- 实现层次Z-Buffer
- 实现基于场景八叉树的视锥剔除和层次Z-Buffer剔除

## 算法和实现

### 光栅化管线

框架基于如下图所示的光栅化管线。

![Rasterization](imgs/raster_pipeline.png)

在着色阶段为了

### 光栅化算法

在光栅阶段可以选择不同的光栅化算法，在运行时切换。

#### GroundTruth 重心坐标

遍历三角形包围盒内像素，计算重心坐标。

#### 扫描线Z-buffer算法

将三角形分为上下两半，减少在三角形之外的像素点判断。
封装了Bresenham画线算法，使X/Y方向上坐标都是整数，尽可能避免浮点数误差。

### 层次Z Buffer

类似于Mipmap的做法，对深度图生成一系列LOD的保守深度值。

由于层次较多，实现的层次Z buffer单纯用于查询和剔除，不支持实时更新。因为对于宽为1024的图像来说，层次Z buffer的细节层数高达10层，实时更新引入巨大的额外开销。所以整体的做法类似UE的HZB，利用回读的上一帧深度信息建立HieZ。

> 当然相机位置的变换会使上一帧的深度信息过时，所以在变换时会出现一些黑影。
> 这通常可以通过重投影（reprojection）和图像空间滤波处理

在剔除的粒度上，管线中内置了一个层次Z Buffer用于视口空间下三角形的剔除。而在后续的八叉树节点也可以进行submesh粒度的剔除。

![hiez](imgs/hiez.png)

### 八叉树

#### 八叉树的建立

在加载场景时，对以submesh为单位的物体包围盒建立场景八叉树，所有的submesh会存储在八叉树的叶子节点中，并且所有包围盒有相交部分的叶子节点都会存储一份拷贝，而不是存储在中间节点中。这样虽然牺牲了一定的空间复杂度，但是可以在剔除时更有效率。

```c++
    auto insert(OctreeNode *node, NodeData const &data, AABB const &bbx, OctreeConfig const &config) -> void
    {
        node->is_empty = false;
        if (node->is_leaf)
        {
            if (node->depth < config.max_depth)
            {
                split(node);
                node->is_leaf = false;
            }
            else
            {
                node->data.push_back(data);
                return;
            }
        }
        for (int i = 0; i < 8; i++)
            if (node->children[i]->bbx.intersect(bbx))
            {
                insert(node->children[i], data, bbx, config);
            }
    }
```

#### 八叉树视锥剔除和HieZ剔除

对于实时场景，视锥剔除是加速场景渲染的重要一步。

遍历八叉树，将AABB在模型空间变换后与相机的视锥测试。对相交的叶子节点内的submesh结点加入drawcall。

基于回传的层次Zbuffer，可以对八叉树节点进行深度剔除，这在大尺度遮挡的场景会比较有用（例如墙壁）。


## 实验与结果

### 测试环境

- 操作系统：Windows 11
- CPU：Ultra 7 155H
- 编译器：MSVC

## 总结

