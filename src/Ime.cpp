// ImeClass.cpp: implementation of the ImeClass class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Ime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImeClass::ImeClass()
{
	m_hWnd			=	NULL;
	m_hIMC			=	NULL;

	bCandList		=	FALSE;
	nCandState		=	0;
	nStartListIdx	=	1;
	nCandListCount	=	0;
	nTX1			=	0;
	CUR_ON			=	0;
	nPrevCompLen	=	0;
	nCurrCompLen	=	0;

	bMove		=	FALSE;
	P_MOUSE.x	=	400;
	P_MOUSE.y	=	200;
}

ImeClass::~ImeClass()
{
}

void ImeClass::GetInfoPerTime()
{
	bCandList		=	FALSE;
	nCandState		=	0;
	nStartListIdx	=	1;
	nCandListCount	=	0;

	m_hIMC	=	ImmGetContext(m_hWnd);   // IME �ڵ�
/*
	HWND hWnd = GetFocus();

	if (m_hWnd == hWnd)
	{
		GetCompositionString();
		GetCandidateList();
		GetImeInfo();
		GetCompositionStringInfo();
	}
	else
	{
		GetImeInfo();

	}
*/

		GetCompositionString();
		GetCandidateList();
		GetImeInfo();
		GetCompositionStringInfo();

	ImmReleaseContext(m_hWnd, m_hIMC);
}

void ImeClass::GetCompositionString()
{
	CHAR	szCompStr[512];

    DWORD dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPSTR,szCompStr,sizeof(szCompStr));
	
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');

	this->m_szCOMPOSTR	=	szCompStr;
}

