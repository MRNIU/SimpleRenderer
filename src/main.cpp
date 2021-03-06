
// This file is a part of Simple-XX/SimpleRenderer
// (https://github.com/Simple-XX/SimpleRenderer).
//
// main.cpp for Simple-XX/SimpleRenderer.

#include "common.h"
#include "geometry.h"
#include "image.h"
#include "iostream"
#include "model.h"
#include "renderer.h"
#include "test.h"
#include "vector.hpp"

using namespace std;

Test test;

int main(int argc, char **argv) {
    string filename;
    if (2 == argc) {
        filename = argv[1];
    }
    else {
        filename = "../../src/obj/african_head.obj";
    }
    OBJModel       model(filename);
    TGAImage       image(width, height, TGAImage::RGBA);
    Geometry       painter(image);
    const Renderer render = Renderer(painter, model);
    render.render();
    render.save();
    // test.test_vector();
    // test.test_matrix();
    return 0;
}
