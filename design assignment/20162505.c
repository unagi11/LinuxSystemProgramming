#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>

#define MATRIX_ROW_SIZE_LIMIT 10010
#define SIZE_OF_BUFFER 20010
#define DEFAULT_INPUT_FILENAME "input.matrix"
#define DEFAULT_OUTPUT_FILENAME "output.matrix"
#define CLEAR_SYSTEM_COMMAND "clear"

// matrix 가로 세로 길이
int width = 0;
int height = 0;

// 현재 매트릭스 2차원 배열
char** matrix;
char** oldMatrix;

// 생성할 세대의 개수
int numOfGeneration;

// 버퍼
char buffer[SIZE_OF_BUFFER];
char fileName[32];

// 시간측정용
struct timeval start_time, end_time;
long proc_sec, proc_usec;

// 병렬 처리용 변수
int numOfDivide;
int *divideOffset;
int *divideLength;

// 옵션용 플래그
bool flag_c = false;
bool flag_t = false;

void AllocMatrix(int w, int h);
void FreeMatrix();
void PrintMatrix();
void ReadMatrix(const char* fileName);
void WriteMatrix(const char* fileName);
void WriteDividedMatrix(const char* fileName, int offset, int length);
void RandMatrix(int w, int h);
char LifeGameAlgo(char** matrix, int x, int y);
void InitDivideWork(int numOfDivide);
void *LifeGameAlgoForThread(void* arg);
void PrintProcessingTime();