void ImeClass::GetCompositionStringInfo()
{
	CStrClass	sz;
	CHAR	szCompStr[512];

	memset(szCompStr,0x00,512);
	DWORD dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPATTR,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_COMPATTR[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPCLAUSE,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_COMPCLAUSE[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPREADATTR,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_COMPREADATTR[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPREADCLAUSE,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_COMPREADCLAUSE[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_COMPREADSTR,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_COMPREADSTR[%s]",szCompStr);
	m_szCompReadStr	=	szCompStr;

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_RESULTCLAUSE,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_RESULTCLAUSE[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_RESULTREADCLAUSE,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_RESULTREADCLAUSE[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_RESULTREADSTR,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_RESULTREADSTR[%s]",szCompStr);
	m_szResultReadStr	=	szCompStr;

//	this->m_ctrlCandInfo.AddString(sz);

	memset(szCompStr,0x00,512);
	dwCompStrLen = ImmGetCompositionString(m_hIMC,GCS_RESULTSTR,szCompStr,sizeof(szCompStr));
	dwCompStrLen /= sizeof(CHAR);
	szCompStr[dwCompStrLen] = TEXT('\0');
	sz.Format("GCS_RESULTSTR[%s]",szCompStr);

//	this->m_ctrlCandInfo.AddString(sz);

}

void ImeClass::GetCandidateList()
{	
	DWORD dwSize;
	LPCANDIDATELIST lpCandList;

	this->nCandListCount	=	0;
	this->bCandList			=	FALSE;
	if (dwSize = ImmGetCandidateList(m_hIMC,0x0,NULL,0))	// Get Size of CandidateList
	{
		lpCandList = (LPCANDIDATELIST)GlobalAlloc(GPTR,dwSize);
   
		ImmGetCandidateList(m_hIMC,0x0,lpCandList,dwSize);

		UINT i;
		LPDWORD lpdwOffset;

		if (!lpCandList->dwCount)
			;
		else
		{
			SetCandidateInfo(lpCandList);

			lpdwOffset = &lpCandList->dwOffset[0];

			lpdwOffset += lpCandList->dwPageStart;

			int nCount = 0;
			for (i = lpCandList->dwPageStart;
				(i < lpCandList->dwCount) && (i < lpCandList->dwPageStart + lpCandList->dwPageSize) ;
				i++)
			{
					LPSTR lpstr = (LPSTR)lpCandList + *lpdwOffset++;

					CStrClass sz;
					sz.Format("[%d] ",((nCount+nStartListIdx) == 10 ? 0 : nCount+nStartListIdx));
					sz += lpstr;
					this->m_szCandList[nCount] = sz; 
					nCount++;
			}
			this->nCandListCount	=	nCount;
			this->bCandList			=	TRUE;
		}

		DWORD dwCount;
		DWORD dwRet = ImmGetCandidateListCount(m_hIMC, &dwCount);

		CStrClass sz;
		sz.Format("Count[%u] returnSize[%u]",dwCount,dwRet);
//		this->m_ctrlCandInfo.AddString(sz);

	}
}

void ImeClass::SetCandidateInfo(LPCANDIDATELIST lpCandList)
{
	return;
	CStrClass sz;
	
	sz.Format("dwCount[%u]",lpCandList->dwCount);
//	this->m_ctrlCandInfo.AddString(sz);

	sz.Format("dwPageSize[%u]",lpCandList->dwPageSize);
//	this->m_ctrlCandInfo.AddString(sz);

	sz.Format("dwPageStart[%u]",lpCandList->dwPageStart);
//	this->m_ctrlCandInfo.AddString(sz);

	sz.Format("dwSelection[%u]",lpCandList->dwSelection);
//	this->m_ctrlCandInfo.AddString(sz);

	sz.Format("dwSize[%u]",lpCandList->dwSize);
//	this->m_ctrlCandInfo.AddString(sz);

	sz.Format("dwStyle[%u]",lpCandList->dwStyle);
//	this->m_ctrlCandInfo.AddString(sz);
}

void ImeClass::GetImeInfo()
{
	char szKLID[128];
	char szDesc[128];
	memset(szKLID,0x00,128);
	memset(szDesc,0x00,128);
	if (TRUE == GetKeyboardLayoutName(szKLID))
	{
		HKL hkl = LoadKeyboardLayout(szKLID,KLF_ACTIVATE);
		UINT uiRet = ImmGetDescription(hkl,szDesc,256);
		this->m_szTitle	=szDesc;


		DWORD dwRet = ImmGetProperty(hkl,IGP_PROPERTY);

		if (dwRet & IME_PROP_CANDLIST_START_FROM_1)
			nStartListIdx	=	1;
		else
			nStartListIdx	=	0;

//		if (dwRet & IME_PROP_AT_CARET)					this->m_ctrlCandInfo.AddString("IME_PROP_AT_CARET");
//		if (dwRet & IME_PROP_SPECIAL_UI)				this->m_ctrlCandInfo.AddString("IME_PROP_SPECIAL_UI");
//		if (dwRet & IME_PROP_CANDLIST_START_FROM_1)	this->m_ctrlCandInfo.AddString("IME_PROP_CANDLIST_START_FROM_1");
//		if (dwRet & IME_PROP_UNICODE)					this->m_ctrlCandInfo.AddString("IME_PROP_UNICODE");
//		if (dwRet & IME_PROP_COMPLETE_ON_UNSELECT)		this->m_ctrlCandInfo.AddString("IME_PROP_COMPLETE_ON_UNSELECT");

//		dwRet = ImmGetProperty(hkl,IGP_GETIMEVERSION);

//		if (dwRet & IMEVER_0310)	this->m_ctrlCandInfo.AddString("IMEVER_0310");
//		if (dwRet & IMEVER_0400)	this->m_ctrlCandInfo.AddString("IMEVER_0400");
	}

	//�ѱ� ���� ���...
	DWORD dwConv, dwSent;

	ImmGetConversionStatus(m_hIMC,&dwConv,&dwSent);

	switch (dwConv & IME_CMODE_LANGUAGE)
	{
	case IME_CMODE_ALPHANUMERIC:
		this->m_szConvMode = "A";//"ALPHANUMERIC";
		break;
	case IME_CMODE_NATIVE:
		this->m_szConvMode = "��";//"NATIVE";
		break;
	case (IME_CMODE_NATIVE | IME_CMODE_KATAKANA):
	default:
		break;
	}
	
	if (dwConv & IME_CMODE_FULLSHAPE)
		this->m_szHalfMode = "��";//"FULLSHAPE";
	else
		this->m_szHalfMode = "��";//"HALFSHAPE";

}

void ImeClass::OnButtonChangemode() 
{
	// TODO: Add your control notification handler code here
	m_hIMC	=	ImmGetContext(m_hWnd);   // IME �ڵ�

    DWORD dwConv, dwSent;
    DWORD dwTemp;

    ImmGetConversionStatus(m_hIMC,&dwConv,&dwSent);
/*
    switch(LOWORD(wParam))
    {
        case IDM_NATIVEMODE:
*/
            dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
            switch (dwConv & IME_CMODE_LANGUAGE)
            {
                case IME_CMODE_ALPHANUMERIC:
                    dwTemp |= IME_CMODE_NATIVE;
                    break;

                case IME_CMODE_NATIVE:
//                    dwTemp |= (IME_CMODE_NATIVE | IME_CMODE_KATAKANA);
                    break;

                case (IME_CMODE_NATIVE | IME_CMODE_KATAKANA):
                default:
                    break;
            }
            dwConv = dwTemp;
/*            break;

        case IDM_FULLHALF:
            if (dwConv & IME_CMODE_FULLSHAPE)
                dwConv &= ~IME_CMODE_FULLSHAPE;
            else
                dwConv |= IME_CMODE_FULLSHAPE;
            break;

        case IDM_ROMAN:
            if (dwConv & IME_CMODE_ROMAN)
                dwConv &= ~IME_CMODE_ROMAN;
            else
                dwConv |= IME_CMODE_ROMAN;
            break;

        case IDM_CHARCODE:
            if (dwConv & IME_CMODE_CHARCODE)
                dwConv &= ~IME_CMODE_CHARCODE;
            else
                dwConv |= IME_CMODE_CHARCODE;
            break;

        case IDM_HANJA:
            if (dwConv & IME_CMODE_HANJACONVERT)
                dwConv &= ~IME_CMODE_HANJACONVERT;
            else
                dwConv |= IME_CMODE_HANJACONVERT;
            break;

        case IDM_SOFTKBD:
            if (dwConv & IME_CMODE_SOFTKBD)
                dwConv &= ~IME_CMODE_SOFTKBD;
            else
                dwConv |= IME_CMODE_SOFTKBD;
            break;

        case IDM_EUDC:
            if (dwConv & IME_CMODE_EUDC)
                dwConv &= ~IME_CMODE_EUDC;
            else
                dwConv |= IME_CMODE_EUDC;
            break;

        case IDM_SYMBOL:
            if (dwConv & IME_CMODE_SYMBOL)
                dwConv &= ~IME_CMODE_SYMBOL;
            else
                dwConv |= IME_CMODE_SYMBOL;
            break;
    }
*/
    ImmSetConversionStatus(m_hIMC,dwConv,dwSent);

	ImmReleaseContext(m_hWnd, m_hIMC);
}
