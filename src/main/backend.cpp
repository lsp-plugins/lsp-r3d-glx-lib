/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-r3d-glx-lib
 * Created on: 24 апр. 2019 г.
 *
 * lsp-r3d-glx-lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-r3d-glx-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-r3d-glx-lib. If not, see <https://www.gnu.org/licenses/>.
 */

#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/common/debug.h>
#include <lsp-plug.in/r3d/glx/backend.h>
#include <stdlib.h>

namespace lsp
{
    namespace r3d
    {
        namespace glx
        {
            enum buffer_state_t
            {
                DBUF_VINDEX  = 1 << 0,
                DBUF_NORMAL  = 1 << 1,
                DBUF_NINDEX  = 1 << 2,
                DBUF_COLOR   = 1 << 3,
                DBUF_CINDEX  = 1 << 4,

                DBUF_NORMAL_FLAGS = DBUF_NORMAL | DBUF_NINDEX,
                DBUF_COLOR_FLAGS  = DBUF_COLOR  | DBUF_CINDEX,
                DBUF_INDEX_MASK   = DBUF_VINDEX | DBUF_NINDEX | DBUF_CINDEX
            };

            #define VATTR_BUFFER_SIZE       3072    /* Multiple of 3 */

            static GLint rgba24x32[]    = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 32, GLX_DOUBLEBUFFER, None };
            static GLint rgba24x24[]    = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            static GLint rgba16x24[]    = { GLX_RGBA, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 6, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            static GLint rgba15x24[]    = { GLX_RGBA, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 5, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            static GLint rgba16[]       = { GLX_RGBA, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 6, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
            static GLint rgba15[]       = { GLX_RGBA, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 5, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
            static GLint rgbax32[]      = { GLX_RGBA, GLX_DEPTH_SIZE, 32, GLX_DOUBLEBUFFER, None };
            static GLint rgbax24[]      = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            static GLint rgbax16[]      = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
            static GLint rgba[]         = { GLX_RGBA, GLX_DOUBLEBUFFER, None };

            static int pb_rgba24x32[]   = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 32, None };
            static int pb_rgba24x24[]   = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 24, None };
            static int pb_rgba16x24[]   = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 6, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 24, None };
            static int pb_rgba15x24[]   = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 5, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 24, None };
            static int pb_rgba16[]      = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 6, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 16, None };
            static int pb_rgba15[]      = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_RED_SIZE, 5, GLX_GREEN_SIZE, 5, GLX_BLUE_SIZE, 5, GLX_DEPTH_SIZE, 16, None };
            static int pb_rgbax32[]     = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_DEPTH_SIZE, 32, None };
            static int pb_rgbax24[]     = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_DEPTH_SIZE, 24, None };
            static int pb_rgbax16[]     = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, GLX_DEPTH_SIZE, 16, None };
            static int pb_rgba[]        = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT, None };

            static GLint *glx_visuals[] =
            {
                rgba24x32, rgba24x24,
                rgba16x24, rgba16,
                rgba15x24, rgba15,
                rgbax32, rgbax24, rgbax16, rgba,
                NULL
            };

            static const int *glx_pb_config[] =
            {
                pb_rgba24x32, pb_rgba24x24,
                pb_rgba16x24, pb_rgba16,
                pb_rgba15x24, pb_rgba15,
                pb_rgbax32, pb_rgbax24, pb_rgbax16, pb_rgba,
                NULL
            };

            backend_t::backend_t()
            {
                construct();
            }

            void backend_t::construct()
            {
                base_backend_t::construct();

                pDisplay    = NULL;
                hWnd        = None;
                hPBuffer    = None;
                pFBConfig   = NULL;
                hContext    = NULL;
                bVisible    = false;
                bDrawing    = false;
                bPBuffer    = false;

                vxBuffer    = NULL;

                // Export virtual table
                #define R3D_GLX_BACKEND_EXP(func)   r3d::backend_t::func = backend_t::func;
                R3D_GLX_BACKEND_EXP(init_window);
                R3D_GLX_BACKEND_EXP(init_offscreen);
                R3D_GLX_BACKEND_EXP(destroy);
                R3D_GLX_BACKEND_EXP(locate);

                R3D_GLX_BACKEND_EXP(start);
                R3D_GLX_BACKEND_EXP(sync);
                R3D_GLX_BACKEND_EXP(read_pixels);
                R3D_GLX_BACKEND_EXP(finish);

                R3D_GLX_BACKEND_EXP(set_matrix);
                R3D_GLX_BACKEND_EXP(set_lights);
                R3D_GLX_BACKEND_EXP(draw_primitives);

                #undef R3D_GLX_BACKEND_EXP
            }

            void backend_t::destroy(r3d::backend_t *handle)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                // Destroy vertex attributes buffer
                if (_this->vxBuffer != NULL)
                {
                    free(_this->vxBuffer);
                    _this->vxBuffer     = NULL;
                }

                // Destroy pBuffer
                if (_this->hPBuffer != None)
                {
                    ::glXDestroyPbuffer(_this->pDisplay, _this->hPBuffer);
                    _this->hPBuffer     = None;
                }

                // Destroy GLX Context
                if (_this->hContext != NULL)
                {
                    ::glXDestroyContext(_this->pDisplay, _this->hContext);
                    _this->hContext    = NULL;
                }

                // Destroy the window
                if (_this->hWnd != None)
                {
                    ::XDestroyWindow(_this->pDisplay, _this->hWnd);
                    _this->hWnd        = None;
                }

                // Destroy X11 display
                if (_this->pDisplay != NULL)
                {
                    ::XSync(_this->pDisplay, False);
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                }

                // Call parent structure for destroy
                r3d::base_backend_t::destroy(handle);
            }

            status_t backend_t::init_window(r3d::backend_t *handle, void **out_window)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                // Check that already initialized
                if (_this->pDisplay != NULL)
                    return STATUS_BAD_STATE;

                // Initialize parent structure
                status_t res = r3d::base_backend_t::init(handle);
                if (res != STATUS_OK)
                    return res;

                // Open display
                _this->pDisplay = ::XOpenDisplay(NULL);
                if (_this->pDisplay == NULL)
                    return STATUS_NO_DEVICE;

                int screen      = DefaultScreen(_this->pDisplay);
                Window root     = RootWindow(_this->pDisplay, screen);

                // Choose GLX visual
                XVisualInfo *vi = NULL;
                for (GLint **visual = glx_visuals; *visual != NULL; ++visual)
                {
                    if ((vi = ::glXChooseVisual(_this->pDisplay, screen, *visual)) != NULL)
                        break;
                }

                lsp_trace("Choosed visual: 0x%lx, red=0x%lx, green=0x%lx, blue=0x%lx",
                        long(vi->visualid), vi->red_mask, vi->green_mask, vi->blue_mask);

                if (vi == NULL)
                {
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                    return STATUS_UNSUPPORTED_DEVICE;
                }

                // Create context
                _this->hContext = ::glXCreateContext(_this->pDisplay, vi, NULL, GL_TRUE);
                if (_this->hContext == NULL)
                {
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                    return STATUS_NO_DEVICE;
                }

                // Create child window
                Colormap cmap   = ::XCreateColormap(_this->pDisplay, root, vi->visual, AllocNone);
                XSetWindowAttributes    swa;

                swa.colormap = cmap;

                // Create the child window
                _this->hWnd = ::XCreateWindow(_this->pDisplay, root, 0, 0, 1, 1, 0, vi->depth, InputOutput, vi->visual, CWColormap, &swa);
                if (_this->hWnd == None)
                {
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                    return STATUS_NO_DEVICE;
                }

                // Flush changes
                ::XFlush(_this->pDisplay);
                ::XSync(_this->pDisplay, False);

                _this->bDrawing     = false;
                _this->bPBuffer     = false;

                // Return result
                if (out_window != NULL)
                    *out_window     = reinterpret_cast<void *>(_this->hWnd);

                return STATUS_OK;
            }

            status_t backend_t::init_offscreen(r3d::backend_t *handle)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                // Check that already initialized
                if (_this->pDisplay != NULL)
                    return STATUS_BAD_STATE;

                // Initialize parent structure
                status_t res = r3d::base_backend_t::init(handle);
                if (res != STATUS_OK)
                    return res;

                // Open display
                _this->pDisplay = ::XOpenDisplay(NULL);
                if (_this->pDisplay == NULL)
                    return STATUS_NO_DEVICE;

                // Choose FB config
                GLXFBConfig *fbc    = NULL;
                int screen          = DefaultScreen(_this->pDisplay);
                int nfbelements;
                for (const int **atts = glx_pb_config; *atts != NULL; ++atts)
                {
                    if ((fbc = ::glXChooseFBConfig(_this->pDisplay, screen, *atts, &nfbelements)) != NULL)
                        break;
                }

                // Success story?
                if ((fbc == NULL) || (nfbelements <= 0))
                {
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                    return STATUS_UNSUPPORTED_DEVICE;
                }

            #ifdef LSP_TRACE
                int r, g, b, a, depth;
                glXGetFBConfigAttrib(_this->pDisplay, fbc[0], GLX_RED_SIZE, &r);
                glXGetFBConfigAttrib(_this->pDisplay, fbc[0], GLX_GREEN_SIZE, &g);
                glXGetFBConfigAttrib(_this->pDisplay, fbc[0], GLX_BLUE_SIZE, &b);
                glXGetFBConfigAttrib(_this->pDisplay, fbc[0], GLX_ALPHA_SIZE, &a);
                glXGetFBConfigAttrib(_this->pDisplay, fbc[0], GLX_DEPTH_SIZE, &depth);
                lsp_trace("Choosed FB config: r=%d, g=%d, b=%d, a=%d, depth=%d",
                        r, g, b, a, depth);
            #endif

                // Create context
                _this->hContext = ::glXCreateNewContext(_this->pDisplay, fbc[0], GLX_RGBA_TYPE, NULL, GL_TRUE);
                if (_this->hContext == NULL)
                {
                    ::XFree(fbc);
                    ::XCloseDisplay(_this->pDisplay);
                    _this->pDisplay    = NULL;
                    return STATUS_NO_DEVICE;
                }

                // Flush changes
                ::XFlush(_this->pDisplay);
                ::XSync(_this->pDisplay, False);

                _this->bDrawing     = false;
                _this->bPBuffer     = true;
                _this->pFBConfig    = fbc;

                return STATUS_OK;
            }

            status_t backend_t::locate(r3d::backend_t *handle, ssize_t left, ssize_t top, ssize_t width, ssize_t height)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (_this->bDrawing))
                    return STATUS_BAD_STATE;

                // Requested size does match current size?
                if (_this->bPBuffer)
                {
                    if ((_this->viewWidth == width) &&
                        (_this->viewHeight == height) &&
                        (_this->hPBuffer != None))
                    {
                        // These attributes don't matter
                        _this->viewLeft    = left;
                        _this->viewTop     = top;
                        return STATUS_OK;
                    }

                    // Destroy previously used pBuffer
                    if (_this->hPBuffer != None)
                    {
                        ::glXDestroyPbuffer(_this->pDisplay, _this->hPBuffer);
                        _this->hPBuffer     = None;
                    }

                    // Create new pBuffer
                    int pbuffer_attributes[]={
                          GLX_PBUFFER_WIDTH, int(width),
                          GLX_PBUFFER_HEIGHT, int(height),
                          GLX_NONE
                        };
                    _this->hPBuffer    = ::glXCreatePbuffer(_this->pDisplay, _this->pFBConfig[0], pbuffer_attributes);
                    if (_this->hPBuffer == None)
                        return STATUS_NO_MEM;
                }
                else
                {
                    if ((_this->viewLeft == left) &&
                        (_this->viewTop == top) &&
                        (_this->viewWidth == width) &&
                        (_this->viewHeight == height))
                        return STATUS_OK;

                    if (!::XMoveResizeWindow(_this->pDisplay, _this->hWnd, left, top, width, height))
                        return STATUS_UNKNOWN_ERR;
                    ::XFlush(_this->pDisplay);
                    ::XSync(_this->pDisplay, False);
                }

                // Update parameters
                _this->viewLeft    = left;
                _this->viewTop     = top;
                _this->viewWidth   = width;
                _this->viewHeight  = height;

                return STATUS_OK;
            }

            status_t backend_t::start(r3d::backend_t *handle)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (_this->bDrawing))
                    return STATUS_BAD_STATE;

                // Setup current GLX context
                if (_this->bPBuffer)
                {
                    ::glXMakeContextCurrent(_this->pDisplay, _this->hPBuffer, _this->hPBuffer, _this->hContext);
                    ::glXWaitX();
                    ::glDrawBuffer(GL_FRONT);
                }
                else
                {
                    ::glXMakeCurrent(_this->pDisplay, _this->hWnd, _this->hContext);
                    ::glXWaitX();
                    ::glDrawBuffer(GL_BACK);
                }

                ::glViewport(0, 0, _this->viewWidth, _this->viewHeight);

                // Enable depth test and culling
                ::glDepthFunc(GL_LEQUAL);
                ::glEnable(GL_DEPTH_TEST);
                ::glEnable(GL_CULL_FACE);
                ::glCullFace(GL_BACK);
                ::glEnable(GL_COLOR_MATERIAL);

                // Tune lighting
                ::glShadeModel(GL_SMOOTH);
                ::glEnable(GL_RESCALE_NORMAL);

                // Special tuning for non-poligonal primitives
                ::glPolygonOffset(1.0f, 2.0f);
                ::glEnable(GL_POLYGON_OFFSET_POINT);
                ::glEnable(GL_POLYGON_OFFSET_FILL);
                ::glEnable(GL_POLYGON_OFFSET_LINE);

                // Clear buffer
                ::glClearColor(_this->colBackground.r, _this->colBackground.g, _this->colBackground.b, _this->colBackground.a);
                ::glClearDepth(1.0);
                ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Mark as started
                _this->bDrawing        = true;

                return STATUS_OK;
            }

            status_t backend_t::set_matrix(r3d::backend_t *handle, r3d::matrix_type_t type, const r3d::mat4_t *m)
            {
                return r3d::base_backend_t::set_matrix(handle, type, m);
            }

            status_t backend_t::set_lights(r3d::backend_t *handle, const r3d::light_t *lights, size_t count)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (!_this->bDrawing))
                    return STATUS_BAD_STATE;

                // Enable all possible lights
                size_t light_id = GL_LIGHT0;

                ::glMatrixMode(GL_MODELVIEW);
                ::glPushMatrix();
                ::glLoadIdentity();

                for (size_t i=0; i<count; ++i)
                {
                    // Skip disabled lights
                    if (lights[i].type == r3d::LIGHT_NONE)
                        continue;

                    // Enable the light and set basic attributes
                    r3d::vec4_t position;

                    ::glEnable(light_id);
                    ::glLightfv(light_id, GL_AMBIENT, &lights[i].ambient.r);
                    ::glLightfv(light_id, GL_DIFFUSE, &lights[i].diffuse.r);
                    ::glLightfv(light_id, GL_SPECULAR, &lights[i].specular.r);

                    switch (lights[i].type)
                    {
                        case r3d::LIGHT_POINT:
                            position.dx     = lights[i].position.x;
                            position.dy     = lights[i].position.y;
                            position.dz     = lights[i].position.z;
                            position.dw     = 1.0f;
                            ::glLightfv(light_id, GL_POSITION, &position.dx);
                            ::glLighti(light_id, GL_SPOT_CUTOFF, 180);
                            break;
                        case r3d::LIGHT_DIRECTIONAL:
                            position.dx     = lights[i].direction.dx;
                            position.dy     = lights[i].direction.dy;
                            position.dz     = lights[i].direction.dz;
                            position.dw     = 0.0f;
                            ::glLightfv(light_id, GL_POSITION, &position.dx);
                            ::glLighti(light_id, GL_SPOT_CUTOFF, 180);
                            break;
                        case r3d::LIGHT_SPOT:
                            position.dx     = lights[i].position.x;
                            position.dy     = lights[i].position.y;
                            position.dz     = lights[i].position.z;
                            position.dw     = 1.0f;
                            ::glLightfv(light_id, GL_POSITION, &position.dx);
                            ::glLightfv(light_id, GL_SPOT_DIRECTION, &lights[i].direction.dx);
                            ::glLightf(light_id, GL_SPOT_CUTOFF, lights[i].cutoff);
                            ::glLightf(light_id, GL_CONSTANT_ATTENUATION, lights[i].constant);
                            ::glLightf(light_id, GL_LINEAR_ATTENUATION, lights[i].linear);
                            ::glLightf(light_id, GL_QUADRATIC_ATTENUATION, lights[i].quadratic);
                            break;
                        default:
                            return STATUS_INVALID_VALUE;
                    }

                    // Ignore all lights that are out of 8 basic lights
                    if (++light_id > GL_LIGHT7)
                        break;
                }

                // Disable all other non-related lights
                while (light_id <= GL_LIGHT7)
                    ::glDisable(light_id++);

                ::glPopMatrix();

                return STATUS_OK;
            }

            void gl_draw_arrays_simple(GLenum mode, size_t bstate, const r3d::buffer_t *buffer, size_t count)
            {
                // Enable vertex pointer (if present)
                ::glEnableClientState(GL_VERTEX_ARRAY);
                ::glVertexPointer(4, GL_FLOAT,
                    (buffer->vertex.stride == 0) ? sizeof(r3d::dot4_t) : buffer->vertex.stride,
                    buffer->vertex.data
                );

                // Enable normal pointer
                if (bstate & DBUF_NORMAL)
                {
                    ::glEnableClientState(GL_NORMAL_ARRAY);
                    ::glNormalPointer(GL_FLOAT,
                        (buffer->normal.stride == 0) ? sizeof(r3d::vec4_t) : buffer->normal.stride,
                        buffer->normal.data
                    );
                }
                else
                    ::glDisableClientState(GL_NORMAL_ARRAY);

                // Enable color pointer
                if (bstate & DBUF_COLOR)
                {
                    ::glEnableClientState(GL_COLOR_ARRAY);
                    ::glColorPointer(4, GL_FLOAT,
                        (buffer->color.stride == 0) ? sizeof(r3d::color_t) : buffer->color.stride,
                        buffer->color.data
                    );
                }
                else
                {
                    ::glColor4fv(&buffer->color.dfl.r);         // Set-up default color
                    ::glDisableClientState(GL_COLOR_ARRAY);
                }

                // Draw the elements (or arrays, depending on configuration)
                if (buffer->type != r3d::PRIMITIVE_WIREFRAME_TRIANGLES)
                {
                    if (bstate & DBUF_VINDEX)
                        ::glDrawElements(mode, count, GL_UNSIGNED_INT, buffer->vertex.index);
                    else
                        ::glDrawArrays(mode, 0, count);
                }
                else
                {
                    if (bstate & DBUF_VINDEX)
                    {
                        const uint32_t *ptr = buffer->vertex.index;
                        for (size_t i=0; i<count; i += 3, ptr += 3)
                            ::glDrawElements(mode, 3, GL_UNSIGNED_INT, ptr);
                    }
                    else
                    {
                        for (size_t i=0; i<count; i += 3)
                            ::glDrawArrays(mode, i, 3);
                    }
                }

                // Disable previous settings
                if (bstate & DBUF_COLOR)
                    ::glDisableClientState(GL_COLOR_ARRAY);
                if (bstate & DBUF_NORMAL)
                    ::glDisableClientState(GL_NORMAL_ARRAY);
                ::glDisableClientState(GL_VERTEX_ARRAY);
            }

            void gl_draw_arrays_indexed(r3d::backend_t *handle, GLenum mode, size_t bstate, const r3d::buffer_t *buffer, size_t count)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                // Lazy initialization: allocate temporary buffer
                if (_this->vxBuffer == NULL)
                {
                    _this->vxBuffer = reinterpret_cast<vertex_t *>(malloc(VATTR_BUFFER_SIZE * sizeof(vertex_t)));
                    if (_this->vxBuffer == NULL)
                        return;
                }

                // Enable vertex pointer
                ::glEnableClientState(GL_VERTEX_ARRAY);
                ::glVertexPointer(4, GL_FLOAT, sizeof(vertex_t), &_this->vxBuffer->v);

                // Enable normal pointer
                if (bstate & DBUF_NORMAL)
                {
                    ::glEnableClientState(GL_NORMAL_ARRAY);
                    ::glNormalPointer(GL_FLOAT, sizeof(vertex_t), &_this->vxBuffer->n);
                }
                else
                    ::glDisableClientState(GL_NORMAL_ARRAY);

                // Enable color pointer
                if (bstate & DBUF_COLOR)
                {
                    ::glEnableClientState(GL_COLOR_ARRAY);
                    ::glColorPointer(4, GL_FLOAT, sizeof(vertex_t), &_this->vxBuffer->c);
                }
                else
                {
                    ::glColor4fv(&buffer->color.dfl.r);         // Set-up default color
                    ::glDisableClientState(GL_COLOR_ARRAY);
                }

                // Compute stride values and indices
                const uint32_t *vindex  = buffer->vertex.index;
                const uint32_t *nindex  = buffer->normal.index;
                const uint32_t *cindex  = buffer->color.index;
                const uint8_t  *vbuf    = reinterpret_cast<const uint8_t *>(buffer->vertex.data);
                const uint8_t  *nbuf    = reinterpret_cast<const uint8_t *>(buffer->normal.data);
                const uint8_t  *cbuf    = reinterpret_cast<const uint8_t *>(buffer->color.data);
                size_t vstride          = (buffer->vertex.stride == 0) ? sizeof(r3d::dot4_t)  : buffer->vertex.stride;
                size_t nstride          = (buffer->normal.stride == 0) ? sizeof(r3d::vec4_t)  : buffer->normal.stride;
                size_t cstride          = (buffer->color.stride == 0)  ? sizeof(r3d::color_t) : buffer->color.stride;

                for (size_t off = 0; off < count; )
                {
                    size_t to_do    = count - off;
                    if (to_do > VATTR_BUFFER_SIZE)
                        to_do           = VATTR_BUFFER_SIZE;

                    // Fill the temporary buffer data
                    vertex_t *vx    = _this->vxBuffer;
                    for (size_t i=0; i<to_do; ++i, ++vx)
                    {
                        size_t vxi      = off + i;

                        // Add vertex coordinates
                        if (bstate & DBUF_VINDEX)
                            vx->v       = *(reinterpret_cast<const dot4_t *>(&vbuf[vindex[vxi] * vstride]));
                        else
                            vx->v       = *(reinterpret_cast<const dot4_t *>(&vbuf[vxi * vstride]));

                        // Add normal coordinates if present
                        if (bstate & DBUF_NORMAL)
                        {
                            if (bstate & DBUF_NINDEX)
                                vx->n       = *(reinterpret_cast<const vec4_t *>(&nbuf[nindex[vxi] * nstride]));
                            else
                                vx->n       = *(reinterpret_cast<const vec4_t *>(&nbuf[vxi * nstride]));
                        }

                        // Add color coordinates if present
                        if (bstate & DBUF_COLOR)
                        {
                            if (bstate & DBUF_CINDEX)
                                vx->c       = *(reinterpret_cast<const color_t *>(&cbuf[cindex[vxi] * cstride]));
                            else
                                vx->c       = *(reinterpret_cast<const color_t *>(&cbuf[vxi * cstride]));
                        }
                    }

                    // Draw the buffer
                    if (buffer->type != r3d::PRIMITIVE_WIREFRAME_TRIANGLES)
                        ::glDrawArrays(mode, 0, count);
                    else
                    {
                        for (size_t i=0; i<count; i += 3)
                            ::glDrawArrays(mode, i, 3);
                    }

                    // Update offset
                    off            += to_do;
                }

                // Disable previous settings
                if (bstate & DBUF_COLOR)
                    ::glDisableClientState(GL_COLOR_ARRAY);
                if (bstate & DBUF_NORMAL)
                    ::glDisableClientState(GL_NORMAL_ARRAY);
                ::glDisableClientState(GL_VERTEX_ARRAY);
            }

            status_t backend_t::draw_primitives(r3d::backend_t *handle, const r3d::buffer_t *buffer)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if (buffer == NULL)
                    return STATUS_BAD_ARGUMENTS;
                if ((_this->pDisplay == NULL) || (!_this->bDrawing))
                    return STATUS_BAD_STATE;

                // Is there any data to draw?
                if (buffer->count <= 0)
                    return STATUS_OK;

                //-------------------------------------------------------------
                // Select the drawing mode

                // Check primitive type to draw
                GLenum mode  = GL_TRIANGLES;
                size_t count = buffer->count;

                switch (buffer->type)
                {
                    case r3d::PRIMITIVE_TRIANGLES:
                        mode    = GL_TRIANGLES;
                        count   = (count << 1) + count; // count *= 3
                        break;
                    case r3d::PRIMITIVE_WIREFRAME_TRIANGLES:
                        mode    = GL_LINE_LOOP;
                        count   = (count << 1) + count; // count *= 3
                        ::glLineWidth(buffer->width);
                        break;
                    case r3d::PRIMITIVE_LINES:
                        mode    = GL_LINES;
                        count <<= 1;                    // count *= 2
                        ::glLineWidth(buffer->width);
                        break;
                    case r3d::PRIMITIVE_POINTS:
                        mode    = GL_POINTS;
                        ::glPointSize(buffer->width);
                        break;
                    default:
                        return STATUS_BAD_ARGUMENTS;
                }

                size_t bstate = 0;
                if (buffer->vertex.data == NULL)
                    return STATUS_BAD_ARGUMENTS;
                if (buffer->vertex.index != NULL)
                    bstate     |= DBUF_VINDEX;

                if (buffer->normal.data != NULL)
                    bstate     |= DBUF_NORMAL;
                if (buffer->normal.index != NULL)
                    bstate     |= DBUF_NINDEX;

                if (buffer->color.data != NULL)
                    bstate     |= DBUF_COLOR;
                if (buffer->color.index != NULL)
                    bstate     |= DBUF_CINDEX;

                if (((bstate & DBUF_NORMAL_FLAGS) == DBUF_NINDEX) ||
                    ((bstate & DBUF_COLOR_FLAGS) == DBUF_CINDEX))
                    return STATUS_BAD_ARGUMENTS; // Index buffers can not be definde without data buffers

                //-------------------------------------------------------------
                // Prepare drawing state
                // Load matrices
                ::glMatrixMode(GL_PROJECTION);
                ::glLoadMatrixf(_this->matProjection.m);
                ::glMatrixMode(GL_MODELVIEW);
                ::glLoadMatrixf(_this->matView.m);
                ::glMultMatrixf(_this->matWorld.m);
                ::glMultMatrixf(buffer->model.m);

                // enable blending
                if (buffer->flags & r3d::BUFFER_BLENDING)
                {
                    ::glEnable(GL_BLEND);
                    if (buffer->flags & r3d::BUFFER_STD_BLENDING)
                        ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    else
                        ::glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
                }
                if (buffer->flags & r3d::BUFFER_LIGHTING)
                    ::glEnable(GL_LIGHTING);
                if (buffer->flags & r3d::BUFFER_NO_CULLING)
                    ::glDisable(GL_CULL_FACE);

                //-------------------------------------------------------------
                // Draw the buffer data
                if (!(bstate & (DBUF_NINDEX | DBUF_CINDEX)))
                    gl_draw_arrays_simple(mode, bstate, buffer, count);
                else
                    gl_draw_arrays_indexed(handle, mode, bstate, buffer, count);

                //-------------------------------------------------------------
                // Reset the drawing state
                if (buffer->flags & r3d::BUFFER_BLENDING)
                    ::glDisable(GL_BLEND);
                if (buffer->flags & r3d::BUFFER_LIGHTING)
                    ::glDisable(GL_LIGHTING);
                if (buffer->flags & r3d::BUFFER_NO_CULLING)
                    ::glEnable(GL_CULL_FACE);

                return STATUS_OK;
            }

            status_t backend_t::sync(r3d::backend_t *handle)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (!_this->bDrawing))
                    return STATUS_BAD_STATE;

                ::glXWaitGL();

                return STATUS_OK;
            }

            status_t backend_t::read_pixels(r3d::backend_t *handle, void *buf, r3d::pixel_format_t format)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (!_this->bDrawing))
                    return STATUS_BAD_STATE;

                size_t fmt;
                size_t row_size;
                switch (format)
                {
                    case r3d::PIXEL_RGBA:
                        fmt         = GL_RGBA;
                        row_size    = _this->viewWidth * 4;
                        break;
                    case r3d::PIXEL_BGRA:
                        fmt         = GL_BGRA;
                        row_size    = _this->viewWidth * 4;
                        break;
                    case r3d::PIXEL_RGB:
                        fmt         = GL_RGB;
                        row_size    = _this->viewWidth * 3;
                        break;
                    case r3d::PIXEL_BGR:
                        fmt         = GL_BGR;
                        row_size    = _this->viewWidth * 3;
                        break;
                    default:
                        return STATUS_BAD_ARGUMENTS;
                }

                ::glReadBuffer(_this->bPBuffer ? GL_BACK : GL_FRONT);
                ::glReadPixels(0, 0, _this->viewWidth, _this->viewHeight, fmt, GL_UNSIGNED_BYTE, buf);
                base_backend_t::swap_rows(buf, _this->viewHeight, row_size);

                return STATUS_OK;
            }

            status_t backend_t::finish(r3d::backend_t *handle)
            {
                backend_t *_this = static_cast<backend_t *>(handle);

                if ((_this->pDisplay == NULL) || (!_this->bDrawing))
                    return STATUS_BAD_STATE;

                if (!_this->bPBuffer)
                    ::glXSwapBuffers(_this->pDisplay, _this->hWnd);

                ::glXWaitGL();
                _this->bDrawing    = false;

                return STATUS_OK;
            }
        }
    }
}

