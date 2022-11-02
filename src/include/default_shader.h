
/**
 * @file default_shader.h
 * @brief 默认着色器
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-11-01
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-11-01<td>Zone.N<td>创建文件
 * </table>
 */

#ifndef _DEFAULT_SHADER_H_
#define _DEFAULT_SHADER_H_

#include "shader.h"

/**
 * @brief 默认着色器
 */
class default_shader_t : public shader_base_t {
private:
    /**
     * @brief 颜色插值，由重心坐标计算出对应点的颜色
     * @param  _color0          第一个点的颜色
     * @param  _color1          第二个点的颜色
     * @param  _color2          第三个点的颜色
     * @param  _barycentric_coord   重心坐标
     * @return const color_t    颜色值
     */
    static const color_t
    interpolate_color(const color_t& _c0, const color_t& _c1,
                      const color_t& _c2, const vector4f_t& _barycentric_coord);

public:
    /**
     * @brief 构造函数
     */
    default_shader_t(void);

    /**
     * @brief 析构函数
     */
    ~default_shader_t(void);

    /**
     * @brief 顶点着色器
     * @param  _shader_vertex_in    输入
     * @return const shader_vertex_out_t    输出
     */
    const shader_vertex_out_t
    vertex(const shader_vertex_in_t& _shader_vertex_in) override;

    /**
     * @brief 片段着色器
     * @param  _shader_fragment_in  输入
     * @return const shader_fragment_out_t  输出
     */
    const shader_fragment_out_t
    fragment(const shader_fragment_in_t& _shader_fragment_in) override;
};

#endif /* _DEFAULT_SHADER_H_ */
