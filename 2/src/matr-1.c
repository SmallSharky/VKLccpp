/*
 ============================================================================
 Name        : matr-1.c
 Author      : SmallSharky
 Version     :
 Copyright   : GNU GPL
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Что нам понадобится?
 * 1)Функция, читающая матрицу из файла
 * 2)Файл с матрицей
 * 3)Функция "сделать все офигенно"
 * 4)Функция "Прочитать строку из файла, выделить под нее память и запихнуть туда"
 * */

/*
 * Функция "Прочитать строку из файла, выделить под нее память и запихнуть туда"
 * Что принимает:
 * Файл
 * Что возвращает:
 * Строку, память под которую выделяется этой функцией, в случае удачного чтения
 * Нулевой укзазатель в случае неудачи
 * */
char * normal_fgets(FILE * f)
{
	char * retval = 0;
	/*Нам не дали файл*/
	if (!f)
	{
		printf("Error in function \"normal_fgets\":\nFILE NOT SPECIFIED\n");
		return retval;
	}
	/*Узнать длину строки, прочитав ее из файла*/
	//Узнать и запомнить текущую позицию в файле
	fpos_t pos;
	fgetpos(f, &pos);
	//Читать из файла по одному символу до тех пор, пока не дойжем до символа переноса строки или
	//до конца файла. При этом запоминать количество прочитанных символов.
	/*
	 пока( (НЕ конец_файла(f)) И (последний_прочитанный_символ НЕ РАВЕН символу_переноса_строки) ){
	 прочитать_символ();
	 }
	 */
	char c = 0;
	size_t i = 0;
	while ((!feof(f)) && (c != '\n'))
	{
		fread(&c, 1, 1, f);
		i++;
	}
	//В файле вернуться на исходную позицию(см. выше)
	fsetpos(f, &pos);

	/*Выделить под нее память*/
	if (!i)
		i = 1;
	retval = (char *) malloc(i);
	if (!retval)
	{
		printf("Error in function \"normal_fgets\":\nMEMORY NOT ALLOCATED\n");
		return retval;
	}
	/*Записать строку в выделенную память, прочитав ее из файла*/
	fread(retval, i - 1, 1, f);
	char tr;
	fread(&tr, 1, 1, f);
	//Записать символ конца строки
	retval[i - 1] = 0;

	return retval;
}

/*
 * Функция, читающая матрицу.
 *
 * Что принимает:
 * Строку  - имя файла
 * Указатель на целое число - куда записать ширину матрицы
 * Указатель на целое число - куда записать высоту матрицы
 *
 * Что возвращает:
 * Двумерный массив, память под который выделяется динамически этой функцией
 *
 * Как выглядит файл:
 * <ширина> <высота>
 * <M[0][0]> ... <M[i][0]>
 * ...       ...       ...
 * <M[0][j]> ... <M[i][j]>
 * */
int ** freadmatr(const char * fname, int * width, int * height)
{
	int ** retval = 0;
	if ((!fname) || (!width) || (!height))
	{
		printf(
				"Error in function \"freadmatr\":\nFILE NAME OR WIDTH OR HEIGHT NOT SPECIFIED\n");
		return retval;
	}
	FILE * f = fopen(fname, "r");
	if (!f)
	{
		printf("Error in function \"freadmatr\":\nCANNOT OPEN FILE \"%s\"\n",
				fname);
		return retval;
	}
	char * wh_str = normal_fgets(f);
	if (!wh_str)
	{
		printf(
				"Error in function \"freadmatr\":\nCANNOT READ STRING WITH WIDTH AND HEIGHT\n");
		return retval;
	}
	char * w_str = wh_str;
	char * h_str = strchr(wh_str, ' ') + 1;
	if ((h_str == w_str) || (!h_str))
	{
		printf("Error in function \"freadmatr\":\nSYNTAX ERROR IN FILE\n");
		return retval;
	}
	*(h_str - 1) = 0;
	//printf("WIDTH: %s\nHEIGHT:%s\n", w_str, h_str);
	int w, h;
	w = atoi(w_str);
	h = atoi(h_str);
	//printf("WIDTH: %d\nHEIGHT:%d\n", w, h);
	//Освободили память
	free(wh_str);
	wh_str = 0;
	w_str = 0;
	h_str = 0;
	//выделяем память под матрицу
	retval = (int **) malloc(h * sizeof(int *));
	int i = 0;
	while (i < h)
	{
		retval[i] = (int *) malloc(w * sizeof(int));
		i++;
	}
	/*
	 * Читать по одной строке из файла до тех пор, пока не прочитаем нужное
	 * количество строк матрицы или не дойдем до конца файла
	 * */
	i = 0;
	while ((!feof(f)) && (i < h))
	{

		char * ms_str = normal_fgets(f);
		if (!ms_str)
		{
			printf("Error in function \"freadmatr\":\nFILE IO ERROR\n");
			return retval;
		}
		size_t msslen = strlen(ms_str);
		int j = 0;
		char * me_str = ms_str;
		while (j < w)
		{
			//printf("%d-%d\n", i, j);
			//printf("%s\n", me_str);
			//printf("%p\n", me_str);
			if (me_str > (ms_str + msslen))
			{
				j++;
				continue;
			}
			char *nme_str = strchr(me_str, ' ') + 1;
			//printf("%p\n", nme_str);
			if (((nme_str == me_str) && (j != w - 1))
					|| ((!nme_str) && (j != w - 1)))
			{

				printf("Error in function \"freadmatr\":\nERROR IN FILE\n");
				j++;
				continue;

			}
			if (j < (w - 1))
			{
				*(nme_str - 1) = 0;
			}
			int me = atoi(me_str);
			(retval[i])[j] = me;
			j++;
			me_str = nme_str;
		}
		i++;
	}
	*width = w;
	*height = h;
	return retval;
}

/*
 * Выводит матрицу на экран
 * */
void printmatr(int ** M, int w, int h)
{
	int i = 0;
	while (i < h)
	{
		int j = 0;

		while (j < w)
		{
			//i - номер строки, j - номер столбца
			printf("%d ", M[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

/*
 * Функция "сделать все офигенно"
 *
 * Принимает:
 * 1)Матрицу
 * 2)ширину матрицы
 * 3)высоту матрицы
 *
 * Возвращает:
 * ничто
 *
 * */
void do_work(int ** M, int w, int h){
	//Проверяем, есть ли матрица. Если нет - прекращаем работу, сообщив о проблеме.

	/*написать*/


	//Проверяем, является ли матрица квадратной. Если нет - прекращаем работу, сообщив о проблеме.

	/*написать*/

	//Обходим матрицу, вычисляем произведение, находим номер строки и столбца, где лежит мин. элемент

	/*написать*/

	//Записываем туда, где мин. элемент, произведение

	/*написать*/

}

int main(void)
{
	int w, h;
	int ** M = freadmatr("/home/sharky/testmatr.txt", &w, &h);
	if (!M)
		return 10;

	printf("%dx%d\n\n", w, h);
	printmatr(M, w, h);
	do_work(M,w,h);
	printf("\n\n");
	printmatr(M,w,h);

	return 0;
}
