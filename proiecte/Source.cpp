#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

#define IDC_ADD 100
#define IDC_OPP 101
#define IDC_PLUS 10
#define IDC_ORI  11

//----------SSTack Algoritm----------------------
struct SStack
{
	int data;
	SStack *next;
};

SStack *head1, *head2;
SStack *tail;
SStack *head_0;

void add_item(int val)
{
	SStack *aux = (SStack*)malloc(sizeof(SStack));
	aux->data = val;
	aux->next = NULL;
	if (head1 == NULL)
	{
		head1 = aux;
		tail = aux;
		return;
	}

	if (head2 == NULL)
	{
		head1->next = aux;
		head2 = aux;
		tail = aux;
	}
	else
	{
		tail->next = aux;
		tail = aux;
	}
	return;
}
void operation(char op)
{
	int x;

	if (op == '*')
		x = (head1->data)*(head2->data);
	else if (op == '+')
		x = (head1->data) + (head2->data);

	head1->data = x;
	head2->data = 0;
	if (head_0 == NULL)
		head_0 = head2;
	head2 = head2->next;
	head1->next = head2;
}
//-----------------------------------------------


HINSTANCE _hInstance;
int _nCmdShow;
MSG msg;
HWND _mainwnd;
HWND _valuare;

int nr_date = 0, cont = 0;
int nr_date_proc = 0;
int opperation = 0;

