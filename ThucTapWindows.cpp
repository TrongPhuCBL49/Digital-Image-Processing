// ThucTapWindows.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ThucTapWindows.h"
#include <commdlg.h>
#include "XuLyAnh.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THUCTAPWINDOWS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THUCTAPWINDOWS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THUCTAPWINDOWS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THUCTAPWINDOWS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static OPENFILENAMEA ofn;       // common dialog box structure
	static char szFile[260];       // buffer for file name
	static Mat imgin;
	static Mat imgout;

    switch (message)
    {
	case WM_CREATE:
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Image (*.bmp; *.jpg; *.jpeg; *.png; *.gif; *.tif; *.tiff)\0*.bmp; *.jpg; *.jpeg; *.png; *.gif; *.tif; *.tiff\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_OPENIMAGE:
				// Display the Open dialog box. 
				if (GetOpenFileNameA(&ofn) == TRUE)
				{
					imgin = imread(ofn.lpstrFile, IMREAD_GRAYSCALE);
					namedWindow("Image In", WINDOW_AUTOSIZE);
					imshow("Image In", imgin);
				}
				break;
			case ID_FILE_SAVEIMAGE:
				if (GetSaveFileNameA(&ofn) == TRUE)
					imwrite(ofn.lpstrFile, imgout);
				break;

			case ID_CHAPTER3_NEGATIVE:
				imgout = Mat(imgin.size(), CV_8UC1);
				Negative(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER3_HISTOGRAMEQUALIZATION:
				imgout = Mat(imgin.size(), CV_8UC1);
				equalizeHist(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER3_LOCALHISTOGRAM:
				imgout = Mat(imgin.size(), CV_8UC1);
				LocalHistogram(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER3_HISTOGRAMSTATISTICS:
				imgout = Mat(imgin.size(), CV_8UC1);
				HistogramStatistics(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER3_LOWPASSFILTER:
				imgout = Mat(imgin.size(), CV_8UC1);
				LowpassFilter(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER3_MEDIANFILTER:
				imgout = Mat(imgin.size(), CV_8UC1);
				medianBlur(imgin, imgout, 5);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;

			case ID_CHAPTER4_FOURIER:
			{
				int M = imgin.size().height;
				int N = imgin.size().width;
				int P = getOptimalDFTSize(M);
				int Q = getOptimalDFTSize(N);
				imgout = Mat(P, Q, CV_8UC1);
				Fourier(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
			}
				break;

			case ID_CHAPTER4_FILTERFREQUENCY:
				imgout = Mat(imgin.size(), CV_8UC1);
				FilterFrequency(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;

			case ID_CHAPTER4_MOIREREMOVE:
				imgout = Mat(imgin.size(), CV_8UC1);
				MoireRemove(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER4_BOLOCCHUNHAT:
			{
				int M = 512;
				int N = 800;
				imgin = Mat(M, N, CV_8UC1, CV_RGB(255, 255, 255));
				TaoBoLocChuNhat(imgin);

				namedWindow("Image In", WINDOW_AUTOSIZE);
				imshow("Image In", imgin);
			}

			break;
			case ID_CHAPTER4_PERIODREMOVE:
				imgout = Mat(imgin.size(), CV_8UC1);
				PeriodRemove(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER9_GRAYSCALEEROSION:
				imgout = Mat(imgin.size(), CV_8UC1);
				GrayScaleErosion(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER9_GRAYSCALEDILATION:
				imgout = Mat(imgin.size(), CV_8UC1);
				GrayScaleDilation(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER9_GRAYSCALEOPENING:
				imgout = Mat(imgin.size(), CV_8UC1);
				GrayScaleOpening(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case ID_CHAPTER9_GRAYSCALECLOSING:
				imgout = Mat(imgin.size(), CV_8UC1);
				GrayScaleClosing(imgin, imgout);
				namedWindow("Image Out", WINDOW_AUTOSIZE);
				imshow("Image Out", imgout);
				break;
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
