
// This file is a part of SimpleXX/SimpleRenderer
// (https://github.com/SimpleXX/SimpleRenderer).
// Based on https://github.com/ssloy/tinyrenderer
// model.cpp for SimpleXX/SimpleRenderer.

#include "iostream"
#include "fstream"
#include "sstream"
#include "model.h"

void OBJModel::parse_v(const std::string &_line, Vert_v_t &_v) const {
    assert(_line.compare(0, 2, "v ") == 0);
    char               trash = 0;
    std::istringstream iss(_line.c_str());
    // 跳过标识
    iss >> trash;
    // 填充数据
    iss >> _v.v.coord.x;
    iss >> _v.v.coord.y;
    iss >> _v.v.coord.z;
    return;
}

void OBJModel::parse_vt(const std::string &_line, Vert_vt_t &_t) const {
    assert(_line.compare(0, 3, "vt ") == 0);
    char               trash = 0;
    std::istringstream iss(_line.c_str());
    // 跳过标识
    iss >> trash >> trash;
    // 填充数据
    iss >> _t.vt.coord.x;
    iss >> _t.vt.coord.y;
    // 第三维纹理不一定存在
    if (iss.eof() == false) {
        iss >> _t.vt.coord.z;
    }
    else {
        _t.vt.coord.z = 0;
    }
    return;
}

void OBJModel::parse_vn(const std::string &_line, Vert_vn_t &_n) const {
    assert(_line.compare(0, 3, "vn ") == 0);
    char               trash = 0;
    std::istringstream iss(_line.c_str());
    // 跳过标识
    iss >> trash >> trash;
    // 填充数据
    iss >> _n.vn.coord.x;
    iss >> _n.vn.coord.y;
    iss >> _n.vn.coord.z;
    return;
}

void OBJModel::parse_f(const std::string &_line, Face_idx_t &_fidx) const {
    assert(_line.compare(0, 2, "f ") == 0);
    char               trash = 0;
    std::istringstream iss(_line.c_str());
    // 跳过标识 f
    iss >> trash;
    // 填充数据
    // 这里注意索引要减 1
    // 第一个顶点
    // v
    iss >> _fidx.v0[0];
    _fidx.v0[0]--;
    // 跳过分隔符
    iss >> trash;
    // vt
    iss >> _fidx.v0[1];
    _fidx.v0[1]--;
    // 跳过分隔符
    iss >> trash;
    // vn
    iss >> _fidx.v0[2];
    _fidx.v0[2]--;

    // 第二个顶点
    iss >> _fidx.v1[0];
    _fidx.v1[0]--;
    // 跳过分隔符
    iss >> trash;
    iss >> _fidx.v1[1];
    _fidx.v1[1]--;
    // 跳过分隔符
    iss >> trash;
    iss >> _fidx.v1[2];
    _fidx.v1[2]--;

    // 第三个顶点
    iss >> _fidx.v2[0];
    _fidx.v2[0]--;
    // 跳过分隔符
    iss >> trash;
    iss >> _fidx.v2[1];
    _fidx.v2[1]--;
    // 跳过分隔符
    iss >> trash;
    iss >> _fidx.v2[2];
    _fidx.v2[2]--;
    return;
}

void OBJModel::map_v(void) {
    for (Face_idx_t i : faces_idx) {
        // 第一个顶点
        vertexes.at(i.v0[0]).v = vert_v.at(i.v0[0]).v;
        // 第二个顶点
        vertexes.at(i.v1[0]).v = vert_v.at(i.v1[0]).v;
        // 第三个顶点
        vertexes.at(i.v2[0]).v = vert_v.at(i.v2[0]).v;
    }
    return;
}

void OBJModel::map_vt(void) {
    for (Face_idx_t i : faces_idx) {
        // 第一个顶点
        vertexes.at(i.v0[0]).vt = vert_vt.at(i.v0[1]).vt;
        // 第二个顶点
        vertexes.at(i.v1[0]).vt = vert_vt.at(i.v1[1]).vt;
        // 第三个顶点
        vertexes.at(i.v2[0]).vt = vert_vt.at(i.v2[1]).vt;
    }
    return;
}

void OBJModel::map_vn(void) {
    for (Face_idx_t i : faces_idx) {
        // 第一个顶点
        vertexes.at(i.v0[0]).vn = vert_vn.at(i.v0[2]).vn;
        // 第二个顶点
        vertexes.at(i.v1[0]).vn = vert_vn.at(i.v1[2]).vn;
        // 第三个顶点
        vertexes.at(i.v2[0]).vn = vert_vn.at(i.v2[2]).vn;
    }
    return;
}

void OBJModel::faces_idx_2face(void) {
    Face_t f;
    for (Face_idx_t i : faces_idx) {
        // 第一个顶点
        f.v0 = vertexes.at(i.v0[0]);
        // 第二个顶点
        f.v1 = vertexes.at(i.v1[0]);
        // 第三个顶点
        f.v2 = vertexes.at(i.v2[0]);
        faces.push_back(f);
    }
    return;
}

OBJModel::OBJModel(const std::string &_filename)
    : vertexes(), vert_v(), vert_vt(), vert_vn(), faces_idx(), faces() {
    std::ifstream in;
    in.open(_filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Can not open file " << _filename << "." << std::endl;
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        Vert_v_t           v;
        Vert_vt_t          t;
        Vert_vn_t          n;
        Face_idx_t         fidx;
        Face_t             f;
        // 处理坐标
        if (line.compare(0, 2, "v ") == 0) {
            parse_v(line, v);

            vert_v.push_back(v);
        }
        // 处理纹理
        else if (line.compare(0, 3, "vt ") == 0) {
            parse_vt(line, t);
            vert_vt.push_back(t);
        }
        // 处理顶点法向量
        else if (line.compare(0, 3, "vn ") == 0) {
            parse_vn(line, n);
            vert_vn.push_back(n);
        }
        // 处理面信息
        else if (line.compare(0, 2, "f ") == 0) {
            parse_f(line, fidx);
            faces_idx.push_back(fidx);
        }
        vertexes.push_back(Vertex_t());
    }
    // 映射坐标信息
    map_v();
    // 映射法线信息
    map_vn();
    // 映射顶点纹理
    map_vt();
    // 转换为实际坐标
    faces_idx_2face();
    std::cout << "# v# " << vert_v.size() << " t# " << vert_vt.size() << " n# "
              << vert_vn.size() << " f# " << faces.size() << std::endl;
    return;
}

OBJModel::~OBJModel() {
    return;
}

size_t OBJModel::get_vertex_count() const {
    return vertexes.size();
}

size_t OBJModel::get_face_count() const {
    return faces.size();
}

Face_t OBJModel::face(size_t _idx) const {
    return faces.at(_idx);
}

Vertex_t OBJModel::vert(size_t _idx) const {
    return vertexes.at(_idx);
}
