/*
 * Copyright (c) 2010, Tae-young Jung
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// yuvplayerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "SizeDialog.h"
#include "OpenGLView.h"

#include <io.h> 
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_WIDTH 352
#define DEFAULT_HEIGHT 288

#define MARGIN 2
#define MAX_PATH_LEN 1024

enum color_format { YUV420_10BE, YUV420_10LE, YUV444, YUV422, YUV420, UYVY, YUYV, AYUV, YYY, PACKED_YUV444, NV12, NV21, RGB32, RGB24, RGB16, RGBP, P010, P010MSB, P210, P210MSB, Y210, Y210MSB, Y216, Y410 };
enum segment_type { SEGMENT16x16 = 1, SEGMENT32x32 = 2, SEGMENT64x64 = 4};

struct FrameBuffer
{
    unsigned char* y;
    unsigned char* u;
    unsigned char* v;

    unsigned char* rgba;
    unsigned char* misc;
};


// CyuvplayerDlg dialog
class CyuvplayerDlg : public CDialog
{
// Construction
public:
	CyuvplayerDlg(CWnd* pParent = NULL);	// standard constructor

    ~CyuvplayerDlg();

// Dialog Data
	enum { IDD = IDD_YUVPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	wchar_t filename[MAX_PATH_LEN];
    wchar_t filenameSecondary[MAX_PATH_LEN];

	int fd; 
    int fdSecondary;

	int count;
	int cur;

	int frame_size;
	int frame_size_y;
	int frame_size_uv;

	int width, height;
	int t_width, t_height;

    FrameBuffer fb;
    FrameBuffer fbSecondary;

    unsigned char* segment;

	int segment_option;

	float ratio;

	color_format m_color;
	
	CBitmapButton m_open_btn;
	CBitmapButton m_rewind_btn;
	CBitmapButton m_play_btn;
	CBitmapButton m_stop_btn;
	CBitmapButton m_fforward_btn;

	CSliderCtrl m_slider;
	CStatic m_view;

	CSizeDialog* customDlg;

	void Resize(int width, int height);
	void UpdateParameter(void);
	void LoadFrame(void);
    void LoadFrame(int fileDesc, const FrameBuffer& frameBuf);
	void yuv2rgb(void);
	void fforward(void);
	void rewind(void);
	void StartTimer(void);
	void StopTimer(void);
	void rgb2yuv444();
	void rgb2yuv422();
	void rgb2yuv420();
	void OnCmenuSaveYuv( color_format type );

	COpenGLView* OpenGLView;

	BOOL started;
	CMenu* menu;

public:
	afx_msg void OnOpen();
	afx_msg void OnFileReload();
	afx_msg void OnSizeChange(UINT nID );
	afx_msg void OnColor(UINT nID);
	afx_msg void OnZoom(UINT nID);
	afx_msg void OnSegment(UINT nID );

	afx_msg void OnBnClickedRewind();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedFforward();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFileExit();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnFileGo();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCmenuSaveLuminance();
	afx_msg void OnCmenuSaveYuv444();
	afx_msg void OnCmenuSaveYuv422();
	afx_msg void OnCmenuSaveYuv420();
	afx_msg void OnCmenuSaveRgb();

private:
	void UpdateFilename(wchar_t* fileName, const wchar_t* path);
	bool FileOpen( const wchar_t* path, int& fileDesc);
	void DrawSegment(void);
    void CalcDifIfNeeded();
    void calcAbsDiff(unsigned char * buf1, unsigned char * buf2, int size,int offset=128);
    void calcAbsDiff2(short * buf1, short * buf2, int size, int offset = 128);
    void calcAbsDiff410(unsigned char *  buf1, unsigned char *  buf2, int size);
    void ReallocateMem(FrameBuffer & frameBuf);
    ULONGLONG primaryFileSize;
    bool isShowingDifference;

public:
    afx_msg void OnOpenSecondary();
    afx_msg void OnViewShowdifference();
};
