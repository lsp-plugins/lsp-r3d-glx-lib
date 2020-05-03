/*
 * backend.h
 *
 *  Created on: 18 апр. 2019 г.
 *      Author: sadko
 */

#ifndef PRIVATE_GLX_BACKEND_H_
#define PRIVATE_GLX_BACKEND_H_

// Include base backend definitiona
#include <lsp-plug.in/r3d/glx/version.h>
#include <lsp-plug.in/r3d/base.h>

// Standard libraries
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace lsp
{
    namespace glx
    {
        typedef struct backend_t: public r3d::base_backend_t
        {
            Display        *pDisplay;
            Window          hWnd;
            GLXPbuffer      hPBuffer;
            GLXContext      hContext;
            GLXFBConfig    *pFBConfig;
            bool            bVisible;
            bool            bDrawing;
            bool            bPBuffer;

            void            init();
            static void     destroy(r3d::backend_t *handle);
            static status_t init_window(r3d::backend_t *handle, void **out_window);
            static status_t init_offscreen(r3d::backend_t *handle);

            static status_t locate(r3d::backend_t *handle, ssize_t left, ssize_t top, ssize_t width, ssize_t height);
            static status_t start(r3d::backend_t *handle);
            static status_t set_matrix(r3d::backend_t *handle, r3d::matrix_type_t type, const r3d::mat4_t *m);
            static status_t set_lights(r3d::backend_t *handle, const r3d::light_t *lights, size_t count);
            static status_t draw_primitives(r3d::backend_t *handle, const r3d::buffer_t *buffer);
            static status_t sync(r3d::backend_t *handle);
            static status_t read_pixels(r3d::backend_t *handle, void *buf, size_t stride, r3d::pixel_format_t format);
            static status_t finish(r3d::backend_t *handle);

        } backend_t;
    }
}

#endif /* PRIVATE_GLX_BACKEND_H_ */