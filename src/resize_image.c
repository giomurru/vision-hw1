#include <math.h>
#include "image.h"
#include <assert.h>
float nn_interpolate(image im, float x, float y, int c)
{
    int ry = (int) round(y);
    int rx = (int) round(x);
    rx = rx < im.w ? rx : im.w - 1;
    ry = ry < im.h ? ry : im.h - 1;
    assert(rx < im.w && ry < im.h);
    int idx = c * im.w * im.h + ry*im.w + rx;
    return im.data[idx];
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image rsz = make_image(w, h, im.c);
    for (int x=0; x < rsz.w; x++) {
        for (int y=0; y < rsz.h; y++) {
            for (int c=0; c < rsz.c; c++) {
                int idx = c * rsz.w * rsz.h + y*rsz.w + x;
                float xx = x * (float)im.w/(float)rsz.w;
                float yy = y * (float)im.h/(float)rsz.h;
                rsz.data[idx]=nn_interpolate(im, xx, yy, c);
            }
        }
    }
    return rsz;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

