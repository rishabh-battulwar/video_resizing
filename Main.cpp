//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Used to read in a standard RGB image and display it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

#define MAX_LOADSTRING 100
#define WIDTH 352
#define HEIGHT 288

// Global Variables:
MyImage	inImage, outImage;				// image objects
MyImage *inVideo, *outVideo;
int frames = 0;
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK	Render(HWND, UINT, WPARAM, LPARAM);



// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	
	// Read in the image and its copy
	float scale_w, scale_h; //Scaling factors
	char ImagePath[_MAX_PATH];
	sscanf(lpCmdLine, "%s %f %f", &ImagePath, &scale_w, &scale_h);

	FILE *p_file = NULL;
	p_file = fopen(ImagePath, "rb");
	fseek(p_file, 0, SEEK_END);
	frames = ftell(p_file) / (352 * 288 * 3);
	fclose(p_file);

	inVideo = new MyImage[frames];
	outVideo = new MyImage[frames];

	//inImage[0].setWidth(WIDTH);
	//inImage[0].setHeight(HEIGHT);
	
	for (int i = 0; i < frames; i++)
	{
		inVideo[i].setWidth(WIDTH);
		inVideo[i].setHeight(HEIGHT);

		outVideo[i].setWidth(WIDTH*scale_w);
		outVideo[i].setHeight(HEIGHT*scale_h);
	}

	if ( strstr(ImagePath, ".rgb" )==NULL )
	{ 
		AfxMessageBox( "Image has to be a '.rgb' file\nUsage - Image.exe image.rgb scale_w scale_h");
		//return FALSE;
	}
	else
	{
		//inImage[0].setImagePath(ImagePath);
		//if ( !inImage[0].ReadImage() )
		//{ 
		//	AfxMessageBox( "Could not read image\nUsage - Image.exe image.rgb scale_w scale_h");
		//	//return FALSE;
		//}

		for (int i = 0; i < frames; i++)
		{
			inVideo[i].setImagePath(ImagePath);
			if (!inVideo[i].ReadImage(i))
			{
				AfxMessageBox("Could not read image %d \nUsage - Image.exe image.rgb scale_w scale_h", i);
				//return FALSE;
			}
		}
		
		//else
			//outImage = inImage;
	}
	

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	

	return msg.wParam;

}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
	   NULL, 
	   NULL, 
	   hInstance, 
	   NULL);

   if (!hWnd)
   {
	   MessageBox(NULL,
		   _T("Call to CreateWindow Failed!"),
		   _T("Win32 Guided Tour"),
		   NULL);

      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hButton;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				/*case PLAY_INVIDEO_BUTTON_ID:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)Render);
					break;*/
				case ID_MODIFY_IMAGE:
					//for (int i = 0; i < frames; i++)
						outVideo[400].Modify(inVideo[400]);
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_CREATE:
			hButton = CreateWindow(TEXT("button"), TEXT("Play"),
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				140, 40,
				50, 20,
				hWnd, (HMENU)PLAY_INVIDEO_BUTTON_ID,
				hInst, NULL);
			break;

		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TO DO: Add any drawing code here...
				char text[1000];
				strcpy(text, "Original image (Left)  Image after modification (Right)\n");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
				strcpy(text, "\nUpdate program with your code to modify input image");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);

				//std::chrono::milliseconds interval(40);
				//for (int i = 0; i < frames; i++)
				//{
				//	BITMAPINFO bmi_in;
				//	CBitmap bitmap;
				//	memset(&bmi_in, 0, sizeof(bmi_in));
				//	bmi_in.bmiHeader.biSize = sizeof(bmi_in.bmiHeader);
				//	bmi_in.bmiHeader.biWidth = inVideo[i].getWidth();
				//	bmi_in.bmiHeader.biHeight = -inVideo[i].getHeight();  // Use negative height.  DIB is top-down.
				//	bmi_in.bmiHeader.biPlanes = 1;
				//	bmi_in.bmiHeader.biBitCount = 24;
				//	bmi_in.bmiHeader.biCompression = BI_RGB;
				//	bmi_in.bmiHeader.biSizeImage = inVideo[i].getWidth()*inVideo[i].getHeight();

				//	SetDIBitsToDevice(hdc,
				//		0, 100, inVideo[i].getWidth(), inVideo[i].getHeight(),
				//		0, 0, 0, inVideo[i].getHeight(),
				//		inVideo[i].getImageData(), &bmi_in, DIB_RGB_COLORS);

				//	std::this_thread::sleep_for(interval);
				//}
				
				BITMAPINFO bmi_in;
				CBitmap bitmap;
				memset(&bmi_in, 0, sizeof(bmi_in));
				bmi_in.bmiHeader.biSize = sizeof(bmi_in.bmiHeader);
				bmi_in.bmiHeader.biWidth = inVideo[400].getWidth();
				bmi_in.bmiHeader.biHeight = -inVideo[400].getHeight();  // Use negative height.  DIB is top-down.
				bmi_in.bmiHeader.biPlanes = 1;
				bmi_in.bmiHeader.biBitCount = 24;
				bmi_in.bmiHeader.biCompression = BI_RGB;
				bmi_in.bmiHeader.biSizeImage = inVideo[400].getWidth()*inVideo[400].getHeight();

				SetDIBitsToDevice(hdc,
					0, 100, inVideo[400].getWidth(), inVideo[400].getHeight(),
					0, 0, 0, inVideo[400].getHeight(),
					inVideo[400].getImageData(), &bmi_in, DIB_RGB_COLORS);


				BITMAPINFO bmi_out;
				//CBitmap bitmap;
				memset(&bmi_out, 0, sizeof(bmi_out));
				bmi_out.bmiHeader.biSize = sizeof(bmi_out.bmiHeader);
				bmi_out.bmiHeader.biWidth = outVideo[400].getWidth();
				bmi_out.bmiHeader.biHeight = -outVideo[400].getHeight();  // Use negative height.  DIB is top-down.
				bmi_out.bmiHeader.biPlanes = 1;
				bmi_out.bmiHeader.biBitCount = 24;
				bmi_out.bmiHeader.biCompression = BI_RGB;
				bmi_out.bmiHeader.biSizeImage = outVideo[400].getWidth()*outVideo[400].getHeight();

				SetDIBitsToDevice(hdc,
					inVideo[0].getWidth() + 50, 100, outVideo[400].getWidth(), outVideo[400].getHeight(),
					0, 0, 0, outVideo[400].getHeight(),
					outVideo[400].getImageData(), &bmi_out, DIB_RGB_COLORS);

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


