#include <stdio.h>

#define SIZE 12

int input_arr(int n, int arr[]);
void print_arr(int size, int a[]);
void swap(int*a, int*b);
void revers_arr(int size, int a[]);
void inversion_every_third_arr(int size, int a[]);

int main(void)
{
    int arr[SIZE] = {0};
    input_arr(SIZE,arr);
    inversion_every_third_arr(SIZE,arr);
    print_arr(SIZE,arr);
    return 0;
}

int input_arr(int n, int a[])

    return i;
}

void print_arr(int size, int a[])
{
    for (size_t i = 0; i < size; i++ )
    {
         printf("%d ",a[i]);
    }
    printf("\n");
    return;
}

void swap(int*a, int*b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void revers_arr(int size, int a[])
{
    for(size_t i = 0; i < size/2; i++)
    {
        swap(&a[i], &a[size-1-i]);
    }
 return;
}

void inversion_every_third_arr(int size, int a[])
{
    for (size_t i = 0; i < 3; i++)
    {
        revers_arr(SIZE/3,a + i*SIZE/3);
    }
    return;
}





