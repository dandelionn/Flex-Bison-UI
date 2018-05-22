#include "main.h"
#include <string>
const char szClassName[] = "myWindowClass";
const char szChildClassName[] = "myMDIChildWindowClass";

#define IDC_MAIN_MDI	301
#define IDC_MAIN_TOOL	302
#define IDC_MAIN_STATUS	303

#include <iostream>


#define ID_MDI_FIRSTCHILD 50000

#define IDC_CHILD_EDIT	105
#define IDC_LOG    106


//let's say an user won't create more than 500 edit files in a program instance
float font_coef = 1.1;

int ll=0;
int yy=0;
int log=0;

HWND hMDIClient = NULL;
HWND hMDIClientLog = NULL;
HWND hMainWindow = NULL;

char title[256];
char buffer[400000];   ///be carefull with this buffer in case your program crashes
char buffer1[400000];
char file[256]="output\\";

void CustomizedToolbar(HWND hwnd, HINSTANCE hInst)
{
    HWND hToolbar = NULL;
    TBBUTTON buttons[27];

    HIMAGELIST himToolbar = ImageList_LoadImage(hInst,
                                               MAKEINTRESOURCE(ID_BMP_TOOLBAR),
                                               16,
                                               1,
                                               RGB(255, 0 , 255),
                                               IMAGE_BITMAP,
                                               LR_CREATEDIBSECTION);

    if( !himToolbar )
    {
        MessageBox(hwnd, "Could not be created!", "Eror", MB_ICONEXCLAMATION | MB_OK);
    }


    hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT, 0, 0, 0, 0,
				hwnd, (HMENU)IDC_MAIN_TOOL, GetModuleHandle(NULL), NULL);


     SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM) himToolbar);

	 ZeroMemory(buttons, sizeof(buttons));

     for(int n=0; n < 27; n++)
     {
         if(n<6)
               buttons[n].iBitmap = n;
               buttons[n].fsState = TBSTATE_ENABLED;
               buttons[n].fsStyle = TBSTYLE_BUTTON;
         if(n==6 || n==7 || n==16 || n==17 || n==25 || n==26)
           {
               buttons[n].iBitmap = 6;
               buttons[n].fsState = TBSTATE_ENABLED;
               buttons[n].fsStyle = TBSTYLE_SEP;
           }
         if((n>7 && n<13) || (n>17 && n<23))
            {
               buttons[n].iBitmap = 6;
               buttons[n].fsState = TBSTATE_INDETERMINATE;
               buttons[n].fsStyle = TBSTYLE_BUTTON;
           }
         if(n>=13 && n<16)
           {
               buttons[n].iBitmap = n-6;
               buttons[n].fsState = TBSTATE_ENABLED;
               buttons[n].fsStyle = TBSTYLE_BUTTON;
           }
         if(n>22 && n<25)
           {
               buttons[n].iBitmap = n-13;
               buttons[n].fsState = TBSTATE_ENABLED;
               buttons[n].fsStyle = TBSTYLE_BUTTON;
           }
         buttons[n].dwData = 0;
         buttons[n].iString = 0;
     }

     ShowWindow(hToolbar, SW_NORMAL);

    buttons[0].idCommand = ID_FILE_NEW ;
    buttons[1].idCommand = ID_FILE_OPEN;
    buttons[2].idCommand = ID_FILE_SAVE;
    buttons[3].idCommand = ID_FILE_SAVEALL;
    buttons[4].idCommand = ID_FILE_UNDO;
    buttons[5].idCommand = ID_FILE_REDO;

    buttons[13].idCommand = ID_FILE_RUN;
    buttons[14].idCommand = ID_FILE_SHOWWLY;
    buttons[15].idCommand = ID_FILE_SHOWLOG;

    buttons[23].idCommand = ID_FILE_RUNL;
    buttons[24].idCommand = ID_FILE_RUNY;

 // Send the TB_BUTTONSTRUCTSIZE message, which is required for
        // backward compatibility.
    SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hToolbar, TB_ADDBUTTONS, sizeof(buttons)/sizeof(TBBUTTON), (LPARAM) &buttons);

    //SendMessage(hToolbar, TB_AUTOSIZE, 0, 0);

    if( !hToolbar)
    {
        MessageBox(hwnd, "Toolbar could not be created!", "Eror", MB_ICONEXCLAMATION | MB_OK);
    }
}

