#pragma once

class CValve3Properties : public CBasePropertyPage
{

public:

    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

    BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();

    void    GetControlValues();

    CValve3Properties(LPUNKNOWN lpunk, HRESULT *phr);

    BOOL m_bIsInitialized;													// Used to ignore startup messages
    IValve3 *m_pIValve3;				// The custom interface on the filter
	Valve3Parameters Valve3Parameters;	// transform parameters
};

