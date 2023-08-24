
/**
 * @file color.cpp
 * @brief 颜色实现
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-11-02
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-11-02<td>Zone.N<td>创建文件
 * </table>
 */

#include <limits>
#include <span>

#include "color.h"
#include "log.hpp"

color_t::color_t(uint32_t _data) {
  auto data_ptr = std::span(reinterpret_cast<uint8_t *>(&_data), 4);
  channel_r = data_ptr[0];
  channel_g = data_ptr[1];
  channel_b = data_ptr[2];
  channel_a = data_ptr[3];
}

color_t::color_t(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha)
    : channel_r(_red), channel_g(_green), channel_b(_blue), channel_a(_alpha) {}

color_t::color_t(float _red, float _green, float _blue, float _alpha)
    : channel_r(uint8_t(_red * std::numeric_limits<uint8_t>::max())),
      channel_g(uint8_t(_green * std::numeric_limits<uint8_t>::max())),
      channel_b(uint8_t(_blue * std::numeric_limits<uint8_t>::max())),
      channel_a(uint8_t(_alpha * std::numeric_limits<uint8_t>::max())) {}

auto color_t::operator[](uint8_t _idx) -> uint8_t & {
  if (_idx > 3) {
    throw std::invalid_argument(log("_idx > 3"));
  }
  if (_idx == 0) {
    return channel_r;
  }
  if (_idx == 1) {
    return channel_g;
  }
  if (_idx == 2) {
    return channel_b;
  }
  return channel_a;
}

auto color_t::operator[](uint8_t _idx) const -> uint8_t {
  if (_idx > 3) {
    throw std::invalid_argument(log("_idx > 3"));
  }
  if (_idx == 0) {
    return channel_r;
  }
  if (_idx == 1) {
    return channel_g;
  }
  if (_idx == 2) {
    return channel_b;
  }
  return channel_a;
}

[[maybe_unused]] auto color_t::bpp() -> size_t { return BPP; }

[[maybe_unused]] auto color_t::to_uint32() const -> uint32_t {
  uint32_t ret = 0;
  auto ret_ptr = std::span(reinterpret_cast<uint8_t *>(&ret), 4);

  ret_ptr[0] = channel_r;
  ret_ptr[1] = channel_g;
  ret_ptr[2] = channel_b;
  ret_ptr[3] = channel_a;

  return ret;
}
