#include "lab3.h"
#include <windows.h>
#include <iostream>

#define THREAD_COUNT 11
#define SEMAPHORE_COUNT 4

using namespace std;

DWORD ThreadID;
HANDLE hThread[THREAD_COUNT];
HANDLE hSemaphore[SEMAPHORE_COUNT], GO, STOP;
HANDLE hMutex;

DWORD WINAPI thread_a(LPVOID);
DWORD WINAPI thread_b(LPVOID);
DWORD WINAPI thread_c(LPVOID);
DWORD WINAPI thread_d(LPVOID);
DWORD WINAPI thread_e(LPVOID);
DWORD WINAPI thread_f(LPVOID);
DWORD WINAPI thread_g(LPVOID);
DWORD WINAPI thread_i(LPVOID);
DWORD WINAPI thread_k(LPVOID);
DWORD WINAPI thread_m(LPVOID);
DWORD WINAPI thread_n(LPVOID);

unsigned int lab3_thread_graph_id()
{
	return 7;
}

const char* lab3_unsynchronized_threads()
{
	return "dfgi";
}

const char* lab3_sequential_threads()
{
	return "bcdg";
}

DWORD WINAPI thread_a(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "a" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}

DWORD WINAPI thread_b(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hSemaphore[0], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		cout << "b" << flush;
		computation();
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphore[1], 1, NULL);
	}
	return 0;
}
DWORD WINAPI thread_c(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hSemaphore[1], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		cout << "c" << flush;
		computation();
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphore[2], 1, NULL);
	}
	ReleaseSemaphore(STOP, 1, NULL);
	WaitForSingleObject(GO, INFINITE);
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "c" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI thread_d(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hSemaphore[2], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		cout << "d" << flush;
		computation();
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphore[3], 1, NULL);
	}
	ReleaseSemaphore(STOP, 1, NULL);
	WaitForSingleObject(GO, INFINITE);
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "d" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	ReleaseSemaphore(STOP, 1, NULL);
	WaitForSingleObject(GO, INFINITE);
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "d" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}

DWORD WINAPI thread_e(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "e" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}

DWORD WINAPI thread_f(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "f" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI thread_g(LPVOID lpParam)
{
	//T0-T1
	//��������� ����� A
	hThread[0] = CreateThread(NULL, 0, thread_a, NULL, 0, &ThreadID);
	if (hThread[0] == NULL) {
		return GetLastError();
	}
	//������� A
	WaitForSingleObject(hThread[0], INFINITE);

	//T1-T2
	//��������� ����� B
	hThread[1] = CreateThread(NULL, 0, thread_b, NULL, 0, &ThreadID);
	if (hThread[1] == NULL) {
		return GetLastError();
	}
	//��������� ����� �
	hThread[2] = CreateThread(NULL, 0, thread_c, NULL, 0, &ThreadID);
	if (hThread[2] == NULL) {
		return GetLastError();
	}
	//��������� ����� D
	hThread[3] = CreateThread(NULL, 0, thread_d, NULL, 0, &ThreadID);
	if (hThread[3] == NULL) {
		return GetLastError();
	}
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hSemaphore[3], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		cout << "g" << flush;
		computation();
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphore[0], 1, NULL);
	}
	//������� B
	WaitForSingleObject(hThread[1], INFINITE);
	WaitForSingleObject(STOP, INFINITE);
	WaitForSingleObject(STOP, INFINITE);

	//T2-T3
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "g" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	ReleaseSemaphore(GO, 1, NULL);
	ReleaseSemaphore(GO, 1, NULL);
	//��������� ����� E
	hThread[5] = CreateThread(NULL, 0, thread_e, NULL, 0, &ThreadID);
	if (hThread[5] == NULL) {
		return GetLastError();
	}
	//������� E
	WaitForSingleObject(hThread[5], INFINITE);
	WaitForSingleObject(STOP, INFINITE);

	//T3-T4
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "g" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	//��������� ����� I
	hThread[7] = CreateThread(NULL, 0, thread_i, NULL, 0, &ThreadID);
	if (hThread[7] == NULL) {
		return GetLastError();
	}
	//��������� ����� F
	hThread[6] = CreateThread(NULL, 0, thread_f, NULL, 0, &ThreadID);
	if (hThread[6] == NULL) {
		return GetLastError();
	}
	ReleaseSemaphore(GO, 1, NULL);
	//������� D
	WaitForSingleObject(hThread[3], INFINITE);
	//������� F
	WaitForSingleObject(hThread[6], INFINITE);
	WaitForSingleObject(STOP, INFINITE);

	//T4-T5
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "g" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	//��������� ����� K
	hThread[8] = CreateThread(NULL, 0, thread_k, NULL, 0, &ThreadID);
	if (hThread[8] == NULL) {
		return GetLastError();
	}
	//��������� ����� M
	hThread[9] = CreateThread(NULL, 0, thread_m, NULL, 0, &ThreadID);
	if (hThread[9] == NULL) {
		return GetLastError();
	}
	ReleaseSemaphore(GO, 1, NULL);
	//������� K
	WaitForSingleObject(hThread[7], INFINITE);
	//������� I
	WaitForSingleObject(hThread[8], INFINITE);
	WaitForSingleObject(STOP, INFINITE);
	
	//T5-T6
	//��������� ����� N
	hThread[10] = CreateThread(NULL, 0, thread_n, NULL, 0, &ThreadID);
	if (hThread[10] == NULL) {
		return GetLastError();
	}
	ReleaseSemaphore(GO, 1, NULL);
	//������� N
	WaitForSingleObject(hThread[10], INFINITE);
	//������� M
	WaitForSingleObject(hThread[9], INFINITE);
	return 0;
}

DWORD WINAPI thread_i(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "i" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	ReleaseSemaphore(STOP, 1, NULL);
	WaitForSingleObject(GO, INFINITE);
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "i" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI thread_k(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "k" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI thread_m(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "m" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	ReleaseSemaphore(STOP, 1, NULL);
	WaitForSingleObject(GO, INFINITE);
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "m" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI thread_n(LPVOID lpParam)
{
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "n" << flush;
		computation();
		ReleaseMutex(hMutex);
	}
	return 0;
}
int lab3_init()
{
	hMutex = CreateMutex(NULL, 0, NULL);
	if (hMutex == NULL) {
		cout << "CreateMutex error: " << GetLastError();
		return 1;
	}

	for (int i = 0; i < SEMAPHORE_COUNT; i++) {
		hSemaphore[i] = CreateSemaphore(NULL, i == 0 ? 1 : 0, 1, NULL);
		if (hSemaphore[i] == NULL) {
			cout << "CreateSemaphore error: " << GetLastError();
			return 1;
		}
	}
	STOP = CreateSemaphore(NULL, 0, 2, NULL);
	GO = CreateSemaphore(NULL, 0, 2, NULL);

	hThread[4] = CreateThread(NULL, 0, thread_g, NULL, 0, &ThreadID);
	if (hThread[4] == NULL) {
		return GetLastError();
	}
	//������� A
	WaitForSingleObject(hThread[4], INFINITE);
	//��������� �����
// close A
	for (int i = 0; i < THREAD_COUNT; ++i)
		CloseHandle(hThread[i]);
	CloseHandle(hMutex);

	for (int i = 0; i < SEMAPHORE_COUNT; i++) {
		CloseHandle(hSemaphore[i]);
	}
	return 0;
}
