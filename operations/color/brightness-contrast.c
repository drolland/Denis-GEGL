/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 */
#ifdef GEGL_CHANT_PROPERTIES
 
gegl_chant_double (contrast,   -100.0, 100.0, 1.0, "Range scale factor")
gegl_chant_double (brightness,  -10.0,  10.0, 0.0, "Amount to increase brightness")

#else

#define GEGL_CHANT_POINT_FILTER
#define GEGL_CHANT_NAME         brightness_contrast
#define GEGL_CHANT_DESCRIPTION  "Changes the light level and contrast."
#define GEGL_CHANT_SELF         "brightness-contrast.c"
#define GEGL_CHANT_CATEGORIES   "color"
#include "gegl-chant.h"

static gboolean
process (GeglOperation *op,
          void          *in_buf,
          void          *out_buf,
          glong          n_pixels)
{
  ChantInstance *self = GEGL_CHANT_INSTANCE (op);
  gint o;
  gfloat *p = in_buf;  /* it is inplace anyways, and out_but==in_buf) */

  g_assert (in_buf == out_buf);

  for (o=0; o<n_pixels; o++)
    {
      gint i;
      for (i=0;i<3;i++)
        p[i] = (p[i] - 0.5) * self->contrast + self->brightness + 0.5;
      p+=4;
    }
  return TRUE;
}
#endif
