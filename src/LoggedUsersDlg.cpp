// MyTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoggedUsers.h"
#include "LoggedUsersDlg.h"
#include <stdio.h>
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#pragma comment(lib, "netapi32.lib")
#define INFO_BUFFER_SIZE 32767


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMyTestDlg dialog




CMyTestDlg::CMyTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CMyTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMyTestDlg::OnNMDblclkList1)
    ON_BN_CLICKED(IDOK, &CMyTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDREFRESH, &CMyTestDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDOPTIONS, &CMyTestDlg::OnBnClickedOptions)
END_MESSAGE_MAP()


// CMyTestDlg message handlers

static void AddData(CListCtrl &ctrl, int row, int col, const char *str)
{
    LVITEM lv;
    lv.iItem = row;
    lv.iSubItem = col;
    lv.pszText = (LPSTR) str;
    lv.mask = LVIF_TEXT;
    if(col == 0)
        ctrl.InsertItem(&lv);
    else
        ctrl.SetItem(&lv);   
}


BOOL CMyTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	usNumUserEntries = 0;
	memset(_computername_str, 0, sizeof(_computername_str));
	memset(_langroup_str, 0, sizeof(_langroup_str));
	memset(_lanroot_str, 0, sizeof(_lanroot_str));


	LPWKSTA_INFO_102 wData;
	GetWorkStationInfo(wData);
	//NetApiBufferFree(&wData);

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_listCtrl.InsertColumn(0, "Username");
    m_listCtrl.SetColumnWidth(0, 160);

    m_listCtrl.InsertColumn(1, "Domain");
    m_listCtrl.SetColumnWidth(1, 120);

	m_listCtrl.InsertColumn(2, "Server");
	m_listCtrl.SetColumnWidth(2, 120);

    m_listCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);


	GetLoggedOnUsers();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyTestDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;
    int row = m_listCtrl.GetSelectionMark();
    if(row < 0)
        return;
    CString s1 = m_listCtrl.GetItemText(row, 0); // Extract Page
    CString s2 = m_listCtrl.GetItemText(row, 1); // Extract Last modified
    CString s3 = m_listCtrl.GetItemText(row, 2); // Extract Priorty
}

void CMyTestDlg::OnBnClickedOk()
{
    POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
       while (pos)
       {
            int row = m_listCtrl.GetNextSelectedItem(pos);
            CString s1 = m_listCtrl.GetItemText(row, 0); // Extract Page
            CString s2 = m_listCtrl.GetItemText(row, 1); // Extract Last modified
            CString s3 = m_listCtrl.GetItemText(row, 2); // Extract Priorty

            CString msg;
            msg.Format("%d - %s, %s, %s", row + 1, (const char*) s1, (const char*) s2, (const char*) s3);
            MessageBox(msg);
       }
    }
    OnOK();
}


