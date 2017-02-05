#!/bin/sh

# Set by TESTS_ENVIRONMENT in Makefile.am
abs_top_srcdir=$ABS_TOP_SRCDIR
abs_top_builddir=$ABS_TOP_BUILDDIR

if [ ! -f $abs_top_builddir/bin/gegl ]; then
  echo "Skipping test-unsharp due to lack of gegl executable"
  exit 77
else
  GEGL_PATH=$abs_top_builddir/operations GEGL_USE_OPENCL=no GEGL_MIPMAP_RENDERING=1 $abs_top_builddir/bin/gegl                                       \
          -s 0.33 $abs_top_srcdir/tests/compositions/data/car-stack.png -o      \
          $abs_top_builddir/tests/mipmap/unsharp-output.png           \
          -- unsharp-mask scale=10.0  \
  && $abs_top_builddir/tools/gegl-imgcmp                           \
          $abs_top_srcdir/tests/mipmap/unsharp-reference.png \
          $abs_top_builddir/tests/mipmap/unsharp-output.png 20.0
  failure=$?
  #if [ $failure -eq 0 ]; then
   # rm -f $abs_top_builddir/tests/mipmap/unsharp-output.png
  #fi
fi

exit $failure
echo "WARNING: The result of this test is being ignored!"
exit 0
