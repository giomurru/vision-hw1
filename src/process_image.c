#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x >= im.w) {
        x = im.w - 1;
    } else if (x < 0) {
        x = 0;
    }
    if (y >= im.h) {
        y = im.h - 1;
    } else if (y < 0) {
        y = 0;
    }
    if (c >= im.c) {
        c = im.c - 1;
    } else if (c < 0) {
        c = 0;
    }
    
    int i = c * im.w * im.h + y * im.w + x;
    
    return im.data[i];
}

int get_index(image im, int x, int y, int c) {
    return c * im.w * im.h + y * im.w + x;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= im.w || x < 0 || y >= im.h || y < 0 || c >= im.c || c < 0) {
        return;
    }
    int i = get_index(im, x, y, c);
    im.data[i] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    //Y' = 0.299 R' + 0.587 G' + .114 B'
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            int r_index = get_index(im, x, y, 0);
            int g_index = get_index(im, x, y, 1);
            int b_index = get_index(im, x, y, 2);
            int dest_index = r_index;
            gray.data[dest_index] = 0.299 * im.data[r_index] + 0.587 * im.data[g_index] + 0.114 * im.data[b_index];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            int c_index = get_index(im, x, y, c);
            im.data[c_index] += v;
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            for (int c = 0; c < im.c; ++c) {
                int idx = get_index(im, x, y, c);
                im.data[idx] = im.data[idx] < 0 ? 0 : (im.data[idx] > 1 ? 1 : im.data[idx]);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            int r_index = get_index(im, x, y, 0);
            int g_index = get_index(im, x, y, 1);
            int b_index = get_index(im, x, y, 2);
            float r = im.data[r_index];
            float g = im.data[g_index];
            float b = im.data[b_index];
            //compute value v
            float v = three_way_max(r, g, b);
            //compute saturation s
            float m = three_way_min(r, g, b);
            float c = v - m;
            float s = c / v;
            //compute hue h
            float h = 0;
            if (c != 0) {
                float hi;
                if (v == r) {
                    hi = (g - b)/c;
                } else if (v == g) {
                    hi = (b - r)/c + 2;
                } else { //if (v == b)
                    hi = (r - g)/c + 4;
                }
                h = hi/6;
                if (hi < 0) {
                    h += 1;
                }
            }
            im.data[r_index] = h;
            im.data[g_index] = s;
            im.data[b_index] = v;
        }
    }
}

void hsv_to_rgb(image im)
{
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            int h_index = get_index(im, x, y, 0);
            int s_index = get_index(im, x, y, 1);
            int v_index = get_index(im, x, y, 2);
            float h = im.data[h_index];
            float s = im.data[s_index];
            float v = im.data[v_index];
            float c = v * s;
            float hi = h * 6;
            float x = c * (1 - fabs((fmod(hi, 2) - 1)));
            float ri = 0;
            float gi = 0;
            float bi = 0;
            if (hi >= 0 && hi < 1) {
                ri = c;
                gi = x;
                bi = 0;
            } else if (hi >= 1 && hi < 2) {
                ri = x;
                gi = c;
                bi = 0;
            } else if (hi >= 2 && hi < 3) {
                ri = 0;
                gi = c;
                bi = x;
            } else if (hi >= 3 && hi < 4) {
                ri = 0;
                gi = x;
                bi = c;
            } else if (hi >= 4 && hi < 5) {
                ri = x;
                gi = 0;
                bi = c;
            } else if (hi >= 5 && hi < 6) {
                ri = c;
                gi = 0;
                bi = x;
            }
            float m = v - c;
            im.data[h_index] = ri + m;
            im.data[s_index] = gi + m;
            im.data[v_index] = bi + m;
        }
    }
}

void scale_image(image im, int c, float v) {
    for (int x = 0; x < im.w; ++x) {
        for (int y = 0; y < im.h; ++y) {
            int idx = get_index(im, x, y, c);
            im.data[idx] = im.data[idx]*v;
        }
    }
}
