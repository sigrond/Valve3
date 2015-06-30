#ifndef WIN32_LEAN_AND_MEAN//magiczna likwidacja konfliktow definicji
#define WIN32_LEAN_AND_MEAN
#endif
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501


#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wsock32.lib")


#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <process.h>

//poczatek oryginalnych naglowkow
#include <streams.h>     // DirectShow (includes windows.h)
#include <initguid.h>    // declares DEFINE_GUID to declare an EXTERN_C const.
#include <tchar.h>
#include <stdio.h>
#include "iValve3.h"
#include "Valve3.h"
#include "Valve3Prop.h"
#include <stdio.h>
#include <sys/stat.h> 
#include <cstring>
#include <sys/types.h>

#include <objbase.h>
#include <combaseapi.h>
#include <OleCtl.h>
#include <atlbase.h>

#pragma comment(lib,"Strmiids.lib")
#pragma comment(lib,"Strmbasd.lib")
#pragma comment(lib,"winmm.lib")




#define TRANSFORM_NAME L"Valve_echo Filter"

using namespace std;//jesli by cos nie dzialalo, tzn, ze trzeba dawac idywidualnie do polecen tego wymagajacych

bool bo1=false,bo2=false;

// setup data - allows the self-registration to work.
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{ &MEDIATYPE_NULL        // clsMajorType
, &MEDIASUBTYPE_NULL };  // clsMinorType

const AMOVIESETUP_PIN psudPins[] =
{ { L"Input"            // strName
  , FALSE               // bRendered
  , FALSE               // bOutput
  , FALSE               // bZero
  , FALSE               // bMany
  , &CLSID_NULL         // clsConnectsToFilter
  , L""                 // strConnectsToPin
  , 1                   // nTypes
  , &sudPinTypes        // lpTypes
  }
, { L"Output"           // strName
  , FALSE               // bRendered
  , TRUE                // bOutput
  , FALSE               // bZero
  , FALSE               // bMany
  , &CLSID_NULL         // clsConnectsToFilter
  , L""                 // strConnectsToPin
  , 1                   // nTypes
  , &sudPinTypes        // lpTypes
  }
};

const AMOVIESETUP_FILTER sudValve3 =
{ &CLSID_Valve3   // clsID
, TRANSFORM_NAME					// strName
, MERIT_DO_NOT_USE					// dwMerit
, 2									// nPins
, psudPins };						// lpPin

// Needed for the CreateInstance mechanism
CFactoryTemplate g_Templates[]=
    {   { TRANSFORM_NAME
        , &CLSID_Valve3
        , CValve3::CreateInstance
        , NULL
        , &sudValve3 },
		{ TRANSFORM_NAME L" Properties"
		, &CLSID_Valve3PropertyPage
		, CValve3Properties::CreateInstance }
    };
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);

//
// WriteProfileInt
//
// Writes an integer to the profile.
//
void WriteProfileInt(TCHAR *section, TCHAR *key, int i)
{
	TCHAR str[80];
	_stprintf(str, TEXT("%d"), i);
	WriteProfileString(section, key, str);
}

