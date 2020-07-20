/*
 * version.h
 *
 *  Created on: 3 мая 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_GLX_VERSION_H_
#define LSP_PLUG_IN_GLX_VERSION_H_

#define LSP_R3D_GLX_LIB_MAJOR           0
#define LSP_R3D_GLX_LIB_MINOR           5
#define LSP_R3D_GLX_LIB_MICRO           2

#ifdef LSP_R3D_GLX_LIB_BUILTIN
    #define LSP_R3D_GLX_LIB_EXPORT
    #define LSP_R3D_GLX_LIB_CEXPORT
    #define LSP_R3D_GLX_LIB_IMPORT          LSP_SYMBOL_IMPORT
    #define LSP_R3D_GLX_LIB_CIMPORT         LSP_CSYMBOL_IMPORT
#else
    #define LSP_R3D_GLX_LIB_EXPORT          LSP_SYMBOL_EXPORT
    #define LSP_R3D_GLX_LIB_CEXPORT         LSP_CSYMBOL_EXPORT
    #define LSP_R3D_GLX_LIB_IMPORT          LSP_SYMBOL_IMPORT
    #define LSP_R3D_GLX_LIB_CIMPORT         LSP_CSYMBOL_IMPORT
#endif


#endif /* LSP_PLUG_IN_GLX_VERSION_H_ */
