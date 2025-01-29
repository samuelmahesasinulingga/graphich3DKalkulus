#include <iostream>        // Library untuk input-output standar
#include <vector>          // Library untuk menggunakan vektor
#include <string>          // Library untuk manipulasi string
#include <map>             // Library untuk struktur data map
#include <windows.h>       // Library untuk menangani GUI Windows
#include "matplotlibcpp.h" // Library untuk plotting menggunakan Matplotlib

using namespace std;           // Menggunakan namespace standar C++
namespace plt = matplotlibcpp; // Alias untuk namespace matplotlibcpp

// Variabel global untuk parameter bidang
// a, b, c adalah koefisien bidang dan d adalah konstanta
// t0 dan t1 adalah batas bawah dan atas nilai variabel x dan y
double a = 1.0, b = 1.0, c = 1.0, d = 4.0;
double t0 = -3.0, t1 = 3.0;

// Deklarasi GUI handles untuk elemen input GUI
HWND hwndA, hwndB, hwndC, hwndD, hwndT0, hwndT1;

// Fungsi untuk membaca input dari GUI dan menyimpannya dalam variabel global
void ReadInputFromGUI()
{
    char buffer[256];                             // Buffer untuk menyimpan teks input
    GetWindowText(hwndA, buffer, sizeof(buffer)); // Membaca teks dari input a
    a = stod(buffer);                             // Mengubah teks menjadi angka

    GetWindowText(hwndB, buffer, sizeof(buffer)); // Membaca teks dari input b
    b = stod(buffer);                             // Mengubah teks menjadi angka

    GetWindowText(hwndC, buffer, sizeof(buffer)); // Membaca teks dari input c
    c = stod(buffer);                             // Mengubah teks menjadi angka

    GetWindowText(hwndD, buffer, sizeof(buffer)); // Membaca teks dari input d
    d = stod(buffer);                             // Mengubah teks menjadi angka

    GetWindowText(hwndT0, buffer, sizeof(buffer)); // Membaca teks dari input t0
    t0 = stod(buffer);                             // Mengubah teks menjadi angka

    GetWindowText(hwndT1, buffer, sizeof(buffer)); // Membaca teks dari input t1
    t1 = stod(buffer);                             // Mengubah teks menjadi angka
}

// Fungsi untuk menggambar grafik 3D bidang
void PlotGraph()
{
    int n = 10;                        // Jumlah titik dalam setiap sumbu
    double step = (t1 - t0) / (n - 1); // Jarak antar titik

    vector<vector<double>> x, y, z; // Matriks untuk menyimpan koordinat

    // Loop untuk menghitung nilai x, y, dan z
    for (int i = 0; i < n; ++i)
    {
        vector<double> row_x, row_y, row_z;
        double x_val = t0 + i * step;

        for (int j = 0; j < n; ++j)
        {
            double y_val = t0 + j * step;
            double z_val = (d - a * x_val - b * y_val) / c; // Menghitung nilai z berdasarkan persamaan bidang

            row_x.push_back(x_val);
            row_y.push_back(y_val);
            row_z.push_back(z_val);
        }

        x.push_back(row_x);
        y.push_back(row_y);
        z.push_back(row_z);
    }

    PyObject *ax = plt::chart(111);           // Membuat objek grafik 3D
    plt::Clear3DChart(ax);                    // Membersihkan grafik sebelum menampilkan yang baru
    plt::surface3D(ax, x, y, z, "blue", 0.9); // Menampilkan bidang dalam warna biru
    plt::axis("equal");                       // Menyesuaikan sumbu agar proporsional
    plt::show();                              // Menampilkan grafik
}

// Fungsi utama untuk menangani event pada jendela GUI
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        // Membuat label dan input untuk parameter bidang
        CreateWindow("STATIC", "a:", WS_VISIBLE | WS_CHILD, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
        hwndA = CreateWindow("EDIT", "1.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 10, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "b:", WS_VISIBLE | WS_CHILD, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
        hwndB = CreateWindow("EDIT", "1.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 40, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "c:", WS_VISIBLE | WS_CHILD, 10, 70, 50, 20, hwnd, NULL, NULL, NULL);
        hwndC = CreateWindow("EDIT", "1.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 70, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "d:", WS_VISIBLE | WS_CHILD, 10, 100, 50, 20, hwnd, NULL, NULL, NULL);
        hwndD = CreateWindow("EDIT", "4.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 100, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "t0:", WS_VISIBLE | WS_CHILD, 10, 130, 50, 20, hwnd, NULL, NULL, NULL);
        hwndT0 = CreateWindow("EDIT", "-3.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 130, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "t1:", WS_VISIBLE | WS_CHILD, 10, 160, 50, 20, hwnd, NULL, NULL, NULL);
        hwndT1 = CreateWindow("EDIT", "3.0", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 160, 100, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("BUTTON", "Submit", WS_VISIBLE | WS_CHILD, 10, 190, 80, 30, hwnd, (HMENU)1, NULL, NULL); // Tombol untuk memulai proses
        break;

    case WM_COMMAND:
        if (LOWORD(wp) == 1) // Jika tombol Submit ditekan
        {
            ReadInputFromGUI(); // Membaca input dari GUI
            PlotGraph();        // Menampilkan grafik berdasarkan input
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0); // Menutup program saat jendela ditutup
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL); // Mendapatkan instance aplikasi
    WNDCLASS wc = {};                            // Struktur kelas jendela
    wc.lpfnWndProc = WindowProcedure;            // Menentukan fungsi prosedur jendela
    wc.hInstance = hInstance;
    wc.lpszClassName = "Graph3DGUI";

    RegisterClass(&wc); // Mendaftarkan kelas jendela

    // Membuat jendela utama aplikasi
    HWND hwnd = CreateWindowEx(0, "Graph3DGUI", "3D Graph Plotter", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 400, 280, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, SW_SHOW); // Menampilkan jendela
    UpdateWindow(hwnd);        // Memperbarui tampilan jendela

    MSG msg = {};                        // Struktur pesan Windows
    while (GetMessage(&msg, NULL, 0, 0)) // Loop utama aplikasi
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0; // Keluar dari program
}