int main(int argc, char* const* argv)
{
	ReadMatrix(DEFAULT_INPUT_FILENAME);

	int c; // option
	while( (c = getopt(argc, argv, "ct")) != -1) {
		switch(c) {
			case 'c':
				flag_c = true;
				break;
			case 't':
				flag_t = true;
				break;
			case '?':
				break;				
		}
	}

	if (flag_c == false)
		system(CLEAR_SYSTEM_COMMAND);

	while(1) 
	{
		if (flag_t == false)
		{
			printf("[Cell Matrix Game] Select Command\n");
			printf("(1) Exit program\n");
			printf("(2) Sequential Processing (Single Thread)\n");
			printf("(3) Multi-process Parallel Processing\n");
			printf("(4) Multi-thread Parallel Processing\n");
			printf("(5) Random Generate input.matrix\n");
		}

		char input = getchar();

		if (flag_c == false)
			system(CLEAR_SYSTEM_COMMAND);

		while(getchar() != '\n'); // 버퍼를 비웁니다.

		if (flag_t == false)
			printf("You Inputed \"%c\"\n", input);

		if (input == '1') 
		{
			if (flag_t == false)
				printf("Selected (1) Exit Program\n");

			exit(1);			
		} 
		else if (input == '2') 
		{
			if (flag_t == false)
			{
				printf("Selected (2) Sequential Processing (Single Thread)\n");
				printf("How many generation do you want? ");
			}

			scanf("%d", &numOfGeneration);

			gettimeofday(&start_time, NULL);

			// 동적 할당
			oldMatrix = (char**) malloc( sizeof(char*) * height );
			oldMatrix[0] = (char*) malloc( sizeof(char) * width * height );
			for (int i = 1; i < height; ++i)
				oldMatrix[i] = oldMatrix[i-1] + width;

			// 세대 진행 반복
			for (int n = 1; n <= numOfGeneration; ++n)
			{
				memcpy(oldMatrix[0], matrix[0], sizeof(char) * width * height);

				// 다음 세대 계산
				for (int i = 0; i < height; ++i)
					for (int j = 0; j < width; ++j)
						matrix[i][j] = LifeGameAlgo(oldMatrix, j, i);

				sprintf(fileName, "gen_%d.matrix", n);
				if (n < numOfGeneration)
					WriteMatrix(fileName);
				else if (n == numOfGeneration)
					WriteMatrix(DEFAULT_OUTPUT_FILENAME);
			}

			gettimeofday(&end_time, NULL);
			PrintProcessingTime();
			exit(1);
		}
		else if (input == '3') 
		{
			if (flag_t == false)
			{
				printf("Selected (3) Multi-process Parallel Processing\n");
				printf("How many worker process do you want? ");
			}
		
			int procNum;
			scanf("%d", &procNum);
		
			if (flag_t == false)
				printf("How many generation do you want? ");
		
			scanf("%d", &numOfGeneration);

			gettimeofday(&start_time, NULL);

			// 동적 할당
			oldMatrix = (char**) malloc( sizeof(char*) * height );
			oldMatrix[0] = (char*) malloc( sizeof(char) * width * height );
			for (int i = 1; i < height; ++i)
				oldMatrix[i] = oldMatrix[i-1] + width;

			//init divide
			InitDivideWork(procNum);

			pid_t pids[numOfDivide];

			for (int n = 1; n <= numOfGeneration; n++)
			{
				memcpy(oldMatrix[0], matrix[0], sizeof(char) * width * height);

				if (n < numOfGeneration)
					sprintf(fileName, "gen_%d.matrix", n);
				else if (n == numOfGeneration)
					sprintf(fileName, DEFAULT_OUTPUT_FILENAME);
				
				FILE* fp = fopen(fileName, "w+");

				for (int i = 0; i < numOfDivide; i++)
				{
					pids[i] = fork();
					if (pids[i] < 0)
					{
						printf("fork error!\n");
						exit(1);
					}
					else if (pids[i] == 0)
					{
						for (int y = divideOffset[i]; y < divideOffset[i] + divideLength[i]; y++)
							for (int x = 0; x < width; x++)
								matrix[y][x] = LifeGameAlgo(oldMatrix, x, y);

						WriteDividedMatrix(fileName, divideOffset[i], divideLength[i]);

						exit(0);
					}
				}

				for (int i = 0; i < numOfDivide; i++)
					waitpid(pids[i], NULL, 0);

				fseek(fp, 0, SEEK_SET);
				ReadMatrix(fileName);

				fclose(fp);
			}
		
			gettimeofday(&end_time, NULL);
			PrintProcessingTime();
			exit(1);
		} 
		else if (input == '4') 
		{
			if (flag_t == false)
			{
				printf("Selected (4) Multi-thread Parallel Processing\n");
				printf("How many worker thread do you want? ");
			}

			int threadNum;
			scanf("%d", &threadNum);

			if (flag_t == false)
				printf("How many generation do you want? ");
			
			scanf("%d", &numOfGeneration);

			gettimeofday(&start_time, NULL);

			// 동적 할당
			oldMatrix = (char**) malloc( sizeof(char*) * height );
			oldMatrix[0] = (char*) malloc( sizeof(char) * width * height );
			for (int i = 1; i < height; ++i)
				oldMatrix[i] = oldMatrix[i-1] + width;

			//init divide
			InitDivideWork(threadNum);

			pthread_t tids[numOfDivide];

			// 세대 진행 반복
			for (int n = 1; n <= numOfGeneration; ++n)
			{
				memcpy(oldMatrix[0], matrix[0], sizeof(char) * width * height);

				for (int i = 0; i < numOfDivide; i++)
				{
					int *thread_number = (int*)malloc(sizeof(int));
					*thread_number = i;

					if (pthread_create(&tids[i], NULL, LifeGameAlgoForThread, (void *)thread_number) != 0)
					{
						printf("pthread_create error (%d)\n", i);
						exit(1);
					}
				}

				for (int i = 0; i < numOfDivide; i++)
				{
					pthread_join(tids[i], NULL);
				}

				sprintf(fileName, "gen_%d.matrix", n);
				if (n < numOfGeneration)
					WriteMatrix(fileName);
				else if (n == numOfGeneration)
					WriteMatrix(DEFAULT_OUTPUT_FILENAME);
			}

			gettimeofday(&end_time, NULL);
			PrintProcessingTime();
			exit(1);
		}
		else if (input == '5') 
		{
			if (flag_t == false)
				printf("Selected (5) Random Generate input.matrix\n");
			int w, h;

			if (flag_t == false)
				printf("please input width : ");
			scanf("%d", &w);
	
			if (flag_t == false)
				printf("please input height : ");
			scanf("%d", &h);

			RandMatrix(w, h);
			WriteMatrix(DEFAULT_INPUT_FILENAME);

			exit(1);
		}
		else 
			printf("You Input Wrong Number. Return To Menu.\n\n");
	}

	FreeMatrix();

	return 0;
}


