/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-r3d-glx-lib
 * Created on: 3 мая 2020 г.
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
#include <lsp-plug.in/r3d/glx/factory.h>
#include <lsp-plug.in/r3d/iface/builtin.h>

namespace lsp
{
    namespace r3d
    {
        namespace glx
        {
            factory_t   factory;

            // Builtin factory binding
            LSP_R3D_BUILTIN_FACTORY(builtin_glx_factory, &lsp::r3d::glx::factory);

            // Version function
            LSP_R3D_GLX_LIB_CEXPORT
            LSP_DEFINE_VERSION_FUNC(LSP_R3D_GLX_LIB)

            // Function that returns factory
            LSP_R3D_GLX_LIB_CEXPORT
            LSP_R3D_BULTIN_FACTORY_FUNCTION
        }
    }
}
