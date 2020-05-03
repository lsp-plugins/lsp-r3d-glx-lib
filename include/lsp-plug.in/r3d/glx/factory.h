/*
 * factory.h
 *
 *  Created on: 24 апр. 2019 г.
 *      Author: sadko
 */

#ifndef PRIVATE_GLX_FACTORY_H_
#define PRIVATE_GLX_FACTORY_H_

#include <lsp-plug.in/r3d/glx/version.h>
#include <lsp-plug.in/r3d/factory.h>

namespace lsp
{
    namespace glx
    {
        // GLX backend factory
        typedef struct factory_t: public r3d::factory_t
        {
            static const r3d::backend_metadata_t    sMetadata[];

            static const r3d::backend_metadata_t   *metadata(r3d::factory_t *_this, size_t id);
            static r3d::backend_t                  *create(r3d::factory_t *_this, size_t id);

            explicit factory_t();
            ~factory_t();

        } factory_t;
    }
}

#endif /* PRIVATE_GLX_FACTORY_H_ */
