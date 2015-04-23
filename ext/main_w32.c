#include <windows.h>
#include "../front.h"

static void render(HWND hwnd)
{
  int i, j;
  int w, h;
  int w2;
  unsigned char* p;
  unsigned char* q;
  BITMAPINFOHEADER bmih = {0};
  HDC hdc;
  HBITMAP hdib;
  VOID* pdib;
  PAINTSTRUCT ps;
  w = front_width();
  h = front_height();
  w2 = w * 3;
  /* every line should be a multiple of 4 bytes ? */
  if (w2 % 4)
    w2 += (4 - (w2 % 4));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biWidth = w;
  /* negative means origin at top-left. */
  bmih.biHeight = -h;
  bmih.biPlanes = 1;
  bmih.biBitCount = 24;
  bmih.biCompression = BI_RGB;
  bmih.biSizeImage = 0; /* ok for BI_RGB */
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  hdc = BeginPaint(hwnd, &ps);
  hdib = CreateDIBSection(hdc, (const BITMAPINFO*)&bmih,
      DIB_RGB_COLORS, &pdib, 0, 0);
  p = front_pixels();
  for (i = 0; i < h; ++i) {
    q = pdib;
    q += w2 * i;
    for (j = 0; j < w; ++j) {
      q[0] = p[2];
      q[1] = p[1];
      q[2] = p[0];
      p += 3;
      q += 3;
    }
  }
  SelectObject(hdc, hdib);
  SetDIBitsToDevice(hdc,
      0, 0,
      w, h,
      0, 0,
      0, h,
      pdib,
      (const BITMAPINFO*)&bmih,
      DIB_RGB_COLORS);
  EndPaint(hwnd, &ps);
  DeleteObject(hdib);
}

#define WORDBITS (sizeof(WORD) * 8)
#define WORDMASK ((1 << WORDBITS) - 1)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  double x, y;
  /* this state var protects against the key repetition
     system commonly active in Windows, i.e. holding down
     "z" will send tons of reptitive WM_CHAR messages */
  static int down = 0;
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_PAINT:
      render(hwnd);
      return 0;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
      x = lParam & WORDMASK;
      y = lParam >> WORDBITS;
      front_mouse_down(x, y);
      return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
      x = lParam & WORDMASK;
      y = lParam >> WORDBITS;
      front_mouse_up(x, y);
      InvalidateRect(hwnd, NULL, FALSE);
      return 0;
    case WM_CHAR:
      if (down == 0) {
        front_key_down(wParam);
        down = 1;
      }
      return 0;
    case WM_KEYUP:
      if (down == 1) {
        front_key_up();
        down = 0;
      }
      InvalidateRect(hwnd, NULL, FALSE);
      return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
  char const class_name[] = "My Window Class";
  int my_argc;
  char* my_argv[3];
  WNDCLASS wc = {0};
  HWND hwnd;
  MSG msg = {0};
  /* so far this may work for one argument */
  my_argc = (strlen(lpCmdLine) == 0 ? 1 : 2);
  my_argv[0] = "viewer";
  my_argv[1] = lpCmdLine;
  my_argv[2] = NULL;
  front_start(my_argc, my_argv);
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = class_name;
  RegisterClass(&wc);
  hwnd = CreateWindowEx(
      0,
      class_name,
      "viewer",
      WS_OVERLAPPEDWINDOW,
      20, 20, 640, 480,
      NULL,
      NULL,
      hInstance,
      NULL);
  if (hwnd == NULL)
    return 0;
  ShowWindow(hwnd, nCmdShow);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  front_stop();
  return 0;
}
