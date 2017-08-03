
// ConfigToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
#include "CListCtrl_DataModel.h"
#include "MyGridListCtrl.h"

// CConfigToolDlg �Ի���
class CConfigToolDlg : public CDialogEx
{
// ����
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGTOOL_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:

public:
	afx_msg void OnTimer(UINT nIDEvent);
	void DisplayDevInfo();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	CEdit m_edCHL;
	CComboBox m_boxDatarate;
	CComboBox m_boxPA;
	CButton m_btnStartScan;
	CButton m_btnSetcfg;
	CButton m_btnGetCfg;
	afx_msg void OnCbnSelchangeCombopa();
	CEdit m_edStartAddr;
	CEdit m_edEndAddr;
	afx_msg void OnLvnItemchangedListdev(NMHDR *pNMHDR, LRESULT *pResult);
private:
	MyGridListCtrl m_ListCtrl;
	CListCtrl_DataModel m_DataModel;
public:
	afx_msg void OnNMDblclkListdev(NMHDR *pNMHDR, LRESULT *pResult);

};
