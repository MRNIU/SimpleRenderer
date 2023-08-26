
/**
 * @file scene.cpp
 * @brief 场景抽象
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-12-15
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-12-15<td>Zone.N<td>创建文件
 * </table>
 */

#include "scene.h"
#include "matrix.h"

/**
 * @brief 计算从模型坐标(局部坐标)到世界坐标的变换矩阵，缩放 + 移动到 (0,
 * 0)，屏幕左上角
 * @param  _model           要被应用的模型
 * @bug 变换可能有问题
 * @param  _rotate          在默认变换的基础上进行变换的旋转矩阵，默认为 1
 * @param  _scale           在默认变换的基础上进行变换的缩放矩阵，默认为 1
 * @param  _translate       在默认变换的基础上进行变换的平移矩阵，默认为 1
 * @return std::pair<const matrix4f_t, const matrix4f_t>    变换矩阵
 */
inline auto model2world_tran(const model_t &_model, const matrix4f_t &_rotate,
                             const matrix4f_t &_scale,
                             const matrix4f_t &_translate)
    -> std::pair<const matrix4f_t, const matrix4f_t> {
  //  // 坐标变换矩阵
  //  matrix4f_t coord_mat;
  //  coord_mat = _rotate * coord_mat;
  //  // 法线变换矩阵
  //  matrix4f_t normal_mat;
  //  normal_mat = coord_mat.inverse().transpose();
  //  // 用旋转后的顶点计算极值
  //  auto tmp = _model.get_face()[0].v0.coord.x();
  //  auto x_max = std::numeric_limits<decltype(tmp)>::lowest();
  //  auto x_min = std::numeric_limits<decltype(tmp)>::max();
  //  auto y_max = x_max;
  //  auto y_min = x_min;
  //  auto z_max = x_max;
  //  auto z_min = x_min;
  //  for (auto &i : _model.get_face()) {
  //    auto v0 = i.v0.coord * coord_mat;
  //    auto v1 = i.v1.coord * coord_mat;
  //    auto v2 = i.v2.coord * coord_mat;
  //    x_max = std::max(x_max, v0.x());
  //    x_max = std::max(x_max, v1.x());
  //    x_max = std::max(x_max, v2.x());
  //    x_min = std::min(x_min, v0.x());
  //    x_min = std::min(x_min, v1.x());
  //    x_min = std::min(x_min, v2.x());
  //
  //    y_max = std::max(y_max, v0.y());
  //    y_max = std::max(y_max, v1.y());
  //    y_max = std::max(y_max, v2.y());
  //    y_min = std::min(y_min, v0.y());
  //    y_min = std::min(y_min, v1.y());
  //    y_min = std::min(y_min, v2.y());
  //
  //    z_max = std::max(z_max, v0.z());
  //    z_max = std::max(z_max, v1.z());
  //    z_max = std::max(z_max, v2.z());
  //    z_min = std::min(z_min, v0.z());
  //    z_min = std::min(z_min, v1.z());
  //    z_min = std::min(z_min, v2.z());
  //  }
  //
  //  // 各分量的长度
  //  auto delta_x = (std::abs(x_max) + std::abs(x_min));
  //  auto delta_y = (std::abs(y_max) + std::abs(y_min));
  //  auto delta_z = (std::abs(z_max) + std::abs(z_min));
  //  auto delta_xy_max = std::max(delta_x, delta_y);
  //  auto delta_xyz_max = std::max(delta_xy_max, delta_z);
  //
  //  // 缩放倍数
  //  auto multi = static_cast<float>((1080 + 1920) / 4.);
  //  // 归一化并乘倍数
  //  auto scale = multi / delta_xyz_max;
  //  // 缩放
  //  auto scale_mat = matrix4f_t();
  //  scale_mat.setIdentity();
  //  scale_mat.diagonal()[0] = scale;
  //  scale_mat.diagonal()[1] = scale;
  //  scale_mat.diagonal()[2] = scale;
  //
  //  coord_mat = scale_mat * coord_mat;
  //  coord_mat = _scale * coord_mat;
  //  // 移动到左上角
  //  auto aaa = matrix4f_t();
  //  aaa(0, 3) = std::abs(x_min) * scale;
  //  aaa(1, 3) = std::abs(y_min) * scale;
  //  aaa(2, 3) = 0;
  //  coord_mat *= aaa;
  //
  //  // 从左上角移动到指定位置
  //  coord_mat = _translate * coord_mat;
  //  return std::pair<const matrix4f_t, const
  //  matrix4f_t>{matrix4f_t(coord_mat),
  //                                                       matrix4f_t(normal_mat)};

  return std::pair<const matrix4f_t, const matrix4f_t>{matrix4f_t(),
                                                       matrix4f_t()};
}

void scene_t::add_model(const model_t &_model) {
  auto matrix = matrix4f_t();
  matrix.setIdentity();
  // 缩放
  auto scale_mat = matrix4f_t();
  scale_mat.setIdentity();
  scale_mat(0, 0) = 1000;
  scale_mat(1, 1) = 1000;
  scale_mat(2, 2) = 1000;
  //  std::cout << scale_mat * matrix << std::endl;
  // 旋转
  auto rotate = Eigen::Affine3f(Eigen::Quaternion<float>(
      Eigen::AngleAxisf(0.1, Eigen::Vector3f::UnitZ())));
  //  std::cout << rotate * matrix << std::endl;
  // 平移
  auto tran_mat = matrix4f_t();
  tran_mat.setIdentity();
  tran_mat(0, 3) = 960;
  tran_mat(1, 3) = 480;
  tran_mat(2, 3) = 0;
  //  std::cout << tran_mat * matrix << std::endl;
  // 应用顺序为从右到左，即先缩放，再旋转，最后平移
  //  auto first = tran_mat * rotate * scale_mat * matrix;
  auto first = rotate * tran_mat * scale_mat * matrix;

  std::cout << first << std::endl;
  //  auto aaa = model2world_tran(_model, matrix4f_t(), matrix4f_t(),
  //  matrix4f_t()); std::cout << aaa.first << std::endl;

  models.push_back(_model);
}

void scene_t::add_model(const model_t &_model,
                        const matrix4f_t &_model_matrix) {
  models.push_back(_model * _model_matrix);
}

void scene_t::add_light(const light_t &_light) { lights.push_back(_light); }

auto scene_t::tick(uint32_t _delta_time) -> bool {
  (void)_delta_time;
  /// @todo 更新可见模型
  // 清空队列
  std::queue<model_t> empty;
  swap(visible_models, empty);
  for (const auto &i : models) {
    visible_models.push(i);
  }
  /// @todo 根据所有光照进行计算
  for (const auto &i : lights) {
    light = i;
  }
  /// @todo 移动摄像机

  return true;
}

auto scene_t::get_current_camera() -> camera_base_t & { return camera; }

auto scene_t::get_current_camera() const -> const camera_base_t & {
  return camera;
}

auto scene_t::get_visible_models() const -> const std::queue<model_t> & {
  return visible_models;
}

auto scene_t::get_light() -> light_t & { return light; }

auto scene_t::get_light() const -> const light_t & { return light; }
