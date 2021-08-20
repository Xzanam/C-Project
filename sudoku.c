#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

//size of matrix N*N
#define N 9
#define K 40

int SRN,n;
int arr[N][N], solved[N][N];

int rndm(int);

void sudoku()
{
    double SRNd =  sqrt(N);
    SRN = (int) SRNd;
}
void print_matrix()
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}
bool not_in_box(int rowS, int colS, int num)//not_in_box = unUsedInBox
{
    for(int i = 0; i<SRN;  i++)
    {
        for(int j = 0; j<SRN; j++)
        {
            if(arr[rowS+i][colS+j]==num)
                return false;
        }
    }
    return true;
}

void fill_box(int row, int col)
{
    int num;
    srand(time(0));
    for(int i = 0; i<SRN; i++)
    {
        for(int j = 0; j<SRN; j++)
        {
          
            do
            {
                num = rndm(N);
            } while (!not_in_box(row, col, num));
            arr[row+i][col+j]= num;
        }
    }
}
void fill_diagonals()
{
    for(int i = 0; i<N; i = i + SRN)
    {
        fill_box(i,i);
    }
}


int rndm(int num)
{
    return rand() % (num+1);
}
bool not_in_row(int i, int num)
{
    for(int j = 0; j<N; j++)
        if(arr[i][j]==num)
            return false;
        return true;
}
bool not_in_col(int j, int num)
{
    for(int i = 0; i <N; i++)
    {
        if(arr[i][j]==num)
            return false;
    }
    return true;
}
bool check_if_safe(int i, int j, int num)
{
    return(not_in_row(i, num) && 
    not_in_col(j,num) && 
    not_in_box(i - i % SRN, j-j %SRN,num));
}
bool fillRemaining(int i, int  j)
{
    if(j>=N && i < N-1)
    {
        i = i+1;
        j = 0;
    }
    if(i >=N && j>= N)
        return true;
    if(i <SRN)
    {
        if(j <SRN)
            j= SRN;
    }
    else if(i < N-SRN)
    {
        if (j ==(int) (i/SRN)*SRN)
            j = j + SRN;
    }
    else{
        if (j == N - SRN)
        {
            i = i + 1;
            j = 0;
            if (i >= N)
                return true;
        }
    }
    for (int num = 1; num <=N; num++)
    {
        if (check_if_safe(i, j, num))
        {
            arr[i][j] = num;
            if(fillRemaining(i, j+1))
                return true;
            arr[i][j] = 0;
        }
    }
    return false;
}
void solved_grid()
{
    for(int i = 0; i< N; i++)
    {
        for(int j = 0; j < N; j++)
            solved[i][j]= arr[i][j];
    }
}
void print_solved()
{
    printf("\nSolved Matrix:\n");
    for( int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            printf("%d ", solved[i][j]);
        }
        printf("\n");
    }
}
void remove_rnd_digits()
{
    int count = K;
    do
    {
        int cell_id = rndm(N*N)-1;
        int i = (cell_id/N);
        int j = cell_id % 9;
        if ( j != 0)
            j = j - 1;

        if (arr[i][j] != 0)
        {
            count--;
            arr[i][j] = 0;
        } 
    }
    while(count != 0);
}
void fill_values()
{
    fill_diagonals();//randomly fill the diagonals

    fillRemaining(0, SRN);//filling the rest of the boxes

    solved_grid();//store the solution in a different matrix for later use

    remove_rnd_digits();//randomly remove digits to generate a sudoku;
}

int main()
{
    char pos;
    int a;
    system("clear");
    sudoku();
    fill_values();
    print_matrix();
    //print_solved();
    printf("Your Input:");
    scanf("%c%d",pos,a);
    switch (a)
    {
    case 11:
        
        break;
    
    default:
        break;
    }
    return 0;
}

