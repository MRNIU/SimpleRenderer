
// This file is a part of Simple-XX/SimpleRenderer
// (https://github.com/Simple-XX/SimpleRenderer).
//
// renderer.cpp for Simple-XX/SimpleRenderer.

#include "iostream"
#include "limits"
#include "common.h"
#include "renderer.h"

using namespace std;

size_t Renderer::get_x(float _x) const {
    return (_x + 1.) * width / 2.;
}

size_t Renderer::get_y(float _y) const {
    return height - (_y + 1.) * height / 2.;
}

Renderer::Renderer(Geometry &_painter, const OBJModel &_model)
    : model(_model), painter(_painter) {
    width   = painter.get_width();
    height  = painter.get_height();
    zbuffer = new float[width * height];
    for (size_t i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    return;
}

Renderer::~Renderer(void) {
    delete zbuffer;
    return;
}

bool Renderer::render(void) {
    // line_zbuffer();
    // triangle_zbuffer();
    // triangle_fill_zbuffer();
    // circle_zbuffer();
    // circle_fill_zbuffer();
    return true;
}

bool Renderer::render(void) const {
    // line();
    // triangle();
    // triangle_fill();
    // circle();
    circle_fill();
    return true;
}

bool Renderer::set_light(const Vectorf3 &_light) {
    light_dir = _light;
    return true;
}

bool Renderer::set_size(size_t _w, size_t _h) {
    width  = _w;
    height = _h;
    return true;
}

Vectorf3 Renderer::get_light(void) const {
    return light_dir;
}

Vectors2 Renderer::get_size(void) const {
    return Vectors2(width, height);
}

bool Renderer::save(const std::string &_filename) const {
    return painter.save(_filename);
}

bool Renderer::line(void) const {
    Vectors2 v0 = Vectors2(width, height);
    Vectors2 v1 = Vectors2(0, 0);
    painter.line(v0, v1, white);
    return true;
}

bool Renderer::line_zbuffer(void) {
    for (size_t i = 0; i < model.get_face_count(); i++) {
        Face_t   face = model.face(i);
        Vectorf3 v0   = Vectorf3(get_x(face.v0.v.coord.x),
                               get_y(face.v0.v.coord.y), face.v0.v.coord.z);
        Vectorf3 v1   = Vectorf3(get_x(face.v1.v.coord.x),
                               get_y(face.v1.v.coord.y), face.v1.v.coord.z);
        painter.line(v0, v1, zbuffer, white);
    }
    return true;
}

bool Renderer::triangle(void) const {
    // 所有面
    for (size_t i = 0; i < model.get_face_count(); i++) {
        // 取出一个面
        Face_t   face = model.face(i);
        Vectors2 v0 =
            Vectors2(get_x(face.v0.v.coord.x), get_y(face.v0.v.coord.y));
        Vectors2 v1 =
            Vectors2(get_x(face.v1.v.coord.x), get_y(face.v1.v.coord.y));
        Vectors2 v2 =
            Vectors2(get_x(face.v2.v.coord.x), get_y(face.v2.v.coord.y));
        painter.triangle(v0, v1, v2, white);
    }
    return true;
}

bool Renderer::triangle_zbuffer(void) {
    for (size_t i = 0; i < model.get_face_count(); i++) {
        Face_t   face = model.face(i);
        Vectorf3 v0   = Vectorf3(get_x(face.v0.v.coord.x),
                               get_y(face.v0.v.coord.y), face.v0.v.coord.z);
        Vectorf3 v1   = Vectorf3(get_x(face.v1.v.coord.x),
                               get_y(face.v1.v.coord.y), face.v1.v.coord.z);
        Vectorf3 v2   = Vectorf3(get_x(face.v2.v.coord.x),
                               get_y(face.v2.v.coord.y), face.v2.v.coord.z);
        painter.triangle(v0, v1, v2, zbuffer, white);
    }
    return true;
}

bool Renderer::triangle_fill(void) const {
    for (size_t i = 0; i < model.get_face_count(); i++) {
        // 取出一个面
        Face_t   face = model.face(i);
        Vectors2 v0 =
            Vectors2(get_x(face.v0.v.coord.x), get_y(face.v0.v.coord.y));
        Vectors2 v1 =
            Vectors2(get_x(face.v1.v.coord.x), get_y(face.v1.v.coord.y));
        Vectors2 v2 =
            Vectors2(get_x(face.v2.v.coord.x), get_y(face.v2.v.coord.y));
        Vectorf3 n         = (face.v2.v - face.v0.v) ^ (face.v1.v - face.v0.v);
        float    intensity = light_dir * n.unit();
        if (intensity > 0) {
            painter.triangle_fill(v0, v1, v2,
                                  TGAColor(intensity * 255, intensity * 255,
                                           intensity * 255, 255));
        }
    }
    return true;
}

bool Renderer::triangle_fill_zbuffer(void) {
    for (size_t i = 0; i < model.get_face_count(); i++) {
        // 取出一个面
        Face_t   face      = model.face(i);
        Vectorf3 v0        = Vectorf3(get_x(face.v0.v.coord.x),
                               get_y(face.v0.v.coord.y), face.v0.v.coord.z);
        Vectorf3 v1        = Vectorf3(get_x(face.v1.v.coord.x),
                               get_y(face.v1.v.coord.y), face.v1.v.coord.z);
        Vectorf3 v2        = Vectorf3(get_x(face.v2.v.coord.x),
                               get_y(face.v2.v.coord.y), face.v2.v.coord.z);
        Vectorf3 n         = (face.v2.v - face.v0.v) ^ (face.v1.v - face.v0.v);
        float    intensity = light_dir * n.unit();
        // Vectorf3 n         = face.v0.vn + face.v1.vn + face.v2.vn;
        // float    intensity = light_dir * (n * 0.33333);
        if (intensity > 0) {
            painter.triangle_fill(v0, v1, v2, zbuffer,
                                  TGAColor(intensity * 255, intensity * 255,
                                           intensity * 255, 255));
        }
    }
    return true;
}

bool Renderer::circle(void) const {
    painter.circle(mid_width, mid_height, 50, white);
    return true;
}

bool Renderer::circle_fill(void) const {
    painter.circle_fill(mid_width, mid_height, 50, white);
    return true;
}

bool Renderer::circle_fill_zbuffer(void) {
    painter.circle_fill(mid_width, mid_height, 50, zbuffer, white);
    return true;
}
