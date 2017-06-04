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


#ifdef HEAVY_DEBUG

#include <stdio.h>

extern int _h_dbg_indent;

#define h_dbg_print(...) printf(__VA_ARGS__)

#define h_dbg_indent_print(...) do {          \
for(int i = 0; i < _h_dbg_indent; i++)        \
    h_dbg_print("  ");                             \
    h_dbg_print("--- ");                           \
    h_dbg_print(__VA_ARGS__);                      \
} while ( 0 )

#define H_DBG_TRACE_BGEIN() do {              \
for(int i = 0; i < _h_dbg_indent; i++)        \
    h_dbg_print("  ");                             \
    h_dbg_print("|%s \n",__FUNCTION__);            \
    _h_dbg_indent++ ;                         \
} while ( 0 );

#define H_DBG_TRACE_END() do {                \   
    _h_dbg_indent--;                          \
} while ( 0 );
        
#else
    
#define h_dbg_print(...) g_print(...)
      
#define h_dbg_indent_print(...) 

#define H_DBG_TRACE_BGEIN() 

#define H_DBG_TRACE_END() 
            
#endif 
    
#endif