int CMyTestDlg::GetLoggedOnUsers()
{

	LPWKSTA_USER_INFO_0 pBuf = NULL;
	LPWKSTA_USER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	m_listCtrl.DeleteAllItems();
	
	// Call the NetWkstaUserEnum() function, specifying level 0.
	do // begin do
	{
		nStatus = NetWkstaUserEnum(nullptr, dwLevel, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);

		// If the call succeeds,
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// Loop through the entries.
				for (i = 0; (i < dwEntriesRead); i++)
				{
					assert(pTmpBuf != NULL);
					if (pTmpBuf == NULL)
					{
						// Only members of the Administrators local group
						//  can successfully execute NetWkstaUserEnum() locally and on a remote server.
						fprintf_s(stderr, TEXT("LAn access violation has occurred, buffer is NULL!\n"));
						break;
					}

					char username_str[64];
					memset(username_str, 0, sizeof(username_str));
					wcstombs(username_str, pTmpBuf->wkui0_username, lstrlenW(pTmpBuf->wkui0_username));

					AddData(m_listCtrl, i, 0, username_str);
					AddData(m_listCtrl, i, 1, _computername_str);
					AddData(m_listCtrl, i, 2, _langroup_str);
					usNumUserEntries++;
			
					// Print the user logged on to the workstation.
					wprintf(L"\t-- %s\n", pTmpBuf->wkui0_username);
\
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		// Otherwise, indicate a system error.
		else
		{
			printf(TEXT("LNetWkstaUserEnum() failed!\n"));
			fprintf_s(stderr, TEXT("LA system error has occurred : % d\n"), nStatus);
		}

		// Free the allocated memory.
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}

	// Continue to call NetWkstaUserEnum() while
	//  there are more entries.
	while (nStatus == ERROR_MORE_DATA); // end do

	// Check again for allocated memory.
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	// Print the final count of workstation users.
	fprintf_s(stderr, TEXT("\nTotal of % d entries enumerated\n"), dwTotalCount);

	return 0;
}
BOOL CMyTestDlg::GetWorkStationInfo(LPWKSTA_INFO_102& pData)
{
	DWORD dwLevel = 102;

	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;

	nStatus = NetWkstaGetInfo(pszServerName,dwLevel,(LPBYTE*)&pData);

	char sMessage[1024];
	memset(sMessage, 0, sizeof(sMessage));


	memset(_computername_str, 0, sizeof(_computername_str));
	memset(_langroup_str, 0, sizeof(_langroup_str));
	memset(_lanroot_str, 0, sizeof(_lanroot_str));
	wcstombs(_computername_str, pData->wki102_computername, lstrlenW(pData->wki102_computername));
	wcstombs(_langroup_str, pData->wki102_langroup, lstrlenW(pData->wki102_langroup));
	wcstombs(_lanroot_str, pData->wki102_lanroot, lstrlenW(pData->wki102_lanroot));

	if (nStatus == NERR_Success)
	{
		return TRUE;
	}


	return 0;
}

int CMyTestDlg::GetNumberLoggedOnUsers()
{
	DWORD dwLevel = 102;
	LPWKSTA_INFO_102 pBuf = NULL;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;

	nStatus = NetWkstaGetInfo(pszServerName, dwLevel, (LPBYTE*)&pBuf);
	if (nStatus == NERR_Success) {
		return  pBuf->wki102_logged_on_users;
	}
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}


BOOL CMyTestDlg::GetLoggedUserInfo(LPWKSTA_USER_INFO_1 &pUserInfo)
{
	DWORD dwLevel = 1;
	
	NET_API_STATUS nStatus;

	// Call the NetWkstaUserGetInfo() function;
	//  specify level 1.

	nStatus = NetWkstaUserGetInfo(NULL, dwLevel, (LPBYTE*)&pUserInfo);

	// If the call succeeds, print the information about the logged-on user.
	if (nStatus == NERR_Success)
	{
		return TRUE;
	}
	return FALSE;


}

void CMyTestDlg::OnBnClickedRefresh()
{
	GetLoggedOnUsers();

	LPWKSTA_USER_INFO_1 loggedUserInfo;
	GetLoggedUserInfo(loggedUserInfo);
	char username_str[64], domain_str[64], logonserver_str[64];
	memset(username_str, 0, sizeof(username_str));
	memset(domain_str, 0, sizeof(username_str));
	memset(logonserver_str, 0, sizeof(username_str));
	wcstombs(username_str, loggedUserInfo->wkui1_username, lstrlenW(loggedUserInfo->wkui1_username));
	wcstombs(domain_str, loggedUserInfo->wkui1_logon_domain, lstrlenW(loggedUserInfo->wkui1_logon_domain));
	wcstombs(logonserver_str, loggedUserInfo->wkui1_logon_server, lstrlenW(loggedUserInfo->wkui1_logon_server));

	int index = usNumUserEntries - 1;
	AddData(m_listCtrl, index, 0, username_str);
	AddData(m_listCtrl, index, 1, /*domain_str*/ "DOMAIN");
	AddData(m_listCtrl, index, 2, /*logonserver_str*/ "TEST SERVER");


	//NetApiBufferFree(&loggedUserInfo);
}


void CMyTestDlg::OnBnClickedOptions()
{
	int num = GetNumberLoggedOnUsers();
	char sMessage[1024];
	memset(sMessage, 0, sizeof(sMessage));

	sprintf(sMessage, "# Logged On Users : % d\n", num);
	MessageBox(sMessage);

	LPWKSTA_INFO_102 wData;
	GetWorkStationInfo(wData);
	sprintf(sMessage, "Platform: %d\nComputer: %s\nLan group: %s\nLan root: %s\nVersion:  %d.%d\n# Logged On Users: %d\n", wData->wki102_platform_id, _computername_str, _langroup_str, _lanroot_str, wData->wki102_ver_major, wData->wki102_ver_minor, wData->wki102_logged_on_users);
	MessageBox(sMessage);
	//NetApiBufferFree(&wData);
}
