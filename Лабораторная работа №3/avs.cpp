#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <ctime>

using namespace std;

double Function(double x) {
  return 1/x;
}

void Simpson(double a, double b, double h, double eps) {
	int counter = 0;
	double i;
	double value = 0, value1 = 0;
	do {
		value1 = value;
		value = Function(a) + Function(b);
		for (i = a + h; i < b - h; i = i + 2*h) {
		    value = value + 4*Function(i);
    	}
	    for (i = a + h*2; i <= b - h; i = i + 2*h) {
	    	value = value + 2*Function(i);
	    }
	    value = h/3 * value;
	    cout << "��� ���� h = " << h << " I(h) = " << value << fixed << setprecision(8) << endl;
	    cout << "---------------------------------------------------------------" << endl;
	     h = h/2;
	     counter++;
	}
	while (counter < 2);
	if (fabs(value - value1) <= 16*eps) {
		cout << "������� ��������: " << fixed << setprecision(6) << value << "\t" << " ��� ���� h : " << h << endl;
		cout << "---------------------------------------------------------------" << endl;
	}
	else Simpson(a, b, h/2, eps);
}

 
int main()
{
    clock_t c0, c1;
    DWORD   tt0, tt1;
 
    // �������� PerformanceCounter 
    LARGE_INTEGER pct0, pct1, freq;  
    QueryPerformanceFrequency(&freq); // ���������� ������� �������
    HANDLE hThread = GetCurrentThread(); // ������������� ���������� �������� ������ �� ������ ���� ����������
    DWORD_PTR prevMask = SetThreadAffinityMask(hThread, 1 ); // ��������� ����� ������������� ������� ��� ���������������� ������������ �� ������ ���������
    Sleep(0); 
    
    long long frequency; 
    if (QueryPerformanceFrequency((LARGE_INTEGER *)&frequency)) 
    cout << frequency << " Hz; " << endl;
    else cout << "FAIL" << endl;

    setlocale(LC_ALL, "Russian");
	double h, value = 0, value1 = 0, eps; 
	int i, numberpoints;
	double a, b;
	cout << " ����� ������� ��������������: a = "; 
	cin >> a;
	cout << "-------------------------------------" << endl;
	cout << " ������ ������� ��������������: b = ";
	cin >> b;
	cout << "-------------------------------------" << endl;
	double *f;
	cout << " �������� ����: h = ";
	cin >> h;
	cout << "-------------------------------------" << endl;
	cout << " ���������� ����� n: ";
	cin >> numberpoints;
	cout << "-------------------------------------" << endl;
	cout << " ������� �������� �������� eps : ";
	cin >> eps; 
	cout << "-------------------------------------" << endl;
	
	// �������� ����� ������ ������� 4 ���������
    c0 = clock();                     // ������ 1
    tt0 = GetTickCount();             // ������ 2 (������ MS Windows)
    QueryPerformanceCounter(&pct0);   // ������ 3 (������ MS Windows)
    
    __int64 StartTscValue, EndTscValue;  //������ 4 (������� �������� TSC)
	// ��������� �������� ��������  
	__asm   
	{     
		RDTSC     
		MOV   DWORD PTR [StartTscValue], EAX     
		MOV   DWORD PTR [StartTscValue+4], EDX   
	}

	double x;
	f = (double*)malloc(numberpoints * sizeof(double));
	x = (b-a);
	//������������� �������� ����� ������� 1/x
	for (i = 0; i < numberpoints; i++) {
		f[i] = 1/x;
		cout << " X" << i << " = " << x << fixed << setprecision(5) << "\t" << "Y" << i << " = " << f[i] << "\n";
		cout << "-------------------------------------" << endl;
		x = x + h;
	}
	cout << "\n";
    Simpson(a, b, h, eps);
 
    // �������� ����� ��������� 4-�� ���������
    c1 = clock();
    tt1 = GetTickCount();
    QueryPerformanceCounter(&pct1);
    
    // �������� �������� ��������  
	__asm   
	{     
		RDTSC     
		MOV   DWORD PTR [EndTscValue], EAX     
		MOV   DWORD PTR [EndTscValue+4], EDX   
	}
 
 
    // ��������� ���������� ������ �� ���� ��������� �����������
    SetThreadAffinityMask(hThread, prevMask ); 
 
    // ������� ��������� ���������
    printf ("\t ����� �������� � ��������\n");
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t ����������� CPU time:        %f �\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t ����������� GetTickCount time: %f c\n", (tt1 - tt0)*0.001);
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t ����������� PerformanceCounter time:        %f c\n", (unsigned int) ( ((double)pct1.QuadPart /(double) freq.QuadPart  - (double)pct0.QuadPart / ( double) freq.QuadPart) * 1000)*0.001);
    cout << "         --------------------------------------------------------" << endl;
    // ����� ����������  
	std::cout << "         ����������� time TSC: " <<     ((EndTscValue - StartTscValue) / frequency)*0.001<< " c\n";   
	std::getchar(); 
  return 0;
}
