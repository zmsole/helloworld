
// ImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image.h"
#include "ImageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SIZE_WIDTH 240//480
#define SIZE_HEIGHT 180//360

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageDlg �Ի���




CImageDlg::CImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iFirst=0;
	m_iX=0;
	m_iY=0;
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_PictureControl);
	DDX_Control(pDX, IDC_SCROLLBAR_H, m_ScrollBarH);
	DDX_Control(pDX, IDC_SCROLLBAR_V, m_ScrollBarV);
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CImageDlg::OnBnClickedButtonOpen)
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CImageDlg ��Ϣ�������

BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_picDC=m_PictureControl.GetDC();
	m_PictureControl.GetClientRect(&m_client);
	m_ScrollBarV.ShowScrollBar(FALSE);
	m_ScrollBarH.ShowScrollBar(FALSE);
	m_cacheDC.CreateCompatibleDC(NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if(m_iFirst)
		{
			m_picDC->BitBlt(0,0,m_iShowWidth,m_iShowHeight,&m_cacheDC,m_iX,m_iY,SRCCOPY);
			ValidateRect(&m_client);
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_iFirst)
	{
		m_cacheBitmap.DeleteObject();
		m_iFirst=0;
	}
	CRect rc;
	GetClientRect(&rc);
	//CFile file;
	//BITMAPFILEHEADER bf;
	CString strFilter(_T("ͼƬ(*.bmp;*.jpg)|*.BMP;*.JPG|�����ļ�(*.*)|*.*||"));
	CFileDialog OpenDlg(TRUE,NULL,NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter);
	//ͨ���򿪶Ի���ѡ��ͼƬ�ļ�
	int nFlags=OpenDlg.DoModal();
	if(IDOK==nFlags)
	{
		//�û��������Ǵ򿪰�ť�����ļ�·������������strFileName
		m_strFilePath=OpenDlg.GetPathName();
		m_iFirst=1;
	//HBITMAP hBitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),m_strFilePath,IMAGE_BITMAP,rc.Width()/2,
	//	rc.Height()/2,LR_LOADFROMFILE);
	//CStatic* pPic=(CStatic*)GetDlgItem(IDC_IMAGE);
	//ASSERT(pPic);
	//pPic->SetBitmap(hBitmap);
	m_pic.Load(m_strFilePath);
	m_picWidth=m_pic.GetWidth();
	m_picHeight=m_pic.GetHeight();
	m_iX=0;
	m_iY=0;
	if(m_picWidth>=m_client.Width())
	{
		m_iShowWidth=m_client.Width();
		m_ScrollBarH.ShowScrollBar(TRUE);

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);     
		info.fMask = SIF_ALL;     
		info.nMin = 0;     
		info.nMax = m_picWidth-1; 
		info.nPage = m_client.Width();     
		info.nPos = 0;    
		m_ScrollBarH.SetScrollInfo(&info);
	}
	else
	{
		m_iShowWidth=m_picWidth;
	}

	if(m_picHeight>=m_client.Height())
	{
		m_iShowHeight=m_client.Height();
		m_ScrollBarV.ShowScrollBar(TRUE);

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);     
		info.fMask = SIF_ALL;     
		info.nMin = 0;     
		info.nMax = m_picHeight-1; 
		info.nPage = m_client.Height();     
		info.nPos = 0;    
		m_ScrollBarV.SetScrollInfo(&info);
	}
	else
	{
		m_iShowHeight=m_picHeight;
	}

	m_cacheBitmap.CreateCompatibleBitmap(m_picDC,m_picWidth,m_picHeight);
	m_cacheDC.SelectObject(&m_cacheBitmap);
	m_pic.Draw(m_cacheDC,0,0,m_picWidth,m_picHeight,0,0,m_picWidth,m_picHeight);
	m_picDC->BitBlt(0,0,m_iShowWidth,m_iShowHeight,&m_cacheDC,m_iX,m_iY,SRCCOPY);
	UpdateWindow();
	}
}


void CImageDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ReleaseDC(m_picDC);
	CDialogEx::OnClose();
}


void CImageDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int pos;
	pos=m_ScrollBarH.GetScrollPos();

	switch(nSBCode)
	{
	case SB_LINEUP:
		pos-=1;
		break;
	case SB_LINEDOWN:
		pos+=1;
		break;
	case SB_PAGEUP:
		pos-=10;
		break;
	case SB_PAGEDOWN:
		pos+=10;
		break;
	case SB_TOP:
		pos=0;
		break;
	case SB_BOTTOM:
		pos=m_picWidth-1;
		break;
	case SB_THUMBPOSITION:
		pos=nPos;
		break;
	default:
		return;
	}
	if(pos<0)
		pos=0;
	else if(pos+m_client.Width()>m_picWidth-1)
		pos=m_picWidth-1-m_client.Width();
	m_ScrollBarH.SetScrollPos(pos,TRUE);
	m_iX=pos;
	InvalidateRect(&m_client);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CImageDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int pos;
	pos=m_ScrollBarV.GetScrollPos();

	switch(nSBCode)
	{
	case SB_LINEUP:
		pos-=1;
		break;
	case SB_LINEDOWN:
		pos+=1;
		break;
	case SB_PAGEUP:
		pos-=10;
		break;
	case SB_PAGEDOWN:
		pos+=10;
		break;
	case SB_TOP:
		pos=0;
		break;
	case SB_BOTTOM:
		pos=m_picHeight-1;
		break;
	case SB_THUMBPOSITION:
		pos=nPos;
		break;
	default:
		return;
	}
	if(pos<0)
		pos=0;
	else if(pos+m_client.Height()>m_picHeight-1)
		pos=m_picHeight-1-m_client.Height();
	m_ScrollBarV.SetScrollPos(pos,TRUE);
	m_iY=pos;
	InvalidateRect(&m_client);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
