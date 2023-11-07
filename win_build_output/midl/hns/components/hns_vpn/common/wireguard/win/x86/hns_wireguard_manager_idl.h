

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.xx.xxxx */
/* at a redacted point in time
 */
/* Compiler settings for ../../hns/components/hns_vpn/common/wireguard/win/hns_wireguard_manager_idl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.xx.xxxx 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __hns_wireguard_manager_idl_h__
#define __hns_wireguard_manager_idl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IHnsVpnWireguardManager_FWD_DEFINED__
#define __IHnsVpnWireguardManager_FWD_DEFINED__
typedef interface IHnsVpnWireguardManager IHnsVpnWireguardManager;

#endif 	/* __IHnsVpnWireguardManager_FWD_DEFINED__ */


#ifndef __IHnsVpnWireguardManager_FWD_DEFINED__
#define __IHnsVpnWireguardManager_FWD_DEFINED__
typedef interface IHnsVpnWireguardManager IHnsVpnWireguardManager;

#endif 	/* __IHnsVpnWireguardManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IHnsVpnWireguardManager_INTERFACE_DEFINED__
#define __IHnsVpnWireguardManager_INTERFACE_DEFINED__

/* interface IHnsVpnWireguardManager */
/* [unique][helpstring][uuid][oleautomation][object] */ 


EXTERN_C const IID IID_IHnsVpnWireguardManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("053057AB-CF06-4E6C-BBAD-F8DA6436D933")
    IHnsVpnWireguardManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnableVpn( 
            /* [string][in] */ const WCHAR *config,
            /* [out] */ DWORD *last_error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisableVpn( 
            /* [out] */ DWORD *last_error) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GenerateKeypair( 
            /* [out] */ BSTR *public_key,
            /* [out] */ BSTR *private_key,
            /* [out] */ DWORD *last_error) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHnsVpnWireguardManagerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHnsVpnWireguardManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHnsVpnWireguardManager * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHnsVpnWireguardManager * This);
        
        DECLSPEC_XFGVIRT(IHnsVpnWireguardManager, EnableVpn)
        HRESULT ( STDMETHODCALLTYPE *EnableVpn )( 
            IHnsVpnWireguardManager * This,
            /* [string][in] */ const WCHAR *config,
            /* [out] */ DWORD *last_error);
        
        DECLSPEC_XFGVIRT(IHnsVpnWireguardManager, DisableVpn)
        HRESULT ( STDMETHODCALLTYPE *DisableVpn )( 
            IHnsVpnWireguardManager * This,
            /* [out] */ DWORD *last_error);
        
        DECLSPEC_XFGVIRT(IHnsVpnWireguardManager, GenerateKeypair)
        HRESULT ( STDMETHODCALLTYPE *GenerateKeypair )( 
            IHnsVpnWireguardManager * This,
            /* [out] */ BSTR *public_key,
            /* [out] */ BSTR *private_key,
            /* [out] */ DWORD *last_error);
        
        END_INTERFACE
    } IHnsVpnWireguardManagerVtbl;

    interface IHnsVpnWireguardManager
    {
        CONST_VTBL struct IHnsVpnWireguardManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHnsVpnWireguardManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHnsVpnWireguardManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHnsVpnWireguardManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHnsVpnWireguardManager_EnableVpn(This,config,last_error)	\
    ( (This)->lpVtbl -> EnableVpn(This,config,last_error) ) 

#define IHnsVpnWireguardManager_DisableVpn(This,last_error)	\
    ( (This)->lpVtbl -> DisableVpn(This,last_error) ) 

#define IHnsVpnWireguardManager_GenerateKeypair(This,public_key,private_key,last_error)	\
    ( (This)->lpVtbl -> GenerateKeypair(This,public_key,private_key,last_error) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHnsVpnWireguardManager_INTERFACE_DEFINED__ */



#ifndef __HnsVpnWireguardServiceLib_LIBRARY_DEFINED__
#define __HnsVpnWireguardServiceLib_LIBRARY_DEFINED__

/* library HnsVpnWireguardServiceLib */
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_HnsVpnWireguardServiceLib;
#endif /* __HnsVpnWireguardServiceLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


