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

#------------------------------------------------------------------------------
# Variables that describe source code dependencies
LSP_COMMON_LIB_VERSION     := 1.0.34
LSP_COMMON_LIB_NAME        := lsp-common-lib
LSP_COMMON_LIB_TYPE        := src
LSP_COMMON_LIB_URL_RO      := https://github.com/lsp-plugins/$(LSP_COMMON_LIB_NAME).git
LSP_COMMON_LIB_URL_RW      := git@github.com:lsp-plugins/$(LSP_COMMON_LIB_NAME).git

LSP_TEST_FW_VERSION        := 1.0.24
LSP_TEST_FW_NAME           := lsp-test-fw
LSP_TEST_FW_TYPE           := src
LSP_TEST_FW_URL_RO         := https://github.com/lsp-plugins/$(LSP_TEST_FW_NAME).git
LSP_TEST_FW_URL_RW         := git@github.com:lsp-plugins/$(LSP_TEST_FW_NAME).git

LSP_R3D_IFACE_VERSION      := 1.0.17
LSP_R3D_IFACE_NAME         := lsp-r3d-iface
LSP_R3D_IFACE_TYPE         := src
LSP_R3D_IFACE_URL_RO       := https://github.com/lsp-plugins/$(LSP_R3D_IFACE_NAME).git
LSP_R3D_IFACE_URL_RW       := git@github.com:lsp-plugins/$(LSP_R3D_IFACE_NAME).git

LSP_R3D_BASE_LIB_VERSION   := 1.0.17
LSP_R3D_BASE_LIB_NAME      := lsp-r3d-base-lib
LSP_R3D_BASE_LIB_TYPE      := src
LSP_R3D_BASE_LIB_URL_RO    := https://github.com/lsp-plugins/$(LSP_R3D_BASE_LIB_NAME).git
LSP_R3D_BASE_LIB_URL_RW    := git@github.com:lsp-plugins/$(LSP_R3D_BASE_LIB_NAME).git

#------------------------------------------------------------------------------
# Variables that describe system dependencies
LIBX11_VERSION             := system
LIBX11_NAME                := x11
LIBX11_TYPE                := pkg

LIBGL_VERSION              := system
LIBGL_NAME                 := gl
LIBGL_TYPE                 := pkg
