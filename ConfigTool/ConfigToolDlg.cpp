
// ConfigToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"
#include "afxdialogex.h"
#include "Public.h"
#include "ConfigCenter.h"
#include "Protocol.h"
#include "CGridListCtrlEx/CGridColumnTraitEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConfigToolDlg 对话框



CConfigToolDlg::CConfigToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIGTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDEV, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBODATARATE, m_boxDatarate);
	DDX_Control(pDX, IDC_COMBOPA, m_boxPA);
	DDX_Control(pDX, IDC_BUTTONSTART, m_btnStartScan);
	DDX_Control(pDX, IDC_BUTTONSETCFG, m_btnSetcfg);
	DDX_Control(pDX, IDC_BUTTONGETCFG, m_btnGetCfg);
	DDX_Control(pDX, IDC_EDITCHL, m_edCHL);
	DDX_Control(pDX, IDC_EDITSTARTADDR, m_edStartAddr);
	DDX_Control(pDX, IDC_EDITENDADDR, m_edEndAddr);
	//DDX_Control(pDX, IDC_LISTDEV, m_lstDev);
}

BEGIN_MESSAGE_MAP(CConfigToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &CConfigToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONSETCFG, &CConfigToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTONGETCFG, &CConfigToolDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBOPA, &CConfigToolDlg::OnCbnSelchangeCombopa)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTDEV, &CConfigToolDlg::OnLvnItemchangedListdev)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTDEV, &CConfigToolDlg::OnNMDblclkListdev)
END_MESSAGE_MAP()


// CConfigToolDlg 消息处理程序