// Mesage handler for about box.
//LRESULT CALLBACK Render(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//	HDC hdc2;
//	RECT rt;
//	GetClientRect(hWin, &rt);
//	switch (message)
//	{
//		case WM_INITDIALOG:
//			return TRUE;
//
//		case WM_COMMAND:
//			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//			{
//				EndDialog(hWin, LOWORD(wParam));
//				return TRUE;
//			}
//			break;
//
//		case WM_PAINT:
//			{
//				hdc2 = BeginPaint(hWin, &ps);
//				// TO DO: Add any drawing code here...
//				char text[1000];
//				strcpy(text, "Original image (Left)  Image after modification (Right)\n");
//				DrawText(hdc2, text, strlen(text), &rt, DT_LEFT);
//				strcpy(text, "\nUpdate program with your code to modify input image");
//				DrawText(hdc2, text, strlen(text), &rt, DT_LEFT);
//
//				std::chrono::milliseconds interval(40);
//				for (int i = 0; i < frames; i++)
//				{
//					BITMAPINFO bmi_in;
//					CBitmap bitmap;
//					memset(&bmi_in, 0, sizeof(bmi_in));
//					bmi_in.bmiHeader.biSize = sizeof(bmi_in.bmiHeader);
//					bmi_in.bmiHeader.biWidth = inVideo[i].getWidth();
//					bmi_in.bmiHeader.biHeight = -inVideo[i].getHeight();  // Use negative height.  DIB is top-down.
//					bmi_in.bmiHeader.biPlanes = 1;
//					bmi_in.bmiHeader.biBitCount = 24;
//					bmi_in.bmiHeader.biCompression = BI_RGB;
//					bmi_in.bmiHeader.biSizeImage = inVideo[i].getWidth()*inVideo[i].getHeight();
//
//					SetDIBitsToDevice(hdc2,
//						0, 100, inVideo[i].getWidth(), inVideo[i].getHeight(),
//						0, 0, 0, inVideo[i].getHeight(),
//						inVideo[i].getImageData(), &bmi_in, DIB_RGB_COLORS);
//
//					std::this_thread::sleep_for(interval);
//				}
//				
//				BITMAPINFO bmi_out;
//				CBitmap bitmap;
//				memset(&bmi_out, 0, sizeof(bmi_out));
//				bmi_out.bmiHeader.biSize = sizeof(bmi_out.bmiHeader);
//				bmi_out.bmiHeader.biWidth = outVideo[0].getWidth();
//				bmi_out.bmiHeader.biHeight = -outVideo[0].getHeight();  // Use negative height.  DIB is top-down.
//				bmi_out.bmiHeader.biPlanes = 1;
//				bmi_out.bmiHeader.biBitCount = 24;
//				bmi_out.bmiHeader.biCompression = BI_RGB;
//				bmi_out.bmiHeader.biSizeImage = outVideo[0].getWidth()*outVideo[0].getHeight();
//
//				SetDIBitsToDevice(hdc2,
//					inVideo[0].getWidth() + 50, 100, outVideo[0].getWidth(), outVideo[0].getHeight(),
//					0, 0, 0, outVideo[0].getHeight(),
//					outVideo[0].getImageData(), &bmi_out, DIB_RGB_COLORS);
//
//				EndPaint(hWin, &ps);
//			}
//			break;
//	}
//    return FALSE;
//}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