bool verifica_numeric(char *sir)
{
	if (strlen(sir) == 0)
		return false;
	for (int i = 0; i < strlen(sir); i++)
	{
		if (!isdigit(sir[i])) // verifica daca e numar
			return false;
	}
	return true;
}
void Butoane(HWND hwnd)
{
	//Butoane de tip Ratio  // AM CREAT BUTOANELE PENTRU OPERATII
	CreateWindowW(L"Button", L"Operation",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		870, 300, 115, 80, hwnd, (HMENU)0, _hInstance, NULL);
	CreateWindowW(L"Button", L"*",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		880, 345, 85, 30, hwnd, (HMENU)IDC_ORI, _hInstance, NULL);
	CreateWindowW(L"Button", L"+",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		880, 320, 85, 30, hwnd, (HMENU)IDC_PLUS, _hInstance, NULL);

	//									BUTON INCONJURAT/COPIL AL UNUI BUTON/ ... COORDONATE
	CreateWindow("BUTTON", "Add Number", WS_BORDER | WS_CHILD | WS_VISIBLE, 870, 100, 130, 30, hwnd, (HMENU)IDC_ADD, _hInstance, NULL);
	CreateWindow("BUTTON", "Execute", WS_BORDER | WS_CHILD | WS_VISIBLE, 870, 400, 130, 30, hwnd, (HMENU)IDC_OPP, _hInstance, NULL);

	_valuare = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 870, 50, 80, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
	// 
}
void PaintWND(HWND hwnd)
{
	//Punem backgroundul AM DECLARAT VARIABILELE PENTRU DESENARE
	HDC         hDC;
	PAINTSTRUCT Ps;
	HBRUSH      NewBrush;

	hDC = BeginPaint(hwnd, &Ps);

	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	qLineColor = RGB(255, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 4, qLineColor); // LINIA SUBTIRE
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);
	MoveToEx(hDC, 840, 0, NULL);
	LineTo(hDC, 840, 500);

	hLinePen = CreatePen(PS_SOLID, 8, qLineColor);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);
	HBRUSH      brCross = CreateHatchBrush(HS_HORIZONTAL, RGB(222, 63, 121)); //AICI CHENARUL MARE
	SelectObject(hDC, brCross);
	RoundRect(hDC, 4, 4, 840, 497, 0, 0);

	if (nr_date > 0) // chenarul cu numarul
	{
		qLineColor = RGB(0, 0, 0);
		hLinePen = CreatePen(PS_SOLID, 4, qLineColor);
		hPenOld = (HPEN)SelectObject(hDC, hLinePen);
		HBRUSH solid = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hDC, solid);

		//Desenam stiva cu datele din ea
		SStack *aux = head1;
		int i = 1 + nr_date_proc;
		char buff[10];
		while (aux != NULL)
		{
			for (int j = 0; j < 10; j++)
				buff[j] = '\0';
			sprintf(buff, "%d\0", aux->data);

			Rectangle(hDC, 30 + (i - 1) * 135, 300, 130 + (i - 1) * 135, 340); // deseneaza patratul

			SetTextColor(hDC, RGB(0, 0, 255));
			TextOut(hDC, 36 + (i - 1) * 135, 310, buff, 8);

			if (i > 1 + nr_date_proc)
			{
				//Linia
				MoveToEx(hDC, 130 + (i - 2) * 135, 318, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 318);
				//Sagetuta
				MoveToEx(hDC, 150 + (i - 2) * 135, 308, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 318);
				MoveToEx(hDC, 150 + (i - 2) * 135, 328, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 318);
			}

			aux = aux->next;
			i++;
		}
		//Desenam stiva cu datele procesate
		aux = head_0;
		i = 2;
		while (i <= nr_date_proc + 1 && aux != NULL)
		{

			Rectangle(hDC, 30 + (i - 1) * 135, 200, 130 + (i - 1) * 135, 240);

			SetTextColor(hDC, RGB(0, 0, 255));
			TextOut(hDC, 36 + (i - 1) * 135, 210, "NULL", 4);

			if (i > 2)
			{
				//Linia
				MoveToEx(hDC, 130 + (i - 2) * 135, 218, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 218);
				//Sagetuta
				MoveToEx(hDC, 150 + (i - 2) * 135, 208, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 218);
				MoveToEx(hDC, 150 + (i - 2) * 135, 228, NULL);
				LineTo(hDC, 165 + (i - 2) * 135, 218);
			}

			aux = aux->next;
			i++;
		}
		if (nr_date_proc)
		{
			//Linia
			MoveToEx(hDC, 130 + (i - 2) * 135, 218, NULL); // ultima linuta
			LineTo(hDC, 165 + (i - 2) * 135, 318);
		}

	}

}
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		PaintWND(hWnd);
		break;

	case WM_CREATE:
		Butoane(hWnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_ADD:

			char buff[5];
			for (int i = 0; i <= 4; i++)
				buff[i] = '\0';

			SendMessage(_valuare, WM_GETTEXT, sizeof(buff) / sizeof(char), reinterpret_cast<LPARAM>(buff));

			DestroyWindow(_valuare);//PENTRU A sterge numarul din chenar
			_valuare = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 870, 50, 80, 30, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

			if (verifica_numeric(buff) && atoi(buff) < 100 && cont<6)//
			{
				int x = atoi(buff);
				cont++;
				nr_date++;
				add_item(x);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (cont >= 6)
			{
				MessageBoxW(hWnd, L"More than 6 value!", L"Error", MB_ICONERROR);
			}
			else if (verifica_numeric(buff) && atoi(buff) >= 100)
			{
				MessageBoxW(hWnd, L"Number greater than 100!", L"Error", MB_ICONERROR);
			}
			else
			{
				MessageBoxW(hWnd, L"Invalide Value!", L"Error", MB_ICONERROR);
			}

			break;

		case IDC_PLUS:
			opperation = 1;
			break;

		case IDC_ORI:
			opperation = 2;
			break;

		case IDC_OPP:
			if (opperation == 0)
				MessageBoxW(hWnd, L"Please select an opperation!", L"Error", MB_ICONERROR);
			else if (nr_date<3)
				MessageBoxW(hWnd, L"Need more numbers!", L"Error", MB_ICONERROR);
			else
			{
				nr_date_proc++;
				nr_date--; // scade o casuta de jos
				if (opperation == 1)
					operation('+');
				else if (opperation == 2)
					operation('*');

				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	_nCmdShow = nShowCmd;
	_hInstance = hInst;

	//Inregistram clasa ferestrei principale cu toate atributele ei
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = _hInstance;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Main";			//Numele procedurii pe care o va folosi
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW | CS_VREDRAW;

	//Eraare daca nu am putut *inregistra fereastra principala
	if (!RegisterClassEx(&wClass)){
		int nResult = GetLastError();
		MessageBox(NULL, "Nu sa putut inregistra fereastra!", "Error", MB_ICONERROR);
	}

	//Creem fereastra principala
	HWND hWnd = CreateWindowEx(NULL, "Main", "Spaghetti Stack", WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 1042, 547, NULL, NULL, hInst, NULL);
	_mainwnd = hWnd;

	//Eroare daca nu am putut *creea fereastra principala
	if (!hWnd){
		int nResult = GetLastError();
		MessageBox(NULL, "Nu sa creat fereastra Principala", "Error", MB_ICONERROR);
	}

	//Afisam fereastra principala
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	ZeroMemory(&msg, sizeof(MSG));

	//Loop Message
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);//aici ajung comenzile, fiecare comanda facuta
		DispatchMessage(&msg);
	}

	return 0;
}