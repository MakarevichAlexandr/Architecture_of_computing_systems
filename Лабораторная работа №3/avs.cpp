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
	    cout << "при шаге h = " << h << " I(h) = " << value << fixed << setprecision(8) << endl;
	    cout << "---------------------------------------------------------------" << endl;
	     h = h/2;
	     counter++;
	}
	while (counter < 2);
	if (fabs(value - value1) <= 16*eps) {
		cout << "Формула Симпсона: " << fixed << setprecision(6) << value << "\t" << " при шаге h : " << h << endl;
		cout << "---------------------------------------------------------------" << endl;
	}
	else Simpson(a, b, h/2, eps);
}

 
int main()
{
    clock_t c0, c1;
    DWORD   tt0, tt1;
 
    // Средства PerformanceCounter 
    LARGE_INTEGER pct0, pct1, freq;  
    QueryPerformanceFrequency(&freq); // Определяем частоту таймера
    HANDLE hThread = GetCurrentThread(); // Устанавливаем выполнение текущего потока на первом ядре процессора
    DWORD_PTR prevMask = SetThreadAffinityMask(hThread, 1 ); // Завершаем квант процессорного времени для гарантированного переключения на задний процессор
    Sleep(0); 
    
    long long frequency; 
    if (QueryPerformanceFrequency((LARGE_INTEGER *)&frequency)) 
    cout << frequency << " Hz; " << endl;
    else cout << "FAIL" << endl;

    setlocale(LC_ALL, "Russian");
	double h, value = 0, value1 = 0, eps; 
	int i, numberpoints;
	double a, b;
	cout << " Левая граница интегрирования: a = "; 
	cin >> a;
	cout << "-------------------------------------" << endl;
	cout << " Правая граница интегрирования: b = ";
	cin >> b;
	cout << "-------------------------------------" << endl;
	double *f;
	cout << " Величина шага: h = ";
	cin >> h;
	cout << "-------------------------------------" << endl;
	cout << " Количество точек n: ";
	cin >> numberpoints;
	cout << "-------------------------------------" << endl;
	cout << " Введите заданную точность eps : ";
	cin >> eps; 
	cout << "-------------------------------------" << endl;
	
	// Измеряем время начала отсчёта 4 способами
    c0 = clock();                     // Способ 1
    tt0 = GetTickCount();             // Способ 2 (Только MS Windows)
    QueryPerformanceCounter(&pct0);   // Способ 3 (Только MS Windows)
    
    __int64 StartTscValue, EndTscValue;  //Способ 4 (регистр счётчика TSC)
	// Начальное значение счётчика  
	__asm   
	{     
		RDTSC     
		MOV   DWORD PTR [StartTscValue], EAX     
		MOV   DWORD PTR [StartTscValue+4], EDX   
	}

	double x;
	f = (double*)malloc(numberpoints * sizeof(double));
	x = (b-a);
	//Инициализация значений нашей функции 1/x
	for (i = 0; i < numberpoints; i++) {
		f[i] = 1/x;
		cout << " X" << i << " = " << x << fixed << setprecision(5) << "\t" << "Y" << i << " = " << f[i] << "\n";
		cout << "-------------------------------------" << endl;
		x = x + h;
	}
	cout << "\n";
    Simpson(a, b, h, eps);
 
    // Измеряем время окончания 4-мя способами
    c1 = clock();
    tt1 = GetTickCount();
    QueryPerformanceCounter(&pct1);
    
    // Конечное значение счетчика  
	__asm   
	{     
		RDTSC     
		MOV   DWORD PTR [EndTscValue], EAX     
		MOV   DWORD PTR [EndTscValue+4], EDX   
	}
 
 
    // Разрешаем выполнение потока на всех доступных процессорах
    SetThreadAffinityMask(hThread, prevMask ); 
 
    // Выводим результат измерения
    printf ("\t Время измерено в секундах\n");
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t Затраченное CPU time:        %f с\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t Затраченное GetTickCount time: %f c\n", (tt1 - tt0)*0.001);
    cout << "         --------------------------------------------------------" << endl;
    printf ("\t Затраченное PerformanceCounter time:        %f c\n", (unsigned int) ( ((double)pct1.QuadPart /(double) freq.QuadPart  - (double)pct0.QuadPart / ( double) freq.QuadPart) * 1000)*0.001);
    cout << "         --------------------------------------------------------" << endl;
    // Время выполнения  
	std::cout << "         Затраченное time TSC: " <<     ((EndTscValue - StartTscValue) / frequency)*0.001<< " c\n";   
	std::getchar(); 
  return 0;
}
