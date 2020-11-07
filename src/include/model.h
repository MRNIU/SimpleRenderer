
// This file is a part of Simple-XX/SimpleRenderer
// (https://github.com/Simple-XX/SimpleRenderer).
//
// model.h for Simple-XX/SimpleRenderer.

#ifndef __MODEL_H__
#define __MODEL_H__

#include "vector"
#include "string"
#include "vector.hpp"

// v x y z
// 顶点坐标 v x坐标 y坐标 z坐标

// vt u v w
// 顶点在纹理图片中的对应点 vt 纹理图片对应的坐标

// vn x y z
// 顶点法向量 vn x分量 y分量 z分量

// f v/vt/vn v/vt/vn v/vt/vn
// f 顶点索引/纹理坐标索引/顶点法向量索引
// 面的法向量是由构成面的顶点对应的顶点法向量的做矢量和决定的（xyz的坐标分别相加再除以3得到的）

#define MAX 100

// 顶点
typedef struct Vertex {
    // 顶点坐标
    Vectorf3 v;
    // 顶点纹理
    Vectorf3 vt;
    // 顶点法向量
    Vectorf3 vn;
} Vertex_t;

// 坐标
typedef struct Vert_v {
    Vectorf3 v;
} Vert_v_t;

// 纹理
typedef struct Vert_vt {
    Vectorf3 vt;
} Vert_vt_t;

// 顶点法向量
typedef struct Vert_vn {
    Vectorf3 vn;
} Vert_vn_t;

// 面索引
typedef struct Face_idx {
    // 三个顶点索引
    // v/vt/vn
    Vectors3 v0;
    Vectors3 v1;
    Vectors3 v2;
} Face_idx_t;

// 面
typedef struct Face {
    // 保存三个顶点的实际数据
    Vertex_t v0;
    Vertex_t v1;
    Vertex_t v2;
} Face_t;

class OBJModel {
private:
    // 顶点集
    std::vector<Vertex_t> vertexes;
    // 坐标集合
    std::vector<Vert_v_t> vert_v;
    // 纹理集
    std::vector<Vert_vt_t> vert_vt;
    // 法线集
    std::vector<Vert_vn_t> vert_vn;
    // 面索引集
    std::vector<Face_idx_t> faces_idx;
    // 面集
    std::vector<Face_t> faces;
    // 解析 v
    void parse_v(const std::string &_line, Vert_v_t &_v) const;
    // 解析 vn
    void parse_vn(const std::string &_line, Vert_vn_t &_n) const;
    // 解析 vt
    void parse_vt(const std::string &_line, Vert_vt_t &_t) const;
    // 解析 f
    void parse_f(const std::string &_line, Face_idx_t &_f) const;
    // 映射顶点坐标信息
    void map_v(void);
    // 映射顶点纹理信息, 没有在 f 中的点不会被映射纹理信息
    void map_vt(void);
    // 映射顶点法线信息
    void map_vn(void);
    // 将面中保存的索引转换为坐标
    void faces_idx_2face(void);

public:
    OBJModel(const std::string &_filename);
    ~OBJModel(void);
    size_t   get_vertex_count(void) const;
    size_t   get_face_count(void) const;
    Vertex_t vert(size_t _idx) const;
    Face_t   face(size_t _idx) const;
};

#endif /* __MODEL_H__ */
