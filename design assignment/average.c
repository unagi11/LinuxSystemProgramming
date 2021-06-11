#include <stdio.h>

int main(int argc, char const *argv[])
{
   int sum = 0;
   int count = 0;
   scanf("%d\n", &count);

   for (int i = 0; i < count; i++)
   {
       int temp = 0;
       scanf("%d\n", &temp);
       sum += temp;
   }
   
    printf("%d\n", sum / count);

    return 0;
}