#include <windows.h>
#include <windowsx.h>
#include <streams.h>
#include <commctrl.h>
#include <olectl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "iValve3.h"
#include "Valve3.h"
#include "Valve3Prop.h"

//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *CValve3Properties::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    CUnknown *punk = new CValve3Properties(lpunk, phr);
    if (punk == NULL) {
	*phr = E_OUTOFMEMORY;
    }
    return punk;

}

//
// Constructor
//
CValve3Properties::CValve3Properties(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("Image Resize Property Page"),
                      pUnk,IDD_PROPERTIES,IDS_TITLE),
    m_pIValve3(NULL),
    m_bIsInitialized(FALSE)
{
    ASSERT(phr);
}

//
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL CValve3Properties::OnReceiveMessage(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if (m_bIsInitialized)
            {
                m_bDirty = TRUE;
                if (m_pPageSite)
                {
                    m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
                }
            }
            return (LRESULT) 1;
        }

    }
    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);

}

//
// OnConnect
//
// Called when we connect to a transform filter
//
HRESULT CValve3Properties::OnConnect(IUnknown *pUnknown)
{
    ASSERT(m_pIValve3 == NULL);

    HRESULT hr = pUnknown->QueryInterface(IID_IValve3, (void **) &m_pIValve3);
    if (FAILED(hr)) {
        return E_NOINTERFACE;
    }

    ASSERT(m_pIValve3);

    m_pIValve3->get_Valve3(&Valve3Parameters);
    m_bIsInitialized = FALSE ;
    return NOERROR;
}

//
// OnDisconnect
//
// Likewise called when we disconnect from a filter
//
HRESULT CValve3Properties::OnDisconnect()
{
    if (m_pIValve3 == NULL) {
        return E_UNEXPECTED;
    }

    m_pIValve3->Release();
    m_pIValve3 = NULL;
    return NOERROR;
}

//
// OnActivate
//
// We are being activated
//
HRESULT CValve3Properties::OnActivate()
{
    TCHAR   sz[60];

    _stprintf(sz, TEXT("%d"), Valve3Parameters.param1);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz);
   
	m_bIsInitialized = TRUE;

	return NOERROR;
}

//
// OnDeactivate
//
// We are being deactivated
//
HRESULT CValve3Properties::OnDeactivate(void)
{
    ASSERT(m_pIValve3);
    m_bIsInitialized = FALSE;
    GetControlValues();
    return NOERROR;
}

//
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT CValve3Properties::OnApplyChanges()
{
    GetControlValues();
    m_pIValve3->put_Valve3(Valve3Parameters);

    return NOERROR;
}

//
// GetControlValues
//
void CValve3Properties::GetControlValues()
{
    TCHAR sz[STR_MAX_LENGTH];

    Edit_GetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz, STR_MAX_LENGTH);
	Valve3Parameters.param1 = _ttoi(sz);
	



}