HWND CreateNewMDIChild(HWND hMDIClient, LPCSTR windowTitle)
{
	MDICREATESTRUCT mcs;
	HWND hChild;

	mcs.szTitle = windowTitle;
	mcs.szClass = szChildClassName;
	mcs.hOwner  = GetModuleHandle(NULL);
	mcs.x = mcs.cx = CW_USEDEFAULT;
	mcs.y = mcs.cy = CW_USEDEFAULT;
	mcs.style = MDIS_ALLCHILDSTYLES;

    hChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE,0, (LONG)&mcs);

    if(hChild == NULL)
         MessageBox(hMainWindow, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

	return hChild;
}

BOOL SetUpMDIChildWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = MDIChildWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szChildClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, "Could Not Register Child Window", "Oh Oh...",
			MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	else
		return TRUE;
}

void CreateEditors()
{
    CreateNewMDIChild(hMDIClient,(LPCSTR) "lex.l");
    CreateNewMDIChild(hMDIClient,(LPCSTR) "yacc.y");
    CreateNewMDIChild(hMDIClientLog,(LPCSTR) "log.txt");
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst,
                   char* cmdParam, int cmdShow)
{
      ::InitCommonControls();


      ///Create top window class
      WinClass winClass(szClassName, hInst, MainWndProc);
      winClass.SetMenu(IDR_MAINMENU);
      winClass.SetResIcons(ID_MAIN);

      ///Is there a running instance of this program?
      HWND hwndOther = winClass.GetRunningWindow();
      if( hwndOther != 0)
      {
            ::SetForegroundWindow(hwndOther); ///Brings the thread that created the specified window into the foreground and activates the window
            if (::IsIconic(hwndOther))        ///Determines whether the specified window is minimized (iconic).
                ::ShowWindow(hwndOther, SW_RESTORE);   ///Activates and displays the window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window
            return 0;
      }

      if(!winClass.Register())
         return 0;

      if(!SetUpMDIChildWindowClass(hInst))
		return 0;

      ///Create main window
      ResString caption (hInst, ID_CAPTION);
      WinMaker win(winClass.GetName(), winClass.GetInstance());
      win.AddCaption(caption.getString());
      win.AddStyle(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
      win.SetPosition(CW_USEDEFAULT, CW_USEDEFAULT, 480, 320);
      win.Create();



      win.Show(SW_MAXIMIZE);


      hMainWindow = win.getHWND();

      //Create lex editor and yacc editor
      CreateEditors();
      SendMessage(hMainWindow, WM_COMMAND, ID_MAINMDI_WINDOWTILE, 0);
      //SendMessage(hMainWindow, WM_COMMAND, ID_LOGMDI_WINDOWTILE, 0);

      MSG messages;
      while(GetMessage(&messages, NULL, 0, 0) > 0)
	  {
		 if (!TranslateMDISysAccel(hMDIClient, &messages))
		 {
                TranslateMessage(&messages);
			    DispatchMessage(&messages);
		 }
	  }
	  return messages.wParam;
}

///   Window Procedure called by Windows
void addNewLines(char buffer[], const char* word)
{
     int i=0, tr=0;
     char* pointer = 0;

     buffer1[0]='\0';
     while((pointer = strstr(buffer + (i-tr), word))!=0)
     {
          for(;i - tr < pointer - buffer;  i++)
               buffer1[i] = buffer[i-tr];

          buffer1[i]='\0';
          strcat(buffer1, "\r\n");
          i+=2;
          tr = tr + 2;

          for(;i-tr < (pointer - buffer) + strlen(word); i++)
               buffer1[i] = buffer[i-tr];
          buffer1[i]='\0';
     }
     while(buffer[(i-tr)] != '\0')
          {
              buffer1[i]= buffer[(i-tr)];
              i++;
          }
     buffer1[i]='\0';

     strcpy(buffer, buffer1);
}

void addNewLinesTabs(char buffer[], const char* word)
{
     int i=0, tr=0;
     char* pointer = 0;

     buffer1[0]='\0';
     while((pointer = strstr(buffer + (i-tr), word))!=0)
     {
          for(;i - tr < pointer - buffer;  i++)
               buffer1[i] = buffer[i-tr];

          buffer1[i]='\0';
          strcat(buffer1, "\r\n\t");
          i+=3;
          tr = tr + 3;

          for(;i-tr < (pointer - buffer) + strlen(word); i++)
               buffer1[i] = buffer[i-tr];
          buffer1[i]='\0';
     }
     while(buffer[(i-tr)] != '\0')
          {
              buffer1[i]= buffer[(i-tr)];
              i++;
          }
     buffer1[i]='\0';

     strcpy(buffer, buffer1);
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
		case WM_CREATE:
		{
			HWND hStatus;
			int statwidths[] = {100, -1};

			CLIENTCREATESTRUCT ccs;

			// Create MDI Client

			// Find window menu where children will be listed
			ccs.hWindowMenu  = GetSubMenu(GetMenu(hwnd), 2);
			ccs.idFirstChild = ID_MDI_FIRSTCHILD;

			hMDIClient = CreateWindowEx(WS_EX_CLIENTEDGE,
                                       "mdiclient",
                                        NULL,
				                        WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
				                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				                        hwnd,
				                        (HMENU)IDC_MAIN_MDI,
                                        GetModuleHandle(NULL), (LPVOID)&ccs);

			if(hMDIClient == NULL)
				MessageBox(hwnd, "Could not create MDI client.", "Error", MB_OK | MB_ICONERROR);


			CLIENTCREATESTRUCT ccs1;

			ccs1.hWindowMenu  = GetSubMenu(GetMenu(hwnd), 2);
			ccs1.idFirstChild = ID_MDI_FIRSTCHILD;

			hMDIClientLog = CreateWindowEx(WS_EX_CLIENTEDGE,
                                       "mdiclient",
                                        NULL,
				                        WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
				                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				                        hwnd,
				                        (HMENU)IDC_LOG_MDI,
                                        GetModuleHandle(NULL), (LPVOID)&ccs1);

			if(hMDIClientLog == NULL)
				MessageBox(hwnd, "Could not create MDI client.", "Error", MB_OK | MB_ICONERROR);

			// Create Toolbar

            CustomizedToolbar(hwnd, GetModuleHandle(NULL));

			// Create Status bar
			hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
				WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
				hwnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"Hi there :)");
		}
		break;

		case WM_SIZE:
		{
                HWND hTool;
                RECT rcTool;
                int iToolHeight;

                HWND hStatus;
                RECT rcStatus;
                int iStatusHeight;

                HWND hMDI;
                RECT rcClient;
                int iMDIHeight;


                // Size toolbar and get height

                hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
                SendMessage(hTool, TB_AUTOSIZE, 0, 0);

                GetWindowRect(hTool, &rcTool);
                iToolHeight = rcTool.bottom - rcTool.top;

                // Size status bar and get height

                hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
                SendMessage(hStatus, WM_SIZE, 0, 0);

                GetWindowRect(hStatus, &rcStatus);
                iStatusHeight = rcStatus.bottom - rcStatus.top;

                // Calculate remaining height and size edit
                // Calculate remaining height and size edit

			    GetClientRect(hwnd, &rcClient);

			    iMDIHeight = rcClient.bottom - iToolHeight - iStatusHeight;

			    hMDI = GetDlgItem(hwnd, IDC_MAIN_MDI);
                SetWindowPos(hMDI, NULL, 0, iToolHeight, rcClient.right, iMDIHeight, SWP_NOZORDER);
                /*HWND hMDI1;
                int iMDIHeight1=150;


                GetClientRect(hwnd, &rcClient);
                iMDIHeight = rcClient.bottom - iToolHeight - iStatusHeight - iMDIHeight1;

                hMDI = GetDlgItem(hwnd, IDC_MAIN_MDI);
                SetWindowPos(hMDI, NULL, 0, iToolHeight, rcClient.right, iMDIHeight, SWP_NOZORDER);

                hMDI1 = GetDlgItem(hwnd, IDC_LOG_MDI);
                SetWindowPos(hMDI1, NULL, 0, iMDIHeight + iToolHeight , rcClient.right, iMDIHeight1, SWP_NOZORDER);*/
		}
		break;
		case WM_CLOSE:
		    {
			DestroyWindow(hwnd);
			}
		break;
		case WM_DESTROY:
		    {
			PostQuitMessage(0);
		    }
		break;
		case WM_COMMAND:
			{
                switch(LOWORD(wParam))
                {
                    case ID_MDI_RESIZE:
                    {
                            HWND hTool;
                            RECT rcTool;
                            int iToolHeight;

                            HWND hStatus;
                            RECT rcStatus;
                            int iStatusHeight;

                            HWND hMDI;
                            RECT rcClient;
                            int iMDIHeight;


                            // Size toolbar and get height
                            hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
                            GetWindowRect(hTool, &rcTool);
                            iToolHeight = rcTool.bottom - rcTool.top;

                            // Size status bar and get height

                            hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
                            GetWindowRect(hStatus, &rcStatus);
                            iStatusHeight = rcStatus.bottom - rcStatus.top;

                            // Calculate remaining height and size edit
                            HWND hMDI1;
                            int iMDIHeight1=150;

                            GetClientRect(hwnd, &rcClient);
                            iMDIHeight = rcClient.bottom - iToolHeight - iStatusHeight - iMDIHeight1;

                            hMDI = GetDlgItem(hwnd, IDC_MAIN_MDI);
                            SetWindowPos(hMDI, NULL, 0, iToolHeight, rcClient.right, iMDIHeight, SWP_NOZORDER);

                            hMDI1 = GetDlgItem(hwnd, IDC_LOG_MDI);
                            SetWindowPos(hMDI1, NULL, 0, iMDIHeight + iToolHeight , rcClient.right, iMDIHeight1, SWP_NOZORDER);

                            /**HWND hChild = (HWND)SendMessage(hMDI1, WM_MDIGETACTIVE,0,0);
                            if ( hChild != NULL )
                                 {
                                     SetWindowLong(hChild, GWL_STYLE, 0);
                                     SetWindowPos(hChild, NULL, 0, 0, rcClient.right, iMDIHeight1, SWP_NOZORDER);
                                     ShowWindow(hMDIClientLog, SW_SHOW);
                                     ShowWindow(hChild, SW_SHOW);
                                 }**/
                            ShowWindow(hMDIClientLog, SW_SHOW);
                    }
                    break;

                    case ID_MDI_SIZE:
                    {
                            ShowWindow(hMDIClientLog, SW_HIDE);

                            HWND hTool;
                            RECT rcTool;
                            int iToolHeight;

                            HWND hStatus;
                            RECT rcStatus;
                            int iStatusHeight;

                            HWND hMDI;
                            RECT rcClient;
                            int iMDIHeight;


                            // Size toolbar and get height
                            hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
                            GetWindowRect(hTool, &rcTool);
                            iToolHeight = rcTool.bottom - rcTool.top;

                            // Size status bar and get height
                            hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
                            GetWindowRect(hStatus, &rcStatus);
                            iStatusHeight = rcStatus.bottom - rcStatus.top;

                            // Calculate remaining height and size edit
                            GetClientRect(hwnd, &rcClient);

                            iMDIHeight = rcClient.bottom - iToolHeight - iStatusHeight;

                            hMDI = GetDlgItem(hwnd, IDC_MAIN_MDI);
                            SetWindowPos(hMDI, NULL, 0, iToolHeight, rcClient.right, iMDIHeight, SWP_NOZORDER);
                    }
                    break;

                    case ID_FILE_SHOWWLY:
                        {
                                 ll=0; yy=0;
                                 EnumChildWindows(GetParent(hMDIClient),(ENUMWINDOWSPROC) ChildWndEnumProc, ID_FILE_SHOWWLY);
                                 if(ll==0 && yy==0)
                                 {
                                        CreateNewMDIChild(hMDIClient,(LPCSTR) "lex.l");
                                        CreateNewMDIChild(hMDIClient,(LPCSTR) "yacc.y");
                                        EnumChildWindows(GetParent(hMDIClient),(ENUMWINDOWSPROC) ChildWndEnumProc, ID_FILE_SHOWWLYEX);
                                 }
                                 else
                                      if(ll==0)
                                           {
                                               CreateNewMDIChild(hMDIClient,(LPCSTR) "lex.l");
                                               EnumChildWindows(GetParent(hMDIClient),(ENUMWINDOWSPROC) ChildWndEnumProc, ID_FILE_SHOWWLYEX);
                                           }
                                      else
                                         if(yy==0)
                                            {
                                               CreateNewMDIChild(hMDIClient,(LPCSTR) "yacc.y");
                                               EnumChildWindows(GetParent(hMDIClient),(ENUMWINDOWSPROC) ChildWndEnumProc, ID_FILE_SHOWWLYEX);
                                            }
                                  SendMessage(hMainWindow, WM_COMMAND, ID_MAINMDI_WINDOWTILE, 0);
                        }
                    break;
                    case ID_FILE_SHOWLOG:
                        {
                               HWND hChild = (HWND)SendMessage(hMDIClientLog, WM_MDIGETACTIVE,0,0);
                               HWND hEdit = GetTopWindow(hChild);

                               if( log==0 )
                                   {
                                       SendMessage(hMainWindow, WM_COMMAND, ID_MDI_RESIZE, 0);
                                       ShowWindow(hChild, SW_SHOWMAXIMIZED);
                                       log=1;
                                   }
                               else
                                   {
                                       SendMessage(hMainWindow, WM_COMMAND, ID_MDI_SIZE, 0);
                                       ShowWindow(hChild, SW_SHOWNORMAL);
                                       log=0;
                                   }

                               SendMessage(hMDIClient, WM_MDITILE, 0, 0);

                               buffer[0]='\0';
                               strcpy(file,"output\\log.txt");




                               if(hEdit==NULL)
                                    MessageBox(NULL,"Logger is not having an edit child","Error", MB_ICONINFORMATION);

                               std::ifstream fin;

                               fin.open(file);


                               if(fin.is_open())
                               {
                                   std::stringstream buffer0;
                                   buffer0 << fin.rdbuf();
                                   std::string contents(buffer0.str());
                                   strcpy(buffer, contents.c_str());

                                   /*buffer1[0]='\0';
                                   int i=0, tr=0;
                                   pointer = 0;

                                   ///error

                                   while((pointer = strstr(buffer + (i-tr), "error:"))!=0)
                                   {
                                        for(;i - tr < pointer - buffer;  i++)
                                             buffer1[i] = buffer[i-tr];

                                        buffer1[i]='\0';
                                        strcat(buffer1, "\r\n\t");
                                        i+=3;
                                        tr = tr + 3;

                                        for(;i-tr < (pointer - buffer) + 6; i++)
                                             buffer1[i] = buffer[i-tr];
                                        buffer1[i]='\0';
                                   }
                                   while(buffer[(i-tr)] != '\0')
                                        {
                                            buffer1[i]= buffer[(i-tr)];
                                            i++;
                                        }
                                   buffer1[i]='\0';


                                   ///-------------------

                                   buffer[0]='\0';
                                   i=0, tr=0;*/

                                   ///warning

                                  /* while((pointer = strstr(buffer1 + (i-tr), "warning:"))!=0)
                                   {
                                        for(;i - tr < pointer - buffer1;  i++)
                                             buffer[i] = buffer1[i-tr];

                                        buffer[i]='\0';
                                        strcat(buffer, "\r\n\t");
                                        i+=3;
                                        tr = tr + 3;

                                        for(;i-tr < (pointer - buffer1) + 8; i++)
                                             buffer[i] = buffer1[i-tr];
                                        buffer[i]='\0';
                                   }
                                   while(buffer1[(i-tr)] != '\0')
                                        {
                                            buffer[i]= buffer1[(i-tr)];
                                            i++;
                                        }
                                   buffer[i]='\0';

                                   ///--------------------*/
                                   addNewLines(buffer, "lex.l");
                                   addNewLines(buffer, "yacc.y:");

                                   addNewLines(buffer, "lex.yy.c:");
                                   addNewLines(buffer, "yacc.tab.h");
                                   addNewLines(buffer, "yacc.tab.c");

                                   addNewLinesTabs(buffer, "error:");
                                   addNewLinesTabs(buffer, "warning:");
                                   addNewLinesTabs(buffer, "note:");



                                   SendMessage(hEdit, WM_SETTEXT,sizeof(buffer)/sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));
                                   fin.close();
                               }
                        }
                    break;

                    case ID_FILE_EXIT:
                        PostMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                    case ID_FILE_NEW:
                        ::DialogBox(GetModuleHandle(NULL),
                                              MAKEINTRESOURCE(IDD_FILENEW),
                                              hwnd,
                                              AboutDlgProc);
                    break;
                    case ID_FILE_OPEN:
                        {
                         ::DialogBox(GetModuleHandle(NULL),
                                              MAKEINTRESOURCE(IDD_FILEOPEN),
                                              hwnd,
                                              AboutDlgProc);
                        }
                    break;


                    case ID_FILE_CLOSE:
                        {
                            HWND hChild = (HWND)SendMessage(hMDIClient, WM_MDIGETACTIVE,0,0);
                            if(hChild)
                            {
                                SendMessage(hChild, WM_CLOSE, 0, 0);
                            }
                        }
                    break;

                    case ID_MAINMDI_WINDOWTILE:
                        SendMessage(hMDIClient, WM_MDITILE, 0, 0);
                    break;

                    case ID_LOGMDI_WINDOWTILE:
                        SendMessage(hMDIClientLog, WM_MDITILE, 0, 0);
                    break;

                    case ID_MAINMDI_WINDOWCASCADE:
                        SendMessage(hMDIClient, WM_MDICASCADE, 0, 0);
                    break;

                    case ID_LOGMDI_WINDOWCASCADE:
                        SendMessage(hMDIClientLog, WM_MDICASCADE, 0, 0);
                    break;

                    default:
                    {
                        if(LOWORD(wParam) >= ID_MDI_FIRSTCHILD)
                            {
                                DefFrameProc(hwnd, hMDIClient, WM_COMMAND, wParam, lParam);
                            }
					else
					{
						HWND hChild = (HWND)SendMessage(hMDIClient, WM_MDIGETACTIVE,0,0);
						if(hChild)
                            {
                                SendMessage(hChild, WM_COMMAND, wParam, lParam);
                            }
					}
                    }
                }
			}
		break;
		default:
			return DefFrameProc(hwnd, hMDIClient, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK MDIChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
		    HDC hdc;
            long lfHeight;
            HFONT hf_font;

		    HWND hEdit;

            // Create Edit Control
            hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                                                   "EDIT",
                                                   "",
                                                   WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                                   20 , 0, 100, 100,
                                                   hwnd,
                                                   (HMENU) IDC_CHILD_EDIT,
                                                   GetModuleHandle(NULL), NULL);



			if(hEdit == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);



            hdc = GetDC(NULL);
            lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            ReleaseDC(NULL, hdc);
            hf_font = CreateFont(lfHeight * font_coef, 0, 0, 0, FW_THIN, FALSE, 0, 0 , 0, 0, 0, 0, 0, "Tahoma");
            //DeleteObject(hf_font);

            SendMessage(hEdit, WM_SETFONT, (WPARAM) hf_font, MAKELPARAM(FALSE, 0));

            //DeleteObject(hf_font);
		}
		break;

		case WM_MDIACTIVATE:
		{
			HMENU hMenu, hFileMenu;
			UINT EnableFlag;


			hMenu = GetMenu(hMainWindow);
			if(hwnd == (HWND)lParam)
			{	   //being activated, enable the menus
				EnableFlag = MF_ENABLED;
			}
			else
			{						   //being de-activated, gray the menus
				EnableFlag = MF_GRAYED;
			}

			EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
			EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);
			EnableMenuItem(hMenu, 3, MF_BYPOSITION | EnableFlag);

			hFileMenu = GetSubMenu(hMenu, 0);

			EnableMenuItem(hFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | EnableFlag);
			EnableMenuItem(hFileMenu, ID_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
			EnableMenuItem(hFileMenu, ID_FILE_CLOSEALL, MF_BYCOMMAND | EnableFlag);

			DrawMenuBar(hMainWindow);
		}
		break;
		case WM_COMMAND:
        {
                switch(LOWORD(wParam))
                {


                    case ID_FILE_SAVE:
                    {
                                 /*char title[256];
                                 char buffer[40000];
                                 char file[256]="output\\";*/
                                 title[0]='\0'; buffer[0]='\0'; strcpy(file,"output\\");


                                 GetWindowText(hwnd, title, sizeof(title)/sizeof(title[0]));
                                 SendMessage(GetDlgItem((hwnd), IDC_CHILD_EDIT), WM_GETTEXT, sizeof(buffer)/sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));


                                 strcat(file, title);

                                 std::ofstream fout(file);

                                 if(fout.is_open())
                                      {
                                          //MessageBox(NULL, "Opened", "Message", MB_OK | MB_ICONINFORMATION);
                                          fout<<buffer;
                                          fout.close();
                                      }
                                //MessageBox(NULL, title, "Message", MB_OK | MB_ICONINFORMATION);
                    }
                    break;


                    case ID_FILE_SAVEALL:
                    {
                         EnumChildWindows(GetParent(hwnd),(ENUMWINDOWSPROC) ChildWndEnumProc, ID_FILE_SAVEALL);
                    }
                    break;

                    case ID_FILE_SAVEAS:
                    {
                        ::DialogBox(GetModuleHandle(NULL),
                                              MAKEINTRESOURCE(IDD_FILESAVEAS),
                                              GetParent(hMDIClient),
                                              AboutDlgProc);
                    }
                    break;

                    case ID_FILE_RUNL:
                        {
                             ShellExecute(NULL,"open","output\\win-flex.bat",NULL,NULL,SW_SHOWNORMAL);
                        }
                    break;

                    case ID_FILE_RUNY:
                        {
                             ShellExecute(NULL,"open","output\\win-bison.bat",NULL,NULL,SW_SHOWNORMAL);
                        }
                    break;

                    case ID_FILE_RUN:
                        {
                            SHELLEXECUTEINFO ShExecInfo = {0};
                            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
                            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
                            ShExecInfo.hwnd = NULL;
                            ShExecInfo.lpVerb = NULL;
                            ShExecInfo.lpFile = "output\\runc.bat";
                            ShExecInfo.lpParameters = "";
                            ShExecInfo.lpDirectory = NULL;
                            ShExecInfo.nShow = SW_SHOW;
                            ShExecInfo.hInstApp = NULL;
                            ShellExecuteEx(&ShExecInfo);
                            WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
                            CloseHandle(ShExecInfo.hProcess);

                            if(((int)ShellExecute(NULL,"open","output\\result.exe",NULL,NULL,SW_SHOWNORMAL))> 32)
                                {
                                    std::ofstream fout("output\\log.txt");
                                    if(fout.is_open())
                                         {
                                                fout<<"Source files have been generated and compiled! Running executable.";
                                                fout.close();
                                         }
                                }
                            //hellExecute(NULL,"open","output\\runc.bat",NULL,NULL,SW_SHOWNORMAL);
                            log=0;
                            SendMessage(hMainWindow, WM_COMMAND, ID_FILE_SHOWLOG, 0);

                        }
                    break;




                    case ID_FONT_PLUS:
                        {
                            if(font_coef < 40)
                                 font_coef += 0.2;
                            HDC hdc;
                            long lfHeight;
                            HFONT hf_font;

                            Beep(23,23);


                            hdc = GetDC(NULL);
                            lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 75);
                            ReleaseDC(NULL, hdc);
                            hf_font = CreateFont(lfHeight * font_coef, 0, 0, 0, FW_THIN, FALSE, 0, 0 , 0, 0, 0, 0, 0, "Tahoma");

                            SendMessage( GetDlgItem(hwnd, IDC_CHILD_EDIT), WM_SETFONT, (WPARAM) hf_font, TRUE);

                            //DeleteObject(hf_font);
                        }
                    break;

                    case ID_FONT_MINUS:
                        {
                            if(font_coef > 0.3)
                                  font_coef -= 0.2;
                            HDC hdc;
                            long lfHeight;
                            HFONT hf_font;

                            Beep(23,23);

                            hdc = GetDC(NULL);
                            lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 75);
                            ReleaseDC(NULL, hdc);
                            hf_font = CreateFont(lfHeight * font_coef, 0, 0, 0, FW_THIN, FALSE, 0, 0 , 0, 0, 0, 0, 0, "Tahoma");

                            SendMessage(GetDlgItem(hwnd, IDC_CHILD_EDIT),WM_SETFONT,(WPARAM)hf_font,TRUE);

                            //DeleteObject(hf_font);
                        }
                    break;

                    case ID_FILE_UNDO:
                    {
                        SendMessage(GetDlgItem(hwnd,IDC_CHILD_EDIT), EM_UNDO, 0, 0);
                    }
                    break;

                    case ID_FILE_REDO:
                    {
                        SendMessage(GetDlgItem(hwnd,IDC_CHILD_EDIT), EM_UNDO, 0, 0);
                    }
                    break;

                    case ID_EDIT_CUT:
                    {
                        SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_CUT, 0, 0);
                    }
                    break;

                    case ID_EDIT_COPY:
                    {
                        SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_COPY, 0, 0);
                    }
                    break;

                    case ID_EDIT_PASTE:
                    {
                        SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_PASTE, 0, 0);
                    }
                    break;
                }
        }
		break;

		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			// Calculate remaining height and size edit and line numbers section

			GetClientRect(hwnd, &rcClient);

			hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
		default:
			return DefMDIChildProc(hwnd, msg, wParam, lParam);

	}
	return 0;
}