//
// CValve3
//
// Constructor; reads default parameters from registry
//
CValve3::CValve3(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	: CTransInPlaceFilter (tszName, punk, CLSID_Valve3, phr), 
      CPersistStream(punk, phr)
{
	// TODO: read parameters from profile
	m_Valve3Parameters.param1 = GetProfileInt(TEXT("Valve3"), TEXT("param1"), 0);
}

//
// ~CValve3
//
// Destructor; saves parameters to registry
//
CValve3::~CValve3()
{
	// TODO: write parameters from profile
	WriteProfileInt(TEXT("Valve3"), TEXT("param1"), m_Valve3Parameters.param1);
}

//
// Transform
//
// Transforms the media sample in-place
//
//*******************************************************************************//
HANDLE hPThread;
void __cdecl PortThread(void* Args)
{
	bo1=true;
//begin
WSADATA wsaData;
int iResult;
iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
struct addrinfo *result = NULL, *ptr = NULL, hints;
ZeroMemory(&hints, sizeof (hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_flags = AI_PASSIVE;
#define DEFAULT_PORT "27016"
iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
SOCKET ListenSocket = INVALID_SOCKET;
ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
freeaddrinfo(result);
if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
    //printf( "Listen failed with error: %ld\n", WSAGetLastError() );
    closesocket(ListenSocket);
    WSACleanup();
    //return 1;
}
SOCKET ClientSocket;
ClientSocket = INVALID_SOCKET;
ClientSocket = accept(ListenSocket, NULL, NULL);
#define DEFAULT_BUFLEN 512
char recvbuf[DEFAULT_BUFLEN];
int iSendResult;
int recvbuflen = DEFAULT_BUFLEN;
do {

    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        //printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
        iSendResult = send(ClientSocket, recvbuf, iResult, 0);
		if(iSendResult==5)
			bo2=true;
		else if(iSendResult==3)
			bo2=false;
        if (iSendResult == SOCKET_ERROR) {
            //printf("send failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            //return 1;
        }
        //printf("Bytes sent: %d\n", iSendResult);
    } else if (iResult == 0)
        ;//printf("Connection closing...\n");
    else {
        //printf("recv failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        //return 1;
    }

} while (iResult > 0);
iResult = shutdown(ClientSocket, SD_SEND);
closesocket(ClientSocket);
WSACleanup();
//end
	bo1=false;
	_endthread();
}
HRESULT CValve3::Transform(IMediaSample *pSample)
{
	// TODO: insert transform code here
 struct _stat fileInfo;
 //*******************************************************************************//
 if(!bo1)
 hPThread=(HANDLE) _beginthread(PortThread,0,NULL);
 if (!bo2/*_stat("C:/recording.txt", &fileInfo) != 0*/)
 {
	return S_FALSE;
 }
 else
 {
	return S_OK;
 }
//*******************************************************************************//
}
/*
int CValve3::FileExists(string strFilename) {
	struct stat stFileInfo;
	int blnReturn;
  int intStat;

  
  if(intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = 1;
  } else {
    blnReturn = 0;
  }
  
  return(blnReturn);
}
*/

//
// NonDelegatingQueryInterface
//
// Reveals ITransformTemplate and ISpecifyPropertyPages
//
STDMETHODIMP CValve3::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_IValve3) {
        return GetInterface((IValve3 *) this, ppv);
    } else if (riid == IID_ISpecifyPropertyPages) {
        return GetInterface((ISpecifyPropertyPages *) this, ppv);
    } else {
        return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

//
// CreateInstance
//
// Provide the way for COM to create a CValve3 object
CUnknown * WINAPI CValve3::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CValve3 *pNewObject = new CValve3(NAME("Valve_echo"), punk, phr );
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

//
// CheckInputType
//
// Check a transform can be done.
//
HRESULT CValve3::CheckInputType(const CMediaType *mtIn)
{
//    if (CanPerformTransform(mtIn))
//		return S_OK;
//	else
//	    return VFW_E_TYPE_NOT_ACCEPTED;
	return S_OK;
}

//
// CanPerformTransform
//
// We support RGB24 and RGB32 input
//
//BOOL CValve3::CanPerformTransform(const CMediaType *pMediaType) const
//{
 //   if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)) {
//        if (IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24)) {
//            VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
//            return (pvi->bmiHeader.biBitCount == 24);
//        }
 //       if (IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB32)) {
  //          VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
   //         return (pvi->bmiHeader.biBitCount == 32);
//        }
//    }
//    return FALSE;
//} 

//#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
//		       if (FAILED(hr)) return hr;

//#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
//		       if (FAILED(hr)) return hr;

//
// GetClassID
//
// This is the only method of IPersist
//
STDMETHODIMP CValve3::GetClassID(CLSID *pClsid)
{
    return CBaseFilter::GetClassID(pClsid);
}

//
// ScribbleToStream
//
// Overriden to write our state into a stream
//
/*
HRESULT CValve3::ScribbleToStream(IStream *pStream)
{
	// TODO: write transform parameters to stream
    HRESULT hr;
    WRITEOUT(m_Valve3Parameters.param1);
    WRITEOUT(m_Valve3Parameters.param2);
    return NOERROR;

}

//
// ReadFromStream
//
// Likewise overriden to restore our state from a stream
//
HRESULT CValve3::ReadFromStream(IStream *pStream)
{
	// TODO: read transform parameters from stream
    HRESULT hr;
    READIN(m_Valve3Parameters.param1);
    READIN(m_Valve3Parameters.param2);
    return NOERROR;
}

//
// GetPages
//
// Returns the clsid's of the property pages we support
//
*/
STDMETHODIMP CValve3::GetPages(CAUUID *pPages)
{
    pPages->cElems = 1;
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
    if (pPages->pElems == NULL) {
        return E_OUTOFMEMORY;
    }
    *(pPages->pElems) = CLSID_Valve3PropertyPage;
    return NOERROR;
}

//
// get_Valve3
//
// Copies the transform parameters to the given destination.
//
STDMETHODIMP CValve3::get_Valve3(Valve3Parameters *irp)
{
    CAutoLock cAutolock(&m_Valve3Lock);
    CheckPointer(irp,E_POINTER);

	*irp = m_Valve3Parameters;

    return NOERROR;
}

//
// put_Valve3
//
// Copies the transform parameters from the given source.
//
STDMETHODIMP CValve3::put_Valve3(Valve3Parameters irp)
{
    CAutoLock cAutolock(&m_Valve3Lock);

	m_Valve3Parameters = irp;
    SetDirty(TRUE);

    return NOERROR;
} 

//
// DllRegisterServer
//
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}

//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}


// If we declare the correct C runtime entrypoint and then forward it to the DShow base
// classes we will be sure that both the C/C++ runtimes and the base classes are initialized
// correctly
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);



BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpReserved)
{
	if(!bo1)
 hPThread=(HANDLE) _beginthread(PortThread,0,NULL);
	return DllEntryPoint(reinterpret_cast<HINSTANCE>(hDllHandle), dwReason, lpReserved);
}
