#include <stdio.h>

#define SIZE 10

int input_arr(int n, int a[]);
void print_arr(int size, int a[]);
void swap(int*a, int*b);
void bubble_sort_array(int size, int a[]);

int main(void)
{
    int arr[SIZE] = {0};
    input_arr(SIZE, arr);
    bubble_sort_array(SIZE, arr);
    print_arr(SIZE, arr);
    return 0;
}


int input_arr(int n, int a[])
{
int i;
    for(i = 0; i < n; i++)
        scanf("%d", &a[i]);
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

void bubble_sort_array(int size, int a[])
{
    int i = 0;
    _Bool flag;
    do
    {
        flag = 0;
        for (int j = size-2; j >= i ; j-- )
            if ( a[j] % 10 > a[j+1] % 10 )
            {
                swap(&a[j], &a[j+1]);
                flag = 1;
            }
        i++;
    }
    while ( flag );
}

