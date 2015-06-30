#pragma once

// 308C23E6-1051-4794-B60C-4616A74977B9
DEFINE_GUID(CLSID_Valve3, 
0x308c23e6, 0x1051, 0x4794, 0xb6, 0xc, 0x46, 0x16, 0xa7, 0x49, 0x77, 0xb9);

// E43B0F99-8D3F-41B1-BBF2-96351094342F
DEFINE_GUID(CLSID_Valve3PropertyPage, 
0xe43b0f99, 0x8d3f, 0x41b1, 0xbb, 0xf2, 0x96, 0x35, 0x10, 0x94, 0x34, 0x2f);

struct Valve3Parameters {
	// TODO: insert your own transform parameters here
	int param1;
};

#ifdef __cplusplus
extern "C" {
#endif

// 03059F86-661B-4525-A0B0-95FDCCF58BA0
DEFINE_GUID(IID_IValve3, 
0x3059f86, 0x661b, 0x4525, 0xa0, 0xb0, 0x95, 0xfd, 0xcc, 0xf5, 0x8b, 0xa0);

DECLARE_INTERFACE_(IValve3, IUnknown)
{
    STDMETHOD(get_Valve3) (THIS_
                Valve3Parameters *irp
             ) PURE;

    STDMETHOD(put_Valve3) (THIS_
                Valve3Parameters irp
             ) PURE;
};

#ifdef __cplusplus
}
#endif

