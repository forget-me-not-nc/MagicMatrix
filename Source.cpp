#define ID_CHECKBUTTON	9001
#define ID_COMBOBOX		9002
#define	ID_LOG	9003
#define	ID_STATIC1	9004

#include <windows.h>
#include <richedit.h>
#include <CommCtrl.h>
#include <string>
#include "resource.h"
//---------------------------------------------
using std::string;
//---------------------------------------------
CONST CHAR matrixSize[][4] = {"2x2", "3x3", "4x4", "5x5"};
HWND hMainWindow;
HINSTANCE hInstance;
INT row_col = 0;
//---------------------------------------------
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
//---------------------------------------------
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG			msg{};
	HWND		hwnd;
	WNDCLASSEX	wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = L"MainWindow";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindowEx(NULL, wc.lpszClassName, L"MatrixChecker", WS_OVERLAPPEDWINDOW, 350, 100, 850, 550, NULL, NULL, wc.hInstance, NULL); hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
	hMainWindow = hWnd;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStartButton = CreateWindow(L"BUTTON", L"Check", BS_FLAT | BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			50, 340, 120, 40, hWnd, (HMENU)ID_CHECKBUTTON, hInstance, NULL);

		HWND hComboBox = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			65, 180, 80, 100, hWnd, (HMENU)ID_COMBOBOX, hInstance, NULL);

		CHAR temp[4];
		memset(&temp, 0, sizeof(temp));

		for (int i = 0; i < 4; i++)
		{
			strcpy_s(temp,(CHAR*)matrixSize[i]);
			SendMessageA((HWND)GetDlgItem(hWnd, ID_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)temp);
		}

		SendMessage((HWND)GetDlgItem(hWnd, ID_COMBOBOX), CB_SETCURSEL, (WPARAM)1, (LPARAM)0);

		HWND hStaticText_Log = CreateWindow(L"STATIC", L"Log:", WS_CHILD | WS_VISIBLE, 253, 2, 100, 30, hWnd, (HMENU)ID_STATIC1, hInstance, NULL);
		
		LoadLibrary(L"riched20.dll"); 

		HWND hLog = CreateWindowEx(NULL, RICHEDIT_CLASS, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_MULTILINE | ES_SAVESEL | ES_SUNKEN,
			250, 20, 500, 450, hWnd, (HMENU)ID_LOG, hInstance, NULL);

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case ID_CHECKBUTTON:
			{
				INT item = SendMessageA((HWND)GetDlgItem(hWnd, ID_COMBOBOX), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				CHAR mSize[4];
				memset(&mSize, 0, sizeof(mSize));
				SendMessageA((HWND)GetDlgItem(hWnd, ID_COMBOBOX), CB_GETLBTEXT, (WPARAM)item, (LPARAM)mSize);
				mSize[3] = '\0';

				if (!strcmp(mSize, "2x2"))
				{
					row_col = 2;
					ShowWindow(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc), 1);
				}
				else if (!strcmp(mSize, "3x3"))
				{
					row_col = 3;
					ShowWindow(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DlgProc), 1);
				}
				else if (!strcmp(mSize, "4x4"))
				{
					row_col = 4;
					ShowWindow(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, DlgProc), 1);
				}
				else
				{
					row_col = 5;
					ShowWindow(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, DlgProc), 1);
				}

				break;
			}

			default:break;
		}

		break;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO min_max = (LPMINMAXINFO)lParam;

		min_max->ptMinTrackSize.x = 850;
		min_max->ptMinTrackSize.y = 550;

		min_max->ptMaxTrackSize.x = 850;
		min_max->ptMaxTrackSize.y = 550;

		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(128, 128, 128));

		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	case WM_PAINT:
	{

		break;
	}
	case WM_DESTROY:
	{
		DestroyWindow(hWnd);
		PostQuitMessage(EXIT_SUCCESS);

		break;
	}

		default: break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			

			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:
				{
					INT textLength = 0;
					INT counter = 0;
					BOOL flag = false;
					string log = "-----------------------------------\nMatrix:\n";


					for (int i = 0; i < row_col * row_col; i++)
					{
						textLength = GetWindowTextLength((HWND)GetDlgItem(hwndDlg, IDC_EDIT1 + counter));

						if (!textLength)
						{
							flag = true;
							break;
						}

						counter++;
					}

					counter = 0;

					if (flag)
					{
						MessageBox(hwndDlg, L"Fill all fields", L"Error", MB_ICONERROR);

						break;
					}

					INT** Matrix = new INT * [row_col];
					for (int i = 0; i < row_col; i++)
					{
						Matrix[i] = new INT[row_col];
					}

					for (int i = 0; i < row_col; i++)
					{
						for (int j = 0; j < row_col; j++)
						{
							textLength = GetWindowTextLength((HWND)GetDlgItem(hwndDlg, IDC_EDIT1 + counter));

							CHAR* buffer = new CHAR[textLength + 1];
							GetWindowTextA((HWND)GetDlgItem(hwndDlg, IDC_EDIT1 + counter), buffer, textLength + 1);
							log += buffer;
							log += "---";
							buffer[textLength] = '\0';

							Matrix[i][j] = atoi(buffer);

							counter++;

							delete[] buffer;
						}

						log += '\n';
					}

					log += "-----------------------------------\n";

					INT sum1 = 0;
					INT sum2 = 0;

					BOOL magic_flag = TRUE;

					for (int i = 0; i < row_col; i++)
					{
						for (int j = 0; j < row_col; j++)
						{
							sum1 += Matrix[i][j];
						}

						for (int k = 0; k < row_col; k++)
						{
							sum2 += Matrix[k][i];
						}

						if (sum1 != sum2)
						{
							magic_flag = FALSE;
						}
					}

					if (magic_flag)
					{
						log += "Matrix IS magic..\n";
						MessageBox(hMainWindow, L"Matrix is magic!", L"Success", MB_OK);
					}
					else
					{
						log += "Matrix IS NOT magic..\n";
						MessageBox(hMainWindow, L"Matrix IS NOT magic!", L"Success", MB_ICONEXCLAMATION);
					}

					log += "-----------------------------------\n";

					int originalLenght = GetWindowTextLength((HWND)GetDlgItem(hMainWindow, ID_LOG));
					SendMessage((HWND)GetDlgItem(hMainWindow, ID_LOG), EM_SETSEL, (WPARAM)originalLenght, (LPARAM)originalLenght);
					SendMessageA((HWND)GetDlgItem(hMainWindow, ID_LOG), EM_REPLACESEL, 0, (LPARAM)log.c_str());

					for (int i = 0; i < row_col; i++) delete[] Matrix[i];
					delete[] Matrix;

					SendMessage(hwndDlg, WM_DESTROY, 0, 0);

					break;
				}
				case IDCANCEL:
				{
					SendMessage(hwndDlg, WM_DESTROY, 0, 0);

					break;
				}

				default:break;
			}

			break;
		}
		case WM_DESTROY:
		{
			EndDialog(hwndDlg, 0);

			break;
		}

		default: return FALSE;
	}

	return TRUE;
}