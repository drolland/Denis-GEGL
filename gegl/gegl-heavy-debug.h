/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gegl-heavy-debug.h
 * Author: denis
 *
 * Created on 28 mai 2017, 14:31
 */

#ifndef GEGL_HEAVY_DEBUG_H
#define GEGL_HEAVY_DEBUG_H

#include <glib.h>

G_BEGIN_DECLS
        
extern gint _gegl_hd_indent;

void gegl_hd_init();


#define GEGL_HD_TRACE() do {                    \
for(int i = 0; i < _gegl_hd_indent; i++)        \
    g_print(" ");                               \
    g_print()                                            \
        
G_END_DECLS


#endif 

