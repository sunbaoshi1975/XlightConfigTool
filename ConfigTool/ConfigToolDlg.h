
// ConfigToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
#include "CListCtrl_DataModel.h"
#include "MyGridListCtrl.h"

// CConfigToolDlg 对话框
class CConfigToolDlg : public CDialogEx
{
// 构造
public:
	CConfigToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGTOOL_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