BOOL CALLBACK ChildWndEnumProc(HWND hwnd, LPARAM lParam)
{
    title[0]='\0'; buffer[0]='\0';
    strcpy(file,"output\\");

    switch(LOWORD((lParam)))
    {
        case ID_FILE_SAVEALL:
            {
                 //char title[256];
                 GetClassName(hwnd, title, sizeof(title)/sizeof(title[0]));

                 if(strcmp(title, szChildClassName)==0)
                 {
                     //char buffer[40000];

                     GetWindowText(hwnd, title, sizeof(title)/sizeof(title[0]));
                     SendMessage(GetDlgItem((hwnd), IDC_CHILD_EDIT), WM_GETTEXT, sizeof(buffer)/sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));

                     //char file[256]="output\\";
                     strcat(file, title);

                     std::ofstream fout(file);

                     if(fout.is_open())
                          {
                              //MessageBox(NULL, "Opened", "Message", MB_OK | MB_ICONINFORMATION);
                              fout<<buffer;
                              fout.close();
                          }
                    //MessageBox(NULL, title, "Message", MB_OK | MB_ICONINFORMATION);
                    return 1;
                  }
                  return 1;
            }
        break;

        case ID_FILE_SHOWWLYEX:
             {
                 //char title[256];
                 GetClassName(hwnd, title, sizeof(title)/sizeof(title[0]));

                 if(strcmp(title, szChildClassName)==0)
                 {
                     GetWindowText(hwnd, title, sizeof(title)/sizeof(title[0]));
                     //char file[256]="output\\";
                     //char buffer[400000];
                     if( (ll==0 && strcmp(title, "lex.l")==0) ||  (yy==0 && strcmp(title, "yacc.y")==0) )
                     {

                           HWND hEdit = GetTopWindow(hwnd);

                           strcat(file, title);

                           if(hEdit==NULL)
                               MessageBox(NULL,"Window is not having an edit child","Error",MB_ICONINFORMATION);

                           std::ifstream fin;
                           fin.open(file);
                           std::string line;
                           std::string contents;

                           if(fin.is_open())
                           {
                               while (std::getline(fin, line))
                               {
                                   line += "\r\n";
                                   contents += line;
                               }

                               strcpy(buffer, contents.c_str());

                               SendMessage(hEdit, WM_SETTEXT,sizeof(buffer)/sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));
                               fin.close();
                           }
                     }
                  }
                  return 1;
            }
        break;


        case ID_FILE_SHOWWLY:
             {
                 //char title[256];
                 GetClassName(hwnd, title, sizeof(title)/sizeof(title[0]));


                 if(strcmp(title, szChildClassName)==0)
                 {
                     GetWindowText(hwnd, title, sizeof(title)/sizeof(title[0]));
                     if(strcmp(title, "lex.l")==0)
                     {
                           ll = 1;
                     }
                     else if(strcmp(title, "yacc.y")==0)
                          {
                                yy = 1;
                          }
                  }
                  return 1;
            }
        break;

        default:
            return 0;
    }
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    buffer[0]='\0';
    switch(message)
    {
        case WM_INITDIALOG:
            return TRUE;
         break;
        case WM_COMMAND:
            switch (LOWORD((wParam)))
            {
                    case IDOKNEW:
                    {
                        //char buffer[256];
                        SendMessage(GetDlgItem(hwnd,ID_EDITTEXT3),    ///returns the handle of the edit box
                                      WM_GETTEXT,
                                      sizeof(buffer)/sizeof(buffer[0]),
                                      reinterpret_cast<LPARAM>(buffer));

                        //MessageBox(NULL,(LPCSTR) buffer,"Information",MB_ICONINFORMATION);
                        ::EndDialog(hwnd, 0);

                        if( strchr(buffer,'.') )
                             CreateNewMDIChild(hMDIClient, buffer);
                        else
                             CreateNewMDIChild(hMDIClient,"Untitled");
                    }
                    break;
                    case IDOKOPEN:
                    {
                        //LPWSTR buffer[256];
                        SendMessage(GetDlgItem(hwnd,ID_EDITTEXT1),    ///returns the handle of the edit box
                                      WM_GETTEXT,
                                      sizeof(buffer)/sizeof(buffer[0]),
                                      reinterpret_cast<LPARAM>(buffer));

                       // MessageBox(NULL,(LPCSTR) buffer,"Information",MB_ICONINFORMATION);
                        ::EndDialog(hwnd, 0);
                    }
                    break;

                    case IDOKSAVEAS:
                    {
                        //LPWSTR buffer[256];
                        SendMessage(GetDlgItem(hwnd,ID_EDITTEXT2),    ///returns the handle of the edit box
                                      WM_GETTEXT,
                                      sizeof(buffer)/sizeof(buffer[0]),
                                      reinterpret_cast<LPARAM>(buffer));

                        //MessageBox(NULL,(LPCSTR) buffer,"Information",MB_ICONINFORMATION);
                        ::EndDialog(hwnd, 0);
                    }
                    break;

                    case IDCANCEL:
                        ::EndDialog(hwnd, 0);
                        return TRUE;
                    break;
            }
    }
    return FALSE;
}
