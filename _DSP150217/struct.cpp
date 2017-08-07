#pragma once
#include "struct.h"

#define M 32

float filterSSE( float* _h, float* _y, unsigned int elNumber)
{

	unsigned int cnt;								//количество повторений
	__declspec(align(16)) float array1[M*20];		//выровненный массив
	__declspec(align(16)) float array2[M*20];		//еще такой же
	__declspec(align(16)) float resSSE;				//тут результат будет
	resSSE = 0;										//обнулили результат

	cnt = elNumber >> 5;							//деление на 32
	cnt++;
	memset(array1+341,0,64);
	memcpy(array1,_y,elNumber*sizeof(float));
	memset(array2+341,0,64);
	memcpy(array2,_h,elNumber*sizeof(float));

	__asm {
		LEA esi, [array1]							//в регистр загружаем адрес первого элемента массива
		LEA edi, [array2]							//в другой ..второго массива
		LEA edx, [resSSE]							//в этот регистр адрес €чейки с результатом (там сейчас 0)
		MOV ecx, cnt								//сколько раз по 128 байт обрабатывать   (32 числа float) кол-во шагов цикла
			align 16								//загадочное нечто
mloop:
		MOVAPS xmm0, XMMWORD PTR [esi]				//в нулевой регистр SSE содержимое по адресу ESI (суфикс P - сразу 4 float)
		MOVAPS xmm1, XMMWORD PTR [esi+16]			//в первый тоже 4 числа параллельно 4 float *4 =16 байт смещени€
		MOVAPS xmm2, XMMWORD PTR [esi+32]			//и.т.д.
		MOVAPS xmm3, XMMWORD PTR [esi+48]
		MOVAPS xmm4, XMMWORD PTR [esi+64]
		MOVAPS xmm5, XMMWORD PTR [esi+80]
		MOVAPS xmm6, XMMWORD PTR [esi+96]
		MOVAPS xmm7, XMMWORD PTR [esi+112]			//теперь 32 элемента массива занесены во все регистры

		MULPS xmm0, XMMWORD PTR[edi]
		MULPS xmm1, XMMWORD PTR[edi+16]
		MULPS xmm2, XMMWORD PTR[edi+32]
		MULPS xmm3, XMMWORD PTR[edi+48]
		MULPS xmm4, XMMWORD PTR[edi+64]
		MULPS xmm5, XMMWORD PTR[edi+80]
		MULPS xmm6, XMMWORD PTR[edi+96]
		MULPS xmm7, XMMWORD PTR[edi+112]		//умножаем поэлементно на второй массив по адресу EDI (суффикс P - векторно)

		ADDPS xmm0,xmm1
			ADDPS xmm0,xmm2
			ADDPS xmm0,xmm3
			ADDPS xmm0,xmm4
			ADDPS xmm0,xmm5
			ADDPS xmm0,xmm6
			ADDPS xmm0,xmm7							//складываем векторно со всеми результатами  - все в xmm0 лежит упаковано

			MOVAPS XMMWORD PTR[esi],xmm0			//выбросили упаковку (4 float) обратно в первый массив

			movss xmm0,WORD PTR[ESI]				//поэлементно достаем все float 
		addss xmm0,WORD PTR[ESI+4]				//суммируем между собой
		addss xmm0,WORD PTR[ESI+8]				// суффикса P нет, значит поэлементно
		addss xmm0,WORD PTR[ESI+12]				//16 байт = 4 float

		addss xmm0, WORD PTR[EDX]				//складываем промежуточный результат с тем что хранитс€ по адресу результата
		movss WORD PTR[EDX],xmm0				//общую сумму кладем на место результата не в регистр а в пам€ть где и положено

			ADD esi, 128							//смещаем указатели на массивы на 128 байт
			ADD edi, 128							//это те самые 32 float, которые уже посчитаны

			LOOP mloop								//зацикливаем пока ECX видимо не обнулитс€, так думаю, там изначально 625 итераций
	}
	return resSSE;
}