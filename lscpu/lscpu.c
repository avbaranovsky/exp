#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

static int	count_current_cpu_cores(void);
static int
count_current_cpu_cores(void)
{
	int			socket = 0,
				core = 0,
				cores = 0,
				i = 0;
	char		buffer[1024],
				param[1024],
				value[1024],
			   *buffer_pos,
			   *token_pos,
				ch;
	FILE	   *f = fopen("/proc/cpuinfo", "rt");

	if (f == NULL)
		return -1;

	while (fgets(buffer, sizeof(buffer) - 1, f))
	{
		memset(param, 0, sizeof(param));
		memset(value, 0, sizeof(value));
		buffer_pos = buffer;
		token_pos = param;
		while ((ch = *buffer_pos++))
		{
			if (ch == ':')
			{
				token_pos = value;
			}
			else if (isalnum(ch))
				*(token_pos++) = ch;
		}
		if (param[0] == 0 || value[0] == 0)
			continue;

		i = atoi(value);
		if (strcmp(param, "physicalid") == 0)
		{
			if (i > socket)
			{
				socket = i;
				cores += core;
				core = 0;
			}
		}
		else if (strcmp(param, "coreid") == 0)
		{
			core++;
			printf("socket: %d, core: %d", socket, core);
		}
	}
	fclose(f);
	cores += core;
	return cores;
}


int main() {
    int cores = count_current_cpu_cores();
    printf("Total cores counted: %d\n", cores);
    return 0;
}