#pragma once

//#include <cstring>



class CValve3
	: public CTransInPlaceFilter,
	  public IValve3,
	  public ISpecifyPropertyPages,
	  public CPersistStream
{
public:

    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	static int object_counter;
	int my_internal_number;

    DECLARE_IUNKNOWN;

    // Constructor
    CValve3(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

    // Destructor
    ~CValve3();

    // Overrriden from CTransformFilter base class
    HRESULT Transform(IMediaSample *pSample);
    HRESULT CheckInputType(const CMediaType* mtIn);

    // CPersistStream stuff
  //  HRESULT ScribbleToStream(IStream *pStream);
   // HRESULT ReadFromStream(IStream *pStream);
    STDMETHODIMP GetClassID(CLSID *pClsid);

    // Reveals ITransformTemplate and ISpecifyPropertyPages
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    // These implement the custom ITransformTemplate interface
    STDMETHODIMP get_Valve3(Valve3Parameters *irp);
    STDMETHODIMP put_Valve3(Valve3Parameters irp);

    // ISpecifyPropertyPages interface
    STDMETHODIMP GetPages(CAUUID *pPages);

private:
	//BOOL CanPerformTransform(const CMediaType *pMediaType) const;

	Valve3Parameters m_Valve3Parameters;
	CCritSec m_Valve3Lock;
};

int CValve3::object_counter = 0;//unikalne numery obiektow
