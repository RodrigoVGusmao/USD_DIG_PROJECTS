#include<windows.h>
#include<Commctrl.h>

#include"resource.h"

DLGPROC ss;

INT_PTR CALLBACK customPenProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<DWORD> *data;

	switch (msg)
	{
		case WM_INITDIALOG:
		{
			data = reinterpret_cast<std::vector<DWORD>*>(lParam);

			HWND spin = GetDlgItem(hwnd, IDC_SPIN1);
			HWND spinEdit = GetDlgItem(hwnd, IDC_EDIT_COUNT);
			SendMessage(spin, UDM_SETBUDDY, reinterpret_cast<WPARAM>(spinEdit), 0);
			SendMessage(spin, UDM_SETRANGE, 0, MAKELPARAM(8, 1));
			SetWindowText(spinEdit, L"1");

			for (uint8_t i = 0; i < 8; ++i)
			{
				HWND editControl = GetDlgItem(hwnd, IDC_EDIT1 + 2*i);
				SetWindowText(editControl, L"0");
				editControl = GetDlgItem(hwnd, IDC_EDIT1 + 2*i+1);
				SetWindowText(editControl, L"0");
			}
			return TRUE;
		}

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_EDIT_COUNT:
				{
					if (HIWORD(wParam) == EN_UPDATE)
					{
						std::wstring wndText;
						wndText.resize(GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_COUNT))+1);
						GetWindowText(GetDlgItem(hwnd, IDC_EDIT_COUNT), const_cast<wchar_t*>(wndText.c_str()), wndText.size());
						if (_wtoi(wndText.c_str()) > 8)
						{
							SetWindowText(GetDlgItem(hwnd, IDC_EDIT_COUNT), L"8");
							wndText = L"8";
						}
						else if (_wtoi(wndText.c_str()) < 1)
						{
							SetWindowText(GetDlgItem(hwnd, IDC_EDIT_COUNT), L"1");
							wndText = L"1";
						}

						for (uint8_t i = 0; i < 8; ++i)
						{
							HWND editControl = GetDlgItem(hwnd, IDC_EDIT1 + 2*i);
							EnableWindow(editControl, i < _wtoi(wndText.c_str()) ? TRUE : FALSE);
							editControl = GetDlgItem(hwnd, IDC_EDIT1 + 2*i+1);
							EnableWindow(editControl, i < _wtoi(wndText.c_str()) ? TRUE : FALSE);
						}
					}
					return TRUE;
				}

				case IDC_BUTTON1:
				{
					std::wstring wndText;
					wndText.resize(GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_COUNT)) + 1);
					GetWindowText(GetDlgItem(hwnd, IDC_EDIT_COUNT), const_cast<wchar_t*>(wndText.c_str()), wndText.size());

					data->clear();
					data->resize(2*_wtoi(wndText.c_str()));
					for (uint8_t i = 0; i < 2*_wtoi(wndText.c_str()); ++i)
					{
						std::wstring paramText;
						paramText.resize(GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1+i)) + 1);
						GetWindowText(GetDlgItem(hwnd, IDC_EDIT1+i), const_cast<wchar_t*>(paramText.c_str()), paramText.size());
						
						data->at(i) = _wtoi(paramText.c_str());
					}
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
			break;
	}
	return FALSE;
}