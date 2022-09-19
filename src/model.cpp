
/**
 * @file model.cpp
 * @brief 模型抽象
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-06-06
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-04<td>Zone.N<td>迁移到 doxygen
 * </table>
 */

#include "iostream"
#include "model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "3rd/tiny_obj_loader.h"

model_t::model_t(const std::string &_obj_path, const std::string &_mtl_path) {
    tinyobj::ObjReader       reader;
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = _mtl_path;
    // 默认开启三角化
    auto ret = reader.ParseFromFile(_obj_path, config);
    if (ret == false) {
        if (reader.Error().empty() == false) {
            throw(std::runtime_error(log(reader.Error())));
        }
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    const auto &attrib    = reader.GetAttrib();
    const auto &shapes    = reader.GetShapes();
    const auto &materials = reader.GetMaterials();

    printf("顶点数：%ld, ", attrib.vertices.size() / 3);
    printf("法线数：%ld, ", attrib.normals.size() / 3);
    printf("UV数：%ld, ", attrib.texcoords.size() / 2);
    printf("子模型数：%ld, ", shapes.size());
    printf("材质数：%ld\n", materials.size());

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        // 由于开启了三角化，所有的 shape 都是由三个点组成的，即 fv == 3
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (fv != 3) {
                throw(std::runtime_error(log("fv != 3")));
            }
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                mesh.vertices.push_back(vertex_t(
                    attrib.vertices[3 * size_t(idx.vertex_index) + 0],
                    attrib.vertices[3 * size_t(idx.vertex_index) + 1],
                    attrib.vertices[3 * size_t(idx.vertex_index) + 2]));

                // Check if `normal_index` is zero or positive. negative = no
                // normal data
                if (idx.normal_index >= 0) {
                    std::cout << "size_t(idx.normal_index): "
                              << size_t(idx.normal_index) << std::endl;
                    mesh.normals.push_back(normal_t(
                        attrib.normals[3 * size_t(idx.normal_index) + 0],
                        attrib.normals[3 * size_t(idx.normal_index) + 1],
                        attrib.normals[3 * size_t(idx.normal_index) + 2]));
                }
                else {
                    mesh.normals.push_back(normal_t(0, 0, 0));
                }

                // Check if `texcoord_index` is zero or positive. negative = no
                // texcoord data
                if (idx.texcoord_index >= 0) {
                    mesh.texcoords.push_back(texcoord_t(
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
                        attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]));
                }
                else {
                    mesh.texcoords.push_back(texcoord_t(0, 0));
                }

                // Optional: vertex colors
                // mesh.color.push_back(color_t(
                // attrib.colors[3 * size_t(idx.vertex_index) + 0] * UINT8_MAX,
                // attrib.colors[3 * size_t(idx.vertex_index) + 1] * UINT8_MAX,
                // attrib.colors[3 * size_t(idx.vertex_index) + 2] *
                // UINT8_MAX));
            }
            index_offset += fv;

            // per-face material
            std::cout << "shapes[s].mesh.material_ids[f]: "
                      << shapes[s].mesh.material_ids[f] << std::endl;
            mesh.materials.push_back(materials[s]);
        }
    }

    return;
}

model_t::~model_t(void) {
    return;
}

const std::vector<vertex_t> &model_t::get_vertex(void) const {
    return mesh.vertices;
}

const std::vector<normal_t> &model_t::get_normal(void) const {
    return mesh.normals;
}

const std::vector<texcoord_t> &model_t::get_texcoord(void) const {
    return mesh.texcoords;
}

const std::vector<tinyobj::material_t> &model_t::get_material(void) const {
    return mesh.materials;
}

const std::vector<face_t> &model_t::get_face(void) const {
    return faces;
}