// Char type 2차원 배열 Matrix를 동적할당 합니다.
void AllocMatrix(int w, int h) 
{
	if (w > MATRIX_ROW_SIZE_LIMIT || h > MATRIX_ROW_SIZE_LIMIT)
	{
		printf("cannot allocate over MATRIX_ROW_SIZE_LIMIT\n");
		exit(1);
	}

	// 이미 공간이 할당되 있다면 일단 해제한다.
	if (width != 0 || height != 0)
		FreeMatrix();

	width = w;
	height = h;

	matrix = (char**) malloc ( sizeof(char*) * height);
	matrix[0] = (char*) malloc ( sizeof(char) * width * height );

	for (int i = 1; i < height; ++i)
		matrix[i] = matrix[i-1] + width;

	//printf("%d %d matrix allocion complete!\n", width, height);
}

// matrix 동적할당을 해제합니다.
void FreeMatrix()
{
	width = 0;
	height = 0;

	free(matrix[0]);
	free(matrix);
}

// 현재 matrix를 출력합니다.
void PrintMatrix()
{
	printf("@@@@@@@@@@@@@@@@@@@@@@ %d by %d matrix print @@@@@@@@@@@@@@@@@@@@@@\n", width, height);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
			printf("%c ", matrix[i][j]);
		printf("\n");
	}

	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

// 파일로부터 matrix를 읽습니다. 한줄씩 읽어서 버퍼 사이즈보다 크면 안됨.
void ReadMatrix(const char* fileName)
{
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL)
	{
		printf("file read error\n");
		exit(1);
	}

	int w = 0;
	int h = 0;

	while (!feof(fp)) 
	{
		fgets(buffer, sizeof(buffer), fp);
		h++;
	}

	w = (strlen(buffer) + 1) / 2;
	
	AllocMatrix(w, h);

	fseek(fp, 0, SEEK_SET);

	for (int i = 0; i < height; ++i)
	{
		fgets(buffer, sizeof(buffer), fp);
		for (int j = 0; j < width; ++j)
			matrix[i][j] = buffer[j * 2];
	}

	fclose(fp);
}

// 파일에다가 현재 메트릭스 정보를 써준다.
void WriteMatrix(const char* fileName)
{
	FILE *fp = fopen(fileName, "w+");

	if (width == 0 || height == 0)
	{
		printf("file write error\n");
		exit(1);
	}

	memset(buffer, 0, sizeof(buffer));

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			buffer[j * 2] = matrix[i][j];
			buffer[j * 2 + 1] = ' ';
		}

		if (i == height - 1)
			buffer[width * 2] = 0;
		else
		{
			buffer[width * 2] = 0x0D;
			buffer[width * 2 + 1] = 0x0A;
		}

		fputs(buffer, fp);
	}
	fclose(fp);
}

// 파일에다가 부분적으로 메트릭스 정보를 써준다.
void WriteDividedMatrix(const char* fileName, int offset, int length)
{
	FILE *fp = fopen(fileName, "r+");

	if (fp == NULL)
	{
		printf("fopen error\n");
		exit(1);
	}

	fseek(fp, offset * (width + 1) * 2, SEEK_SET);

	for (int i = offset; i < offset + length; ++i)
	{
		memset(buffer, 0, sizeof(buffer));
	
		for (int j = 0; j < width; ++j)
		{
			buffer[j * 2] = matrix[i][j];
			buffer[j * 2 + 1] = ' ';
		}

		if (i == height - 1)
			buffer[width * 2] = 0;
		else
		{
			buffer[width * 2] = 0x0D;
			buffer[width * 2 + 1] = 0x0A;
		}

		fputs(buffer, fp);
	}
	fclose(fp);
}

