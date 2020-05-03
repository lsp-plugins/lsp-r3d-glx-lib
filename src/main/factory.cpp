/*
 * glx.cpp
 *
 *  Created on: 18 апр. 2019 г.
 *      Author: sadko
 */

#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/r3d/glx/backend.h>
#include <lsp-plug.in/r3d/glx/factory.h>

#include <stdlib.h>

namespace lsp
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
                if (res == NULL)
                    return NULL;

                res->init();
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


