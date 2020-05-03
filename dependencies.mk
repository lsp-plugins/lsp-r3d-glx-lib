# Variables that describe dependencies
LSP_COMMON_LIB_VERSION     := 1.0.3
LSP_COMMON_LIB_NAME        := lsp-common-lib
LSP_COMMON_LIB_URL         := https://github.com/sadko4u/$(LSP_COMMON_LIB_NAME).git
LSP_COMMON_LIB_TYPE        := hdr

LSP_TEST_FW_VERSION        := 1.0.3
LSP_TEST_FW_NAME           := lsp-test-fw
LSP_TEST_FW_URL            := https://github.com/sadko4u/$(LSP_TEST_FW_NAME).git
LSP_TEST_FW_TYPE           := src

LSP_R3D_BASE_LIB_VERSION   := 0.5.0
LSP_R3D_BASE_LIB_NAME      := lsp-r3d-base-lib
LSP_R3D_BASE_LIB_URL       := https://github.com/sadko4u/$(LSP_R3D_BASE_LIB).git
LSP_R3D_BASE_LIB_TYPE      := src

XLIB_VERSION               := system
XLIB_NAME                  := x11
XLIB_TYPE                  := pkg

OPENGL_VERSION             := system
OPENGL_NAME                := gl
OPENGL_TYPE                := pkg

STDLIB_VERSION             := system
STDLIB_LDFLAGS             := -lpthread -ldl