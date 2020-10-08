#
# Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
#           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
#
# This file is part of lsp-r3d-glx-lib
#
# lsp-r3d-glx-lib is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# lsp-r3d-glx-lib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with lsp-r3d-glx-lib.  If not, see <https://www.gnu.org/licenses/>.
#

# Package version
ARTIFACT_NAME               = lsp-r3d-glx-lib
ARTIFACT_DESC               = GLX-based 3D backend library for rendering scenes
ARTIFACT_VARS               = LSP_R3D_GLX_LIB
ARTIFACT_HEADERS            = lsp-plug.in
ARTIFACT_EXPORT_ALL         = 0
ARTIFACT_VERSION            = 0.5.3-devel

# List of dependencies
TEST_DEPENDENCIES = \
  LSP_TEST_FW

DEPENDENCIES = \
  STDLIB \
  LSP_COMMON_LIB \
  LSP_R3D_IFACE \
  LSP_R3D_BASE_LIB \
  XLIB \
  OPENGL

ALL_DEPENDENCIES = \
  $(DEPENDENCIES) \
  $(TEST_DEPENDENCIES)
