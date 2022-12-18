
/**
 * @file draw3d.cpp
 * @brief 三维绘制
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-14
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-14<td>Zone.N<td>创建文件
 * </table>
 */

#include "cmath"

#include "draw3d.h"

/// @todo 巨大性能开销
std::pair<bool, vector4f_t>
draw3d_t::get_barycentric_coord(const vector4f_t& _p0, const vector4f_t& _p1,
                                const vector4f_t& _p2, const vector4f_t& _p) {
    auto ab   = _p1 - _p0;
    auto ac   = _p2 - _p0;
    auto ap   = _p - _p0;

    auto deno = (ab.x * ac.y - ab.y * ac.x);
    if (std::abs(deno) < std::numeric_limits<decltype(deno)>::epsilon()) {
        return std::pair<bool, const vector4f_t> { false, vector4f_t() };
    }

    auto s = (ac.y * ap.x - ac.x * ap.y) / deno;
    if ((s > 1) || (s < 0)) {
        return std::pair<bool, const vector4f_t> { false, vector4f_t() };
    }

    auto t = (ab.x * ap.y - ab.y * ap.x) / deno;
    if ((t > 1) || (t < 0)) {
        return std::pair<bool, const vector4f_t> { false, vector4f_t() };
    }

    if ((1 - s - t > 1) || (1 - s - t < 0)) {
        return std::pair<bool, const vector4f_t> { false, vector4f_t() };
    }

    return std::pair<bool, const vector4f_t> { true,
                                               vector4f_t(1 - s - t, s, t) };
}

framebuffer_t::depth_t
draw3d_t::interpolate_depth(const framebuffer_t::depth_t& _depth0,
                            const framebuffer_t::depth_t& _depth1,
                            const framebuffer_t::depth_t& _depth2,
                            const vector4f_t&             _barycentric_coord) {
    auto z = _depth0 * _barycentric_coord.x;
    z      += _depth1 * _barycentric_coord.y;
    z      += _depth2 * _barycentric_coord.z;
    return z;
}

void draw3d_t::triangle(const model_t::vertex_t& _v0,
                        const model_t::vertex_t& _v1,
                        const model_t::vertex_t& _v2,
                        const model_t::normal_t& _normal) {
    auto min = _v0.coord.min(_v1.coord).min(_v2.coord);
    auto max = _v0.coord.max(_v1.coord).max(_v2.coord);
#pragma omp parallel for num_threads(config->procs) collapse(2)
    for (auto x = int32_t(min.x); x <= int32_t(max.x); x++) {
        for (auto y = int32_t(min.y); y <= int32_t(max.y); y++) {
            /// @todo 这里要用裁剪替换掉
            if ((unsigned)x >= width || (unsigned)y >= height) {
                continue;
            }
            auto [is_inside, barycentric_coord]
              = get_barycentric_coord(_v0.coord, _v1.coord, _v2.coord,
                                      vector4f_t(static_cast<float>(x),
                                                 static_cast<float>(y), 0));
            // 如果点在三角形内再进行下一步
            if (is_inside == false) {
                continue;
            }
            // 计算该点的深度，通过重心坐标插值计算
            auto z = interpolate_depth(_v0.coord.z, _v1.coord.z, _v2.coord.z,
                                       barycentric_coord);
            // 深度在已有颜色之上
            if (z < framebuffer->get_depth_buffer(x, y)) {
                continue;
            }
            // 计算颜色，颜色为通过 shader 片段着色器计算
            auto shader_fragment_out
              = shader.fragment(shader_fragment_in_t(barycentric_coord, _normal,
                                                     light.dir, _v0.color,
                                                     _v1.color, _v2.color));
            if (shader_fragment_out.is_need_draw == false) {
                continue;
            }
            auto color = color_t(shader_fragment_out.color);
            // 填充像素
            framebuffer->pixel(x, y, color, z);
        }
    }
    return;
}

void draw3d_t::triangle(const model_t::face_t& _face) {
    triangle(_face.v0, _face.v1, _face.v2, _face.normal);
    return;
}