BOOL CConfigToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_EDITCHL, _T("71"));
	m_boxDatarate.Clear();
	m_boxPA.Clear();
	m_boxDatarate.InsertString(RF24_1MBPS, "1Mbps");
	m_boxDatarate.InsertString(RF24_2MBPS, "2Mbps");
	m_boxDatarate.InsertString(RF24_250KBPS,"250Kbps");
	m_boxDatarate.SetCurSel(RF24_1MBPS);
	m_boxPA.InsertString(RF24_PA_MIN, "MIN");
	m_boxPA.InsertString(RF24_PA_LOW, "LOW");
	m_boxPA.InsertString(RF24_PA_HIGH, "HIGH");
	m_boxPA.InsertString(RF24_PA_MAX, "MAX");
	m_boxPA.SetCurSel(RF24_PA_MAX);
	SetDlgItemText(IDC_EDITSTARTADDR, _T("44544954fe"));
	SetDlgItemText(IDC_EDITENDADDR, _T(""));

	m_ListCtrl.InsertHiddenLabelColumn();
	for (int col = 0; col < m_DataModel.GetColCount(); ++col)
	{
		CGridColumnTrait* pTrait = NULL;
		const CString& title = m_DataModel.GetColTitle(col);
		if (title == "nodeid" || title == "subid" || 
			title == "rfchannel" || title == "rfDataRate" || title == "rfPowerLevel" ||
			title == "networkID")
		{
			pTrait = new CGridColumnTraitEdit;
		}
		m_ListCtrl.InsertColumnTrait(col + 1, title, LVCFMT_LEFT, 90, col, pTrait);
	}
	//m_lstDev.InsertColumn(0, "uniqueid",0,100,50);
	//m_lstDev.InsertColumn(1, "version", 0, 50, 50);
	//m_lstDev.InsertColumn(2, "type", 0, 90, 50);
	//m_lstDev.InsertColumn(3, "nodeid", 0, 50, 50);
	//m_lstDev.InsertColumn(4, "subid", 0, 40, 50);
	//m_lstDev.InsertColumn(5, "rfchannel", 0, 70, 50);
	//m_lstDev.InsertColumn(6, "rfrate", 0, 50, 50);
	//m_lstDev.InsertColumn(7, "rfPA", 0, 40, 50);
	//m_lstDev.InsertColumn(8, "network", 0, 100, 50);
	//m_boxDatarate.SetCurSel(0);
	//m_boxPA.SetCurSel(3);
	SetTimer(1, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConfigToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConfigToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConfigToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConfigToolDlg::OnTimer(UINT nIDEvent)
{
	DisplayDevInfo();
}

void CConfigToolDlg::DisplayDevInfo()
{
	m_ListCtrl.m_redRowCol.clear();
	m_ListCtrl.DeleteAllItems();
	int nItem = 0;
	int rowId = 0;
	CPicaSoftMap<CString, ScanDevBaseInfo> &devmap = ConfigCenter::GetInstance()->m_mapBaseInfoCfg;
	for (CPicaSoftMap<CString, ScanDevBaseInfo>::CIterator it = devmap.GetIterator(); !it.IsEnd(); it.MoveNext())
	{
		ScanDevBaseInfo dev = it.GetValue();
		CString sUniqueid = it.GetKey();
		BaseDeviceInfo_t disInfo = dev.baseInfo;
		ScanDevBaseInfo lastSetbaInfo;
		if (ConfigCenter::GetInstance()->GetLastSetDevBaseInfoByUnique(sUniqueid, lastSetbaInfo))
		{
			if (lastSetbaInfo.tmTime > dev.tmTime)
			{
				disInfo = lastSetbaInfo.baseInfo;
			}
			else
			{ // display newst info,if display scan value is different ,red
				if (lastSetbaInfo.baseInfo.rfChannel != dev.baseInfo.rfChannel)
				{
					m_ListCtrl.m_redRowCol.push_back(CPicaSoftAssistant::FormatString("%d-%d", nItem, m_DataModel.GetColIndexByTitle("rfchannel")));
				}
				if (lastSetbaInfo.baseInfo.rfDataRate != dev.baseInfo.rfDataRate)
				{
					m_ListCtrl.m_redRowCol.push_back(CPicaSoftAssistant::FormatString("%d-%d", nItem, m_DataModel.GetColIndexByTitle("rfdatarate")));
				}
				if (lastSetbaInfo.baseInfo.rfPowerLevel != dev.baseInfo.rfPowerLevel)
				{
					m_ListCtrl.m_redRowCol.push_back(CPicaSoftAssistant::FormatString("%d-%d", nItem, m_DataModel.GetColIndexByTitle("rfpowerlevel")));
				}
				if (lastSetbaInfo.baseInfo.nodeID != dev.baseInfo.nodeID)
				{
					m_ListCtrl.m_redRowCol.push_back(CPicaSoftAssistant::FormatString("%d-%d", nItem, m_DataModel.GetColIndexByTitle("nodeid")));
				}
				if (lastSetbaInfo.baseInfo.subID != dev.baseInfo.subID)
				{
					m_ListCtrl.m_redRowCol.push_back(CPicaSoftAssistant::FormatString("%d-%d", nItem, m_DataModel.GetColIndexByTitle("subid")));
				}
			}
		}
		nItem = m_ListCtrl.InsertItem(++nItem, m_DataModel.GetCellText(rowId, 0));
		m_ListCtrl.SetItemData(nItem, rowId);
		m_ListCtrl.SetItemText(nItem, 1, sUniqueid);
		m_ListCtrl.SetItemText(nItem, 2, CPicaSoftAssistant::FormatString("%d", disInfo.version));
		m_ListCtrl.SetItemText(nItem, 3, CPicaSoftAssistant::FormatString("%d", disInfo.type));
		m_ListCtrl.SetItemText(nItem, 4, CPicaSoftAssistant::FormatString("%d", disInfo.nodeID));
		m_ListCtrl.SetItemText(nItem, 5, CPicaSoftAssistant::FormatString("%d", disInfo.subID));
		m_ListCtrl.SetItemText(nItem, 6, CPicaSoftAssistant::FormatString("%d", disInfo.rfChannel));
		m_ListCtrl.SetItemText(nItem, 7, CPicaSoftAssistant::FormatString("%d", disInfo.rfDataRate));
		m_ListCtrl.SetItemText(nItem, 8, CPicaSoftAssistant::FormatString("%d", disInfo.rfPowerLevel));
		m_ListCtrl.SetItemText(nItem, 9, GetHexStr(disInfo.networkID, 5,TRUE));
		m_ListCtrl.SetItemText(nItem, 10, ToStrbyDevtype(disInfo.type));
		m_ListCtrl.SetItemText(nItem, 11, CPicaSoftAssistant::FormatString("%ld", (CTime::GetCurrentTime() - dev.tmTime).GetTotalSeconds()));
		rowId++;					  
	}
}

void CConfigToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ConfigCenter::GetInstance()->GetScannerStatus() != SYS_RUNNING)
	{
		// start scan
		BYTE channel = 71;
		BYTE bandwidth = 2;
		BYTE powerlevel = 3;
		BYTE network[5] = { 0xFE,0x54,0x49,0x54,0x44 };
		CString sChannel;
		m_edCHL.GetWindowText(sChannel);
		channel = atoi(sChannel);
		bandwidth = m_boxDatarate.GetCurSel();
		powerlevel = m_boxPA.GetCurSel();
		CString sStartNetwork;
		CString sEndNetwork;
		m_edStartAddr.GetWindowText(sStartNetwork);
		if (sStartNetwork.GetLength() != 10)
		{
			AfxMessageBox(_T("StartAddr len error,should 10 byte！"));
		}
		UINT64 bytenet = _strtoui64(sStartNetwork, NULL, 16);
		memcpy(network, (uint8_t *)&bytenet, sizeof(network));
		m_edEndAddr.GetWindowText(sEndNetwork);
		if (sEndNetwork.IsEmpty())
		{
			//long i = strtol(p, &str, 16);
			ConfigCenter::GetInstance()->m_spSerialDP->SendStartScan(channel, bandwidth, powerlevel, network);
			ConfigCenter::GetInstance()->SetScannerStatus(SYS_RUNNING);
			SetDlgItemText(IDC_BUTTONSTART, _T("StopScan"));
			SetTimer(1, 1000, NULL);
		}
	}
	else
	{
		UC network[5] = { 0 };
		ConfigCenter::GetInstance()->m_spSerialDP->SendStopScan(0, 0, 0, network);
		ConfigCenter::GetInstance()->SetScannerStatus(SYS_PAUSE);
		SetDlgItemText(IDC_BUTTONSTART, _T("StartScan"));
		KillTimer(1);
	}
}


void CConfigToolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//UC subid[1] = { 0x47 };
	//m_spSerialDP->SendSetConfigByNodeid(130,0,subid,4,1);
	//rf.network[0] = 0x00;
	//rf.network[1] = 0x54;
	//rf.network[2] = 0x49;
	//rf.network[3] = 0x54;
	//rf.network[4] = 0x44;
	//rf.network[5] = 0x00;
	int rowIndex = m_ListCtrl.GetFocusRow();
	if (rowIndex == -1 )
	{
		AfxMessageBox(_T("Please select a item!"));
	}
	else
	{
		DWORD itemdata = m_ListCtrl.GetItemData(rowIndex);
		CString sUniqueid = m_ListCtrl.GetItemText(rowIndex, 1);
		ScanDevBaseInfo devbaseinfo;
		ConfigCenter::GetInstance()->GetDeviceBaseInfoByUnique(sUniqueid, devbaseinfo);
		UC len = 10; // channel(1) + datarate(1) + powerlevel(1) + network(6) + nodeid(1);
		devbaseinfo.baseInfo.rfChannel = atoi(m_ListCtrl.GetItemText(rowIndex, 6));
		devbaseinfo.baseInfo.rfDataRate = atoi(m_ListCtrl.GetItemText(rowIndex, 7));
		devbaseinfo.baseInfo.rfPowerLevel = atoi(m_ListCtrl.GetItemText(rowIndex, 8));
		UC nodeid = atoi(m_ListCtrl.GetItemText(rowIndex, 4));
		UC subid = atoi(m_ListCtrl.GetItemText(rowIndex, 5));
		CString sNetworkID = m_ListCtrl.GetItemText(rowIndex, 9);
		if (devbaseinfo.baseInfo.nodeID != nodeid)
		{ // nodeid modified
			devbaseinfo.baseInfo.nodeID = nodeid;
		}
		if (devbaseinfo.baseInfo.subID != subid)
		{ // subid modified
			devbaseinfo.baseInfo.subID = subid;
			len++;
		}
		UINT64 bytenet = _strtoui64(sNetworkID, NULL, 16);
		memcpy(devbaseinfo.baseInfo.networkID, (uint8_t *)&bytenet, sizeof(devbaseinfo.baseInfo.networkID));
		//memset(rf.network, 0, 6);
		UC uniqueid[UNIQUE_ID_LEN] = { 0 };
		ConvertHexStr2ByteArr(sUniqueid, uniqueid, UNIQUE_ID_LEN);
		MySetUpRF_t rf;
		ConverDevBase2RFSetStruct(devbaseinfo.baseInfo, rf);
		ConfigCenter::GetInstance()->m_spSerialDP->SendRFByUniqueid(uniqueid, UNIQUE_ID_LEN, rf, len);
		ConfigCenter::GetInstance()->AddLastSetDevBaseInfo(sUniqueid, devbaseinfo.baseInfo);
	}
}


void CConfigToolDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// get config
	// TODO: 在此添加控件通知处理程序代码
	UC uniqueid[8] = { 0x00,0x24,0x00,0x3A,0x0E,0x47,0x31,0x32 };
	//UC uniqueid[8] = { 0x00,0x39,0x00,0x63,0x0B,0x47,0x36,0x31 };
	//UC uniqueid[8] = { 0x7B,0x00,0x3D,0x00,0x08,0x47,0x36,0x34 };
	ConfigCenterPtr spCenter = ConfigCenter::GetInstance();
	spCenter->m_spSerialDP->SendGetConfigByUniqueid(uniqueid, 8, 0, 0);
}


void CConfigToolDlg::OnCbnSelchangeCombopa()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CConfigToolDlg::OnLvnItemchangedListdev(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CConfigToolDlg::OnNMDblclkListdev(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	//KillTimer(1);
}
