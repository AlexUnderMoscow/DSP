#pragma once
#include "struct.h"

#define M 32

float filterSSE( float* _h, float* _y, unsigned int elNumber)
{

	unsigned int cnt;								//���������� ����������
	__declspec(align(16)) float array1[M*20];		//����������� ������
	__declspec(align(16)) float array2[M*20];		//��� ����� ��
	__declspec(align(16)) float resSSE;				//��� ��������� �����
	resSSE = 0;										//�������� ���������

	cnt = elNumber >> 5;							//������� �� 32
	cnt++;
	memset(array1+341,0,64);
	memcpy(array1,_y,elNumber*sizeof(float));
	memset(array2+341,0,64);
	memcpy(array2,_h,elNumber*sizeof(float));

	__asm {
		LEA esi, [array1]							//� ������� ��������� ����� ������� �������� �������
		LEA edi, [array2]							//� ������ ..������� �������
		LEA edx, [resSSE]							//� ���� ������� ����� ������ � ����������� (��� ������ 0)
		MOV ecx, cnt								//������� ��� �� 128 ���� ������������   (32 ����� float) ���-�� ����� �����
			align 16								//���������� �����
mloop:
		MOVAPS xmm0, XMMWORD PTR [esi]				//� ������� ������� SSE ���������� �� ������ ESI (������ P - ����� 4 float)
		MOVAPS xmm1, XMMWORD PTR [esi+16]			//� ������ ���� 4 ����� ����������� 4 float *4 =16 ���� ��������
		MOVAPS xmm2, XMMWORD PTR [esi+32]			//�.�.�.
		MOVAPS xmm3, XMMWORD PTR [esi+48]
		MOVAPS xmm4, XMMWORD PTR [esi+64]
		MOVAPS xmm5, XMMWORD PTR [esi+80]
		MOVAPS xmm6, XMMWORD PTR [esi+96]
		MOVAPS xmm7, XMMWORD PTR [esi+112]			//������ 32 �������� ������� �������� �� ��� ��������

		MULPS xmm0, XMMWORD PTR[edi]
		MULPS xmm1, XMMWORD PTR[edi+16]
		MULPS xmm2, XMMWORD PTR[edi+32]
		MULPS xmm3, XMMWORD PTR[edi+48]
		MULPS xmm4, XMMWORD PTR[edi+64]
		MULPS xmm5, XMMWORD PTR[edi+80]
		MULPS xmm6, XMMWORD PTR[edi+96]
		MULPS xmm7, XMMWORD PTR[edi+112]		//�������� ����������� �� ������ ������ �� ������ EDI (������� P - ��������)

		ADDPS xmm0,xmm1
			ADDPS xmm0,xmm2
			ADDPS xmm0,xmm3
			ADDPS xmm0,xmm4
			ADDPS xmm0,xmm5
			ADDPS xmm0,xmm6
			ADDPS xmm0,xmm7							//���������� �������� �� ����� ������������  - ��� � xmm0 ����� ���������

			MOVAPS XMMWORD PTR[esi],xmm0			//��������� �������� (4 float) ������� � ������ ������

			movss xmm0,WORD PTR[ESI]				//����������� ������� ��� float 
		addss xmm0,WORD PTR[ESI+4]				//��������� ����� �����
		addss xmm0,WORD PTR[ESI+8]				// �������� P ���, ������ �����������
		addss xmm0,WORD PTR[ESI+12]				//16 ���� = 4 float

		addss xmm0, WORD PTR[EDX]				//���������� ������������� ��������� � ��� ��� �������� �� ������ ����������
		movss WORD PTR[EDX],xmm0				//����� ����� ������ �� ����� ���������� �� � ������� � � ������ ��� � ��������

			ADD esi, 128							//������� ��������� �� ������� �� 128 ����
			ADD edi, 128							//��� �� ����� 32 float, ������� ��� ���������

			LOOP mloop								//����������� ���� ECX ������ �� ���������, ��� �����, ��� ���������� 625 ��������
	}
	return resSSE;
}