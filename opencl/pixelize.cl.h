static const char* pixelize_cl_source =
"/* This file is an image processing operation for GEGL                        \n"
" *                                                                            \n"
" * GEGL is free software; you can redistribute it and/or                      \n"
" * modify it under the terms of the GNU Lesser General Public                 \n"
" * License as published by the Free Software Foundation; either               \n"
" * version 3 of the License, or (at your option) any later version.           \n"
" *                                                                            \n"
" * GEGL is distributed in the hope that it will be useful,                    \n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of             \n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          \n"
" * Lesser General Public License for more details.                            \n"
" *                                                                            \n"
" * You should have received a copy of the GNU Lesser General Public           \n"
" * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.       \n"
" *                                                                            \n"
" * Copyright 2013 Victor Oliveira <victormatheus@gmail.com>                   \n"
" * Copyright 2013 Carlos Zubieta  <czubieta.dev@gmail.com>                    \n"
" */                                                                           \n"
"                                                                              \n"
"__kernel void calc_block_color(__global float4 *in,                           \n"
"                               __global float4 *out,                          \n"
"                                        int     xsize,                        \n"
"                                        int     ysize,                        \n"
"                                        int     roi_x,                        \n"
"                                        int     roi_y,                        \n"
"                                        int     total_width_x,                \n"
"                                        int     total_width_y,                \n"
"                                        int     line_width,                   \n"
"                                        int     block_count_x )               \n"
"{                                                                             \n"
"  int gidx = get_global_id(0);                                                \n"
"  int gidy = get_global_id(1);                                                \n"
"                                                                              \n"
"  int cx = roi_x / xsize + gidx;                                              \n"
"  int cy = roi_y / ysize + gidy;                                              \n"
"                                                                              \n"
"  int px = cx * xsize - roi_x;                                                \n"
"  int py = cy * ysize - roi_y;                                                \n"
"                                                                              \n"
"  int i, j;                                                                   \n"
"                                                                              \n"
"  float4 col = (float4)(0.0f, 0.0f, 0.0f, 0.0f);                              \n"
"                                                                              \n"
"  int real_xsize = min (total_width_x - px - roi_x, xsize);                   \n"
"  int real_ysize = min (total_width_y - py - roi_y, ysize);                   \n"
"                                                                              \n"
"  float weight = 1.0f / (real_xsize * real_ysize);                            \n"
"                                                                              \n"
"  for (j = py; j < py + real_ysize; ++j)                                      \n"
"    {                                                                         \n"
"      for (i = px; i < px + real_xsize; ++i)                                  \n"
"        {                                                                     \n"
"          col += in[(j + ysize) * line_width + i + xsize];                    \n"
"        }                                                                     \n"
"    }                                                                         \n"
"  out[gidy * block_count_x + gidx] = col * weight;                            \n"
"}                                                                             \n"
"                                                                              \n"
"                                                                              \n"
"#define NORM_MANHATTAN 0                                                      \n"
"#define NORM_EUCLIDEAN 1                                                      \n"
"#define NORM_INFINITY  2                                                      \n"
"#define SQR(x)         ((x)*(x))                                              \n"
"                                                                              \n"
"__kernel void kernel_pixelize(__global float4 *in,                            \n"
"                              __global float4 *out,                           \n"
"                                       int     xsize,                         \n"
"                                       int     ysize,                         \n"
"                                       float   xratio,                        \n"
"                                       float   yratio,                        \n"
"                                       int     roi_x,                         \n"
"                                       int     roi_y,                         \n"
"                                       float4  bg_color,                      \n"
"                                       int     norm,                          \n"
"                                       int     block_count_x)                 \n"
"{                                                                             \n"
"  int gidx = get_global_id(0);                                                \n"
"  int gidy = get_global_id(1);                                                \n"
"                                                                              \n"
"  int src_width  = get_global_size(0);                                        \n"
"  int cx = (gidx + roi_x) / xsize - roi_x / xsize;                            \n"
"  int cy = (gidy + roi_y) / ysize - roi_y / ysize;                            \n"
"                                                                              \n"
"  float4 grid_color = in[cx + cy * block_count_x];                            \n"
"  float4 out_color = bg_color;                                                \n"
"                                                                              \n"
"  int x_pos = gidx + roi_x;                                                   \n"
"  int y_pos = gidy + roi_y;                                                   \n"
"                                                                              \n"
"  int rect_shape_width  = ceil (xsize * xratio);                              \n"
"  int rect_shape_height = ceil (ysize * yratio);                              \n"
"                                                                              \n"
"  int off_shape_x = floor ((xsize - xratio * xsize) / 2.0f);                  \n"
"  int off_shape_y = floor ((ysize - yratio * ysize) / 2.0f);                  \n"
"                                                                              \n"
"  int start_x = (x_pos / xsize) * xsize - roi_x;                              \n"
"  int start_y = (y_pos / ysize) * ysize - roi_y;                              \n"
"                                                                              \n"
"  float shape_area = rect_shape_width * rect_shape_height;                    \n"
"                                                                              \n"
"  float center_x = start_x + off_shape_x + (float)(rect_shape_width)  / 2.0f; \n"
"  float center_y = start_y + off_shape_y + (float)(rect_shape_height) / 2.0f; \n"
"                                                                              \n"
"  if (norm == NORM_MANHATTAN &&                                               \n"
"      (fabs (gidx - center_x) * rect_shape_height +                           \n"
"       fabs (gidy - center_y) * rect_shape_width                              \n"
"       < shape_area))                                                         \n"
"    out_color = grid_color;                                                   \n"
"                                                                              \n"
"  if (norm == NORM_EUCLIDEAN &&                                               \n"
"      SQR ((gidx - center_x) / (float)rect_shape_width) +                     \n"
"      SQR ((gidy - center_y) / (float)rect_shape_height) <= 1.0f)             \n"
"    out_color = grid_color;                                                   \n"
"                                                                              \n"
"  if (norm == NORM_INFINITY &&                                                \n"
"      (gidx >= start_x + off_shape_x &&                                       \n"
"       gidy >= start_y + off_shape_y &&                                       \n"
"       gidx < start_x + off_shape_x + rect_shape_width &&                     \n"
"       gidy < start_y + off_shape_y + rect_shape_height))                     \n"
"      out_color = grid_color;                                                 \n"
"                                                                              \n"
"  out[gidx + gidy * src_width] = out_color;                                   \n"
"}                                                                             \n"
;