// 랜덤으로 matrix를 생성합니다.
void RandMatrix(int w, int h)
{
	AllocMatrix(w, h);

    srand((unsigned int)time(NULL));

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			matrix[i][j] = rand() % 2 ? '1' : '0';
}

// 넘겨준 매트릭스를 기반으로, 다음 세대의 (x,y)위치의 결과를 반환합니다.
char LifeGameAlgo(char** matrix, int x, int y)
{

	int left = (x-1 == -1) ? x : x-1;
	int right = (x+1 == width) ? x : x+1;
	int up = (y-1 == -1) ? y : y-1;
	int down = (y+1 == height) ? y : y+1;
	int count = 0;

	for (int i = up; i <= down; ++i) 
	{
		for (int j = left; j <= right; ++j)
		{
			// (x,y)라면 넘어가고
			if (i == y && j == x)
				continue;

			// (x,y)가 아닌 주위 8칸의 '1' 개수를 셈니다.
			if (matrix[i][j] == '1')
				count++;
		}
	}

	if (matrix[y][x] == '1')
	{
		// 규칙 1 : 해당 세포가 살아있을 때, 살아있는 이웃 세포의 수가
		// 2개 이하 혹은 7개 이상일 경우 해당 세포는 다음 세대에 죽는다.
		if (count <= 2 || count >= 7)
			return '0';
		// 규칙 2 : 해당 세포가 살아있을 때, 살아있는 이웃 세포의 수가
		// 3개 이상, 6개 이하인 경우 해당 세포는 다음 세대에 살아서 유지된다.
		else
			return '1';
	}
	else
	{
		// 규칙 3 : 해당 세포가 죽어있을 때, 살아있는 이웃 세포의 수가
		// 4개인 경우 해당 세포는 다음 세대에 살아난다.
		if (count == 4) 
			return '1';
		// 규칙 4 : 해당 세포가 죽어있을 때, 살아있는 이웃 세포의 수가
		// 4개를 제외한 나머지 경우에는 다음 세대에 여전히 죽어있는 상태이다.
		else
			return '0';
	}
}

// 일을 할 범위를 분할해줍니다.
// numOfDivide, divideOffset, divideLength를 초기화해줍니다.
void InitDivideWork(int num)
{
	if (num > height)
	{
		printf("Divide error!\n");
		exit(1);
	}

	numOfDivide = num;
	divideOffset = (int *)malloc(sizeof(int) * numOfDivide);
	divideLength = (int *)malloc(sizeof(int) * numOfDivide);

	memset(divideOffset, 0, sizeof(int) * numOfDivide);
	memset(divideLength, 0, sizeof(int) * numOfDivide);

	for (int i = 0; i < height; i++)
		divideLength[i % numOfDivide]++;

	divideOffset[0] = 0;
	for (int i = 1; i < numOfDivide; i++)
		divideOffset[i] = divideOffset[i-1] + divideLength[i-1];
}

// 인자로 넘겨준 thread number의 일을 수행합니다.
void *LifeGameAlgoForThread(void* arg)
{
	int thread_number = *((int *)arg);

	for (int y = divideOffset[thread_number]; y < divideOffset[thread_number] + divideLength[thread_number]; y++)
		for (int x = 0; x < width; x++)
			matrix[y][x] = LifeGameAlgo(oldMatrix, x, y);	

	pthread_exit(NULL);
	return NULL;
}

void PrintProcessingTime()
{
	proc_usec = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
	if (flag_t)
		printf("%ld\n", proc_usec);
	else
	{
		proc_sec = proc_usec / 1000000;
		proc_usec = proc_usec - proc_sec * 1000000;
		printf("Completed %d Generation. [ Taken Time : %ld s %ld us]\n\n", numOfGeneration, proc_sec, proc_usec);
	}
}