draw3d_t::draw3d_t(const std::shared_ptr<config_t>&      _config,
                   const std::shared_ptr<framebuffer_t>& _framebuffer,
                   const shader_base_t&                  _shader)
    : config(_config), framebuffer(_framebuffer), shader(_shader) {
    width  = framebuffer->get_width();
    height = framebuffer->get_height();
    return;
}

draw3d_t::~draw3d_t(void) {
    return;
}

void draw3d_t::line(float _x0, float _y0, float _x1, float _y1,
                    const color_t& _color) {
    auto p0_x  = static_cast<int32_t>(_x0);
    auto p0_y  = static_cast<int32_t>(_y0);
    auto p1_x  = static_cast<int32_t>(_x1);
    auto p1_y  = static_cast<int32_t>(_y1);

    auto steep = false;
    if (std::abs(p0_x - p1_x) < std::abs(p0_y - p1_y)) {
        std::swap(p0_x, p0_y);
        std::swap(p1_x, p1_y);
        steep = true;
    }
    // 终点 x 坐标在起点 左边
    if (p0_x > p1_x) {
        std::swap(p0_x, p1_x);
        std::swap(p0_y, p1_y);
    }

    auto de  = 0;
    auto dy2 = std::abs(p1_y - p0_y) << 1;
    auto dx2 = std::abs(p1_x - p0_x) << 1;
    auto y   = p0_y;
    auto yi  = 1;
    if (p1_y <= p0_y) {
        yi = -1;
    }
    for (auto x = p0_x; x <= p1_x; x++) {
        if (steep == true) {
            /// @todo 这里要用裁剪替换掉
            if ((unsigned)y >= width || (unsigned)x >= height) {
                continue;
            }
            framebuffer->pixel(y, x, _color);
        }
        else {
            /// @todo 这里要用裁剪替换掉
            if ((unsigned)x >= width || (unsigned)y >= height) {
                continue;
            }
            framebuffer->pixel(x, y, _color);
        }
        de += std::abs(dy2);
        if (de >= dx2) {
            y  += yi;
            de -= dx2;
        }
    }

    return;
}

void draw3d_t::triangle(const vector4f_t& _v0, const vector4f_t& _v1,
                        const vector4f_t& _v2, const color_t& _color) {
    auto min = _v0.min(_v1).min(_v2);
    auto max = _v0.max(_v1).max(_v2);

    for (auto x = static_cast<uint32_t>(min.x);
         x <= static_cast<uint32_t>(max.x); x++) {
        for (auto y = static_cast<uint32_t>(min.y);
             y <= static_cast<uint32_t>(max.y); y++) {
            auto [is_inside, _]
              = get_barycentric_coord(_v0, _v1, _v2,
                                      vector4f_t(static_cast<float>(x),
                                                 static_cast<float>(y)));
            if (is_inside) {
                framebuffer->pixel(x, y, _color);
            }
        }
    }
    return;
}

void draw3d_t::model(const model_t& _model) {
    if (config->draw_wireframe == true) {
#pragma omp parallel for num_threads(config->procs)
        for (const auto& f : _model.get_face()) {
            /// @todo 巨大性能开销
            auto face = shader.vertex(shader_vertex_in_t(f)).face;
            line(face.v0.coord.x, face.v0.coord.y, face.v1.coord.x,
                 face.v1.coord.y, color_t::WHITE);
            line(face.v1.coord.x, face.v1.coord.y, face.v2.coord.x,
                 face.v2.coord.y, color_t::WHITE);
            line(face.v2.coord.x, face.v2.coord.y, face.v0.coord.x,
                 face.v0.coord.y, color_t::WHITE);
        }
    }
    else {
#pragma omp parallel for num_threads(config->procs)
        for (const auto& f : _model.get_face()) {
            /// @todo 巨大性能开销
            auto face = shader.vertex(shader_vertex_in_t(f)).face;
            triangle(face);
        }
    }
    return;
}
