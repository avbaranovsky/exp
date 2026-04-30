#include <stdlib.h>
#include <stdio.h>

#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* Получаем количество ядер в системе */
/* Структура для хранения информации о ядрах */
typedef struct 
{
    int physical_id;  // ID физического процессора (сокета)
    int core_id;      // ID ядра внутри процессора
} CoreInfo;

/* Функция сравнения для qsort(), сортируем сначала по physical_id, затем по core_id */
static int compare_cores(const void *a, const void *b) 
{
    CoreInfo *coreA = (CoreInfo *)a;
    CoreInfo *coreB = (CoreInfo *)b;
    if (coreA->physical_id != coreB->physical_id)
        return coreA->physical_id - coreB->physical_id;
    return coreA->core_id - coreB->core_id;
}

static int 
get_sys_cores_count(char *info_file_name)
{
	char buffer[1024];
	int  physical_id = -1,
		 core_id = -1,
		 unique_cores = 1,
		 cores_count = 0,
		 max_cores = 128;
	FILE *f = NULL;
	CoreInfo *cores = NULL, *tmp = NULL;

	if ((f = fopen(info_file_name, "r")) == NULL)
		return -1;  // Ошибка открытия файла
    
    /* Динамический массив для хранения (physical_id, core_id) */
	if ((cores = malloc(max_cores * sizeof(CoreInfo))) == NULL) 
	{
		fclose(f);
		return -1;  // Ошибка выделения памяти
	}

    /* Читаем /proc/cpuinfo построчно */
	while (fgets(buffer, sizeof(buffer), f)) 
	{
		/* Ищем строки "physical id" и "core id" */
		if (sscanf(buffer, "physical id : %d", &physical_id) == 1)
			continue;
		if (sscanf(buffer, "core id : %d", &core_id) == 1) 
		{
			if (physical_id == -1)  // Убеждаемся, что physical_id найден перед core_id
			continue;

			/* Если массив переполнен, увеличиваем его в 2 раза */
			if (cores_count >= max_cores)
			{
				max_cores *= 2;
				tmp = cores;
				if ((cores = realloc(cores, max_cores * sizeof(CoreInfo))) == NULL) 
				{
					free(tmp);
					fclose(f);
					return -1;  // Ошибка выделения памяти
				}
			}
			/* Записываем пару (physical_id, core_id) */
			cores[cores_count].physical_id = physical_id;
			cores[cores_count].core_id = core_id;
			cores_count++;
		}
    }
    fclose(f);  // Закрываем файл

    /* Если не найдено ни одного ядра, освобождаем память и возвращаем ошибку */
	if (cores_count == 0) 
	{
		free(cores);
		return -1;
	}

	/* Сортируем массив (physical_id, core_id) */
	qsort(cores, cores_count, sizeof(CoreInfo), compare_cores);

	/* Подсчитываем количество уникальных (physical_id, core_id) */
	for (int i = 1; i < cores_count; i++) 
	{
		if (cores[i].physical_id != cores[i - 1].physical_id ||
			cores[i].core_id != cores[i - 1].core_id)
		unique_cores++;
	}

	free(cores);  // Освобождаем память
	return unique_cores;  // Возвращаем количество уникальных ядер
}

int main(int argc, char *argv[]) {
	char *info_file=NULL;
	if(argc >1)
		info_file=argv[1];
	
	printf("%d cores detected\n", get_sys_cores_count(info_file ? info_file : "/proc/cpuinfo"));
	return 0;
}