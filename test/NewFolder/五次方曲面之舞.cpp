#include "C:/UserFiles/Library/Math/src/Matrix/Matrix.h"
#include "C:/UserFiles/Library/Engineering/Graphics/src/Graphics2D.h"
#include "C:/UserFiles/Library/Engineering/Graphics/src/Graphics3D.h"
#include "C:/UserFiles/Library/Engineering/Graphics/src/Modeling.h"

#include <Windows.h>
#include <cmath>
#include <time.h>
#include <thread>
#include <mutex>
#include <string>

#define M_PI 3.141592653589
mutex m;
Mat<int> coff;

void work_thread(int id, Mat<double>& ran, vector<vector<double>>& Tri) {
    vector<double> p1(3), p2(3), p3(3);
    vector<int> N(3);

    int x = id % 2, y = (id / 2) % 2, z = (id / 4) % 2;

    p1 = { -3.2 + x * 3.2, -3.2 + y * 3.2, -3.2 + z * 3.2 };
    p2 = { -3.2 + (x + 1) * 3.2, -3.2 + (y + 1) * 3.2, -3.2 + (z + 1) * 3.2 };
    N = { 70 / 2 + 1, 70 / 2 + 1, 70 / 2 + 1 };
    float vec[4];

    Graphics::MarchingCubes(
        [&ran, &vec](float x, float y, float z) {
            float ans = 0;
            vec[0] = 1;
            vec[1] = x;
            vec[2] = y;
            vec[3] = z;

            for (int d = 0; d < 4; d++) {
                float dd = vec[d];

                for (int e = d; e < 4; e++) {
                    float ee = dd * vec[e];

                    for (int i = e; i < 4; i++) {
                        float ii = ee * vec[i];

                        for (int j = i; j < 4; j++) {
                            float jj = ii * vec[j];

                            for (int k = j; k < 4; k++) {
                                float kk = jj * vec[k];
                                int t = d * 4 * 4 * 4 * 4 + e * 4 * 4 * 4 + i * 4 * 4 + j * 4 + k;

                                ans += ran[t] * kk;
                            }
                        }
                    }
                }
            }
        return ans;}, p1, p2, N, Tri
    );
}


void work(Mat<unsigned int>& a) {
    static int n = -1, fg = 1;
    static Mat<double> ratateMat(3, 3);
    static vector<double> p(3), p1(3), p2(3), p3(3);
    static vector<int> pi(3), pi1(3), pi2(3), pi3(3);
    static Mat<int> Z_buf(a.rows, a.cols);
    static unsigned int color[3] = { 0 };
    static unsigned int color_st = 0xFFFFFF, color_ed = 0xFFFFFF;
    int color_T = 24, T_ = 60;

    static Mat<double> ran_st(4 * 4 * 4 * 4 * 4), ran_ed(4 * 4 * 4 * 4 * 4), ran(4 * 4 * 4 * 4 * 4);

    n++;

    if (fg) {
        fg = 0;

        Graphics::PaintSize = 2;
        srand(time(0));
        Matrix::rands(ran_st, -1, 1);
        Matrix::rands(ran_ed, -1, 1);

        {
            int degree = 5;
            vector<int> t(degree);

            coff.zero(pow(4, degree));
            
            for (int i = 0; i < coff.size(); i++) {
                for (int d = 0; d < degree; d++)
                    t[d] = (i >> (2 * (int)pow(4, d))) % 4;

                sort(t.begin(), t.end());
  
                int j = 0;
                for (int d = 0; d < degree; d++) {
                    j *= 4;
                    j += t[d];
                }
                coff[j] += 1;
            }
        }
    }
    {
        if (n % color_T == 0) {
            color_st = color_ed;
            color_ed = (rand() / (double)RAND_MAX) * 0xFFFFFF;
        }
        if (n % T_ == 0) {
            ran_st = ran_ed;
            Matrix::rands(ran_ed, -1, 1);
        }
        {
            double t = (n % color_T) / (double)color_T;
            ARGB c = RGB::Alpha(color_st, color_ed, 1 - t);

            Graphics::PaintColor = 0xFF000000 | c;
        }
        {
            double t = (n % T_) / (double)T_;

            for (int i = 0; i < ran.size(); i++) {
                ran[i] = (ran_st[i] * (1 - t) + ran_ed[i] * t) * (double)coff[i];
            }
        }
           
        Mat<double> Theta(3, 3);
        double t = n * 0.05;

        Theta = {
            1, t, t,
            0, 1, t,
            0, 0, 1
        };
        Matrix::rotate(Theta, ratateMat);
    }
    
    a.zero();
    Z_buf = -0x7FFFFFFF;

    for (int y = 0; y < a.rows; ++y) {
        for (int x = 0; x < a.cols; ++x) {
            int A = 255;
            int r = x * 255 / a.cols;
            int b = y * 255 / a.rows;
            int g = y * 255 / a.rows * 0.8;
            a(y, x) = (A << 24) | (r << 16) | (g << 8) | b;
        }
    }

    vector<thread> threads;
    vector<vector<vector<double>>> Tri(8);

    for (int id = 0; id < 8; id++) {
        threads.emplace_back(work_thread, id, ref(ran), ref(Tri[id]));
    }

    for (auto& t : threads) {
        t.join();
    }

    for (auto& e0 : Tri) {
        for (auto& e : e0) {
            p1 = { (e[0] * 80), (e[1] * 80), (e[2] * 80) };
            p2 = { (e[3] * 80), (e[4] * 80), (e[5] * 80) };
            p3 = { (e[6] * 80), (e[7] * 80), (e[8] * 80) };

            Matrix::mul(p1, ratateMat, p1);
            Matrix::mul(p2, ratateMat, p2);
            Matrix::mul(p3, ratateMat, p3);

            for (int d = 0; d < 2; d++) {
                p1[d] += 400;
                p2[d] += 400;
                p3[d] += 400;
            }

            Graphics::drawTriangle(a, Z_buf, p1, p2, p3);
        }
    }
}

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAM_PI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    HWND hwnd = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 800, 800, NULL, NULL, NULL, NULL);
    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    
    static Mat<unsigned int> buffer(800, 800);
    const UINT_PTR timerId = 1;

    switch (msg) {
    case WM_CREATE: {
        // Create a buffer with ARGB data
        SetTimer(hwnd, timerId, 1, NULL);
        break;
    }
    case WM_DESTROY:
        KillTimer(hwnd, timerId);
        PostQuitMessage(0);
        break;

    case WM_TIMER: {
        if (wp == timerId) {
            work(buffer);
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        BITMAM_PINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAM_PINFOHEADER);
        bmi.bmiHeader.biWidth = buffer.rows;
        bmi.bmiHeader.biHeight = -buffer.rows; // negative height for top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        StretchDIBits(hdc, 0, 0, buffer.rows, buffer.rows, 0, 0, buffer.rows, buffer.rows, buffer.data, &bmi, DIB_RGB_COLORS, SRCCOPY);

        EndPaint(hwnd, &ps);
        break;
    }
    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    return 0;
}
