// MyTestDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include <lmwksta.h>
#include <lm.h>
// CMyTestDlg dialog
class CMyTestDlg : public CDialog
{
// Construction
public:
	CMyTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int GetLoggedOnUsers();
	int GetNumberLoggedOnUsers();
	BOOL GetLoggedUserInfo( LPWKSTA_USER_INFO_1& pUserInfo);
	BOOL GetWorkStationInfo(LPWKSTA_INFO_102& pData);
	unsigned short usNumUserEntries;
public:
    CListCtrl m_listCtrl;
    afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOptions();
};
