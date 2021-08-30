#pragma once
//=====================================================================
// ��Ⱦ�豸
//=====================================================================
#ifndef render_h
#define render_h
#include "s_math.h"
#include "rasterize.h"
#include<iostream>
#include<vector>
using namespace std;
typedef struct
{
	s_vector lightpos;
	s_vector lightcolor;
}point_light;
typedef struct
{
	s_vector viewpos;
}s_camera;
typedef struct
{
	s_transform transform;   // ����任��
	int width;               // ���ڿ��
	int height;              // ���ڸ߶�
	IUINT32** framebuffer;   // ���ػ��棺framebuffer[y] ����� y��
	float** zbuffer;         // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��
	IUINT32** texture;       // ����ͬ����ÿ������
	int tex_width;           // ������
	int tex_height;          // ����߶�
	float max_u;             // ��������ȣ�tex_width - 1
	float max_v;             // �������߶ȣ�tex_height - 1
	int render_state;        // ��Ⱦ״̬
	IUINT32 background;      // ������ɫ
	IUINT32 foreground;      // �߿���ɫ
	point_light pointlight[20];
	s_camera camera;

}device_t;

#define RENDER_STATE_WIREFRAME 1  //��Ⱦ�߿� 
#define RENDER_STATE_TEXTURE    2  //��Ⱦ���� 
#define RENDER_STATE_COLOR      4  //��Ⱦ��ɫ
//�豸��ʼ����fbΪ�ⲿ֡���棬��NULL�������ⲿ֡���� 
void device_init(device_t* device, int width, int height, void* fb);

// ɾ���豸
void device_destory(device_t* device);
//���õ�ǰ���� 
void device_set_texture(device_t* device, void* bits, long pitch, int w, int h);
//���õ��Դ
void device_set_pointlight(device_t* device, s_vector& pos, s_vector& color, int cnt);
// ��� framebuffer �� zbuffer
void device_clear(device_t* device, int mode);

void device_pixel(device_t* device, int x, int y, IUINT32 color);

void device_draw_line(device_t* device, int x1, int y1, int x2, int y2, IUINT32 c);

// ���������ȡ����
IUINT32 device_texture_read(const device_t* device, float u, float v);

//=====================================================================
// ��Ⱦʵ��
//=====================================================================

// ����ɨ����
void device_draw_scanline(device_t* device, scanline_t* scanline,int count);

//����Ⱦ���� 
void device_render_trap(device_t* device, trapezoid_t* trap,int count);

// ���� render_state ����ԭʼ������
void device_draw_primitive(device_t* device, vertex_t* v1,
	vertex_t* v2, vertex_t* v3,int count);

typedef struct
{
	s_vector pos;//���� 
	s_color color;// ��ɫ 
	s_vector normal; //������ 
	s_vector tangent;// ���� 
	s_vector binormal;//�������� 
	s_vector2f texcoord;//�������� 
}for_vs;

typedef struct
{
	s_vector pos;//���� 
	s_vector2f texcoord;//�������� 
	s_color color; //��ɫ 
	s_vector normal;//������ 
	s_vector storage0;// ��ֵ�Ĵ���
	s_vector storage1;
	s_vector storage2;
}for_fs;

void v_shader(device_t* device, for_vs* vv, for_fs* ff);

void f_shader(device_t* device, for_fs* ff, s_color& color,int count);
#endif
