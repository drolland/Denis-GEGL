/* This file is part of GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2007 Øyvind Kolås <pippin@gimp.org>
 */

#ifndef __GEGL_VECTOR_H__
#define __GEGL_VECTOR_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GEGL_TYPE_VECTOR            (gegl_vector_get_type ())
#define GEGL_VECTOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEGL_TYPE_VECTOR, GeglVector))
#define GEGL_VECTOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GEGL_TYPE_VECTOR, GeglVectorClass))
#define GEGL_IS_VECTOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEGL_TYPE_VECTOR))
#define GEGL_IS_VECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GEGL_TYPE_VECTOR))
#define GEGL_VECTOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GEGL_TYPE_VECTOR, GeglVectorClass))

typedef struct _GeglVectorClass  GeglVectorClass;

struct _GeglVector
{
  GObject parent_instance;
};

/* Internally the following structures are used, parts
 * of the internal implementation are exposed through
 * the path access API. The linked list api is currently
 * only used for adding new path interpolators/flatteners
 * with new knot interpretations.
 */

typedef struct Point
{
  gfloat x;
  gfloat y;
} Point;

typedef struct GeglVectorKnot
{
  gchar  type; /* should perhaps be padded out? */
  Point  point[4];
} GeglVectorKnot;

typedef struct GeglVectorPath
{
  GeglVectorKnot         d;
  struct GeglVectorPath *next;
} GeglVectorPath;

struct _GeglVectorClass
{
  GObjectClass parent_class;
  GeglVectorPath *(*flattener) (GeglVectorPath *original);
};


GType        gegl_vector_get_type       (void) G_GNUC_CONST;

GeglVector * gegl_vector_new            (void);


void         gegl_vector_line_to        (GeglVector  *self,
                                         gdouble      x,
                                         gdouble      y);

void         gegl_vector_move_to        (GeglVector *self,
                                         gdouble     x,
                                         gdouble     y);

void         gegl_vector_curve_to       (GeglVector *self,
                                         gdouble     x1,
                                         gdouble     y1,
                                         gdouble     x2,
                                         gdouble     y2,
                                         gdouble     x3,
                                         gdouble     y3);

void         gegl_vector_rel_line_to    (GeglVector  *self,
                                         gdouble      x,
                                         gdouble      y);

void         gegl_vector_rel_move_to    (GeglVector *self,
                                         gdouble     x,
                                         gdouble     y);

void         gegl_vector_rel_curve_to   (GeglVector *self,
                                         gdouble     x1,
                                         gdouble     y1,
                                         gdouble     x2,
                                         gdouble     y2,
                                         gdouble     x3,
                                         gdouble     y3);

void         gegl_vector_get_bounds     (GeglVector   *self,
                                         gdouble      *min_x,
                                         gdouble      *max_x,
                                         gdouble      *min_y,
                                         gdouble      *max_y);

gdouble      gegl_vector_get_length     (GeglVector  *self);


void         gegl_vector_calc           (GeglVector  *self,
                                         gdouble      pos,
                                         gdouble     *x,
                                         gdouble     *y);

void         gegl_vector_calc_values    (GeglVector  *self,
                                         guint        num_samples,
                                         gdouble     *xs,
                                         gdouble     *ys);


#define GEGL_TYPE_PARAM_VECTOR    (gegl_param_vector_get_type ())
#define GEGL_IS_PARAM_VECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEGL_TYPE_PARAM_VECTOR))

GType        gegl_param_vector_get_type (void) G_GNUC_CONST;

GParamSpec * gegl_param_spec_vector     (const gchar *name,
                                         const gchar *nick,
                                         const gchar *blurb,
                                         GeglVector  *default_vector,
                                         GParamFlags  flags);

#include <gegl-buffer.h>

void gegl_vector_fill (GeglBuffer *buffer,
                       GeglVector *vector,
                       GeglColor  *color,
                       gboolean    winding);

void gegl_vector_stroke (GeglBuffer *buffer,
                         GeglVector *vector,
                         GeglColor  *color,
                         gdouble     linewidth,
                         gdouble     hardness);

void gegl_vector_parse_svg_path (GeglVector *vector,
                                 const gchar *path);

void gegl_vector_clear (GeglVector *vector);

gchar * gegl_vector_to_svg_path (GeglVector  *vector);



gint                  gegl_vector_get_knot_count  (GeglVector *vector);
const GeglVectorKnot *gegl_vector_get_knot        (GeglVector *vector,
                                                   gint        pos);

/* -1 means last */
/* pos = 0, pushes the existing 0 if any to 1,
 * passing -1 means add at end
 */

void  gegl_vector_remove_knot  (GeglVector           *vector,
                                gint                  pos);
void  gegl_vector_add_knot     (GeglVector           *vector,
                                gint                  pos,
                                const GeglVectorKnot *knot);
void  gegl_vector_replace_knot (GeglVector           *vector,
                                gint                  pos,
                                const GeglVectorKnot *knot);
void  gegl_vector_knot_foreach (GeglVector           *vector,
                                void (*func) (const GeglVectorKnot *knot,
                                              gpointer              data),
                                gpointer              data);
void  gegl_vector_flat_knot_foreach (GeglVector *vector,
                                     void (*func) (const GeglVectorKnot *knot,
                                                   gpointer              data),
                                     gpointer    data);
#if 0
const GeglMatrix *gegl_vector_get_matrix (GeglVector *vector);
GeglMatrix gegl_vector_set_matrix (GeglVector *vector,
                                   const GeglMatrix *matrix);

#endif


GeglVectorPath *gegl_vector_path_flatten (GeglVectorPath *original);

void gegl_vector_add_flattener (GeglVectorPath *(*func) (GeglVectorPath *original));
void gegl_vector_add_knot_type (gchar type, gint pairs, const gchar *description);

GeglVectorPath * gegl_vector_path_add1 (GeglVectorPath *head,
                                        gchar           type,
                                        gfloat          x,  gfloat  y);
GeglVectorPath * gegl_vector_path_add2 (GeglVectorPath *head,
                                        gchar           type,
                                        gfloat          x,  gfloat y,
                                        gfloat          x1, gfloat y1);
GeglVectorPath * gegl_vector_path_add3 (GeglVectorPath *head,
                                        gchar           type,
                                        gfloat          x,  gfloat y,
                                        gfloat          x1, gfloat y1,
                                        gfloat          x2, gfloat y2);
GeglVectorPath * gegl_vector_path_add4 (GeglVectorPath *head,
                                        gchar           type,
                                        gfloat          x0, gfloat  y0,
                                        gfloat          x1, gfloat  y1,
                                        gfloat          x2, gfloat  y2,
                                        gfloat          x3, gfloat  y3);
GeglVectorPath *gegl_vector_path_destroy (GeglVectorPath *path);

G_END_DECLS

#endif /* __GEGL_VECTOR_H__ */
