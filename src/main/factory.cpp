/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-r3d-glx-lib
 * Created on: 18 апр. 2019 г.
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
#include <lsp-plug.in/r3d/glx/backend.h>
#include <lsp-plug.in/r3d/glx/factory.h>

#include <stdlib.h>

namespace lsp
{
    namespace r3d
    {
        namespace glx
        {
            const r3d::backend_metadata_t factory_t::sMetadata[] =
            {
                { "glx_2x", "openGL 2.0+ (GLX)", "glx_opengl_v2" }
            };

            const r3d::backend_metadata_t *factory_t::metadata(r3d::factory_t *handle, size_t id)
            {
                size_t count = sizeof(sMetadata) / sizeof(r3d::backend_metadata_t);
                return (id < count) ? &sMetadata[id] : NULL;
            }

            r3d::backend_t *factory_t::create(r3d::factory_t *handle, size_t id)
            {
                if (id == 0)
                {
                    glx::backend_t *res = static_cast<glx::backend_t *>(::malloc(sizeof(glx::backend_t)));
                    if (res != NULL)
                        res->construct();
                    return res;
                }
                return NULL;
            }

            factory_t::factory_t()
            {
                #define R3D_GLX_FACTORY_EXP(func)   r3d::factory_t::func = factory_t::func;
                R3D_GLX_FACTORY_EXP(create);
                R3D_GLX_FACTORY_EXP(metadata);
                #undef R3D_GLX_FACTORY_EXP
            }

            factory_t::~factory_t()
            {
            }
        }
    }
}


