#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

//size of matrix N*N
#define N 9
#define K 40
#define MAX_LEN 200

int SRN, n, pos, value;
int arr[N][N], solved[N][N], validij[40];

int rndm(int);
int menu();
void sudoku()
{
    double SRNd = sqrt(N);
    SRN = (int)SRNd;
}
void print_matrix()
{
    system("clear");
    printf("\t\t\t  1 2 3 4 5 6 7 8 9\n");
    printf("\t\t\t  -----------------\n");
    for (int i = 0; i < N; i++)
    {
        printf("\t\t\t");
        for (int j = 0; j < N; j++)
        {
            if( j == 0)
            {
            	printf("%d|%d ",i+1, arr[i][j]);
            }
            else
            {
            printf("%d ", arr[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\t\tInput syntax:a-position value");
    printf("\n\t\tYou can enter 0 0 to go back to main menu");
}
bool not_in_box(int rowS, int colS, int num) //not_in_box = unUsedInBox
{
    for (int i = 0; i < SRN; i++)
    {
        for (int j = 0; j < SRN; j++)
        {
            if (arr[rowS + i][colS + j] == num)
                return false;
        }
    }
    return true;
}

void fill_box(int row, int col)
{
    int num;
    srand(time(0));
    for (int i = 0; i < SRN; i++)
    {
        for (int j = 0; j < SRN; j++)
        {

            do
            {
                num = rndm(N);
            } while (!not_in_box(row, col, num));
            arr[row + i][col + j] = num;
        }
    }
}
void fill_diagonals()
{
    for (int i = 0; i < N; i = i + SRN)
    {
        fill_box(i, i);
    }
}

int rndm(int num)
{
    return rand() % (num + 1);
}
bool not_in_row(int i, int num)
{
    for (int j = 0; j < N; j++)
        if (arr[i][j] == num)
            return false;
    return true;
}
bool not_in_col(int j, int num)
{
    for (int i = 0; i < N; i++)
    {
        if (arr[i][j] == num)
            return false;
    }
    return true;
}
bool check_if_safe(int i, int j, int num)
{
    return (not_in_row(i, num) &&
            not_in_col(j, num) &&
            not_in_box(i - i % SRN, j - j % SRN, num));
}
bool fillRemaining(int i, int j)
{
    if (j >= N && i < N - 1)
    {
        i = i + 1;
        j = 0;
    }
    if (i >= N && j >= N)
        return true;
    if (i < SRN)
    {
        if (j < SRN)
            j = SRN;
    }
    else if (i < N - SRN)
    {
        if (j == (int)(i / SRN) * SRN)
            j = j + SRN;
    }
    else
    {
        if (j == N - SRN)
        {
            i = i + 1;
            j = 0;
            if (i >= N)
                return true;
        }
    }
    for (int num = 1; num <= N; num++)
    {
        if (check_if_safe(i, j, num))
        {
            arr[i][j] = num;
            if (fillRemaining(i, j + 1))
                return true;
            arr[i][j] = 0;
        }
    }
    return false;
}
void solved_grid()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            solved[i][j] = arr[i][j];
    }
}
void print_solved()
{
    printf("\nSolved Matrix:\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", solved[i][j]);
        }
        printf("\n");
    }
}
void check_input(int i, int j, int x)
{
    validij[x] = (i + 1) * 10 + (j + 1);
}
void remove_rnd_digits()
{
    int count = K, x = 0;
    do
    {
        int cell_id = rndm(N * N) - 1;
        int i = (cell_id / N);
        int j = cell_id % 9;
        if (j != 0)
            j = j - 1;

        if (arr[i][j] != 0)
        {
            count--;
            arr[i][j] = 0;
            check_input(i, j, x);
            x += 1;
        }

    } while (count != 0);
}
void fill_values()
{
    fill_diagonals(); //randomly fill the diagonals

    fillRemaining(0, SRN); //filling the rest of the boxes

    solved_grid(); //store the solution in a different matrix for later use

    remove_rnd_digits(); //randomly remove digits to generate a sudoku;
}
int invalid(int x)
{
    for (int i = 0; i < 40; i++)
    {
        if (x == validij[i]) //validij is the postion of all 0s
            return 1;
    }
    return 0;
}
int check(int p, int v) //p for pos, v for value
{
    int i = p / 10;
    int j = p % 10;
    int r;
    r = check_if_safe(i - 1, j - 1, v);
    if (v >= 10)
        return 2;
    return r;
}

void update_scr()
{
    int i, j;
    i = pos / 10;
    j = pos % 10;
    if (check(pos, value) == 1) //row, column and box check)
    {
        arr[i - 1][j - 1] = value;
        print_matrix();
    }
    else if (check(pos, value) == 2)
    {
        printf("Go through the game rules again please!");
    }
    else
    {
        printf("Already in row or column or box");
    }
}
void print_text(FILE *fptr)
{
    char read_string[MAX_LEN];

    while (fgets(read_string, sizeof(read_string), fptr) != NULL)
        printf("%s", read_string);
}
int how_to_play()
{
    int in;
    system("clear");
    char *filename = "HTP.txt";
    FILE *fptr = NULL;

    if ((fptr = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error printing title! %s\n", filename);
        return 1;
    }

    print_text(fptr);
    fclose(fptr);
    y:
    printf("\nEnter 0 to exit to main menu:");
    scanf("%d", &in);
    switch (in)
    {
    case 0:
        menu();
        break;

    default:
        goto y;
        break;
    }
}
int check_if_solved()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (arr[i][j] == 0)
                return 0;
        }
    }
    return 1;
}
void user_prompt()
{
    int boolean;

    x:
    printf("\nYour Input:a-");
    scanf("%d%d", &pos, &value);
    if (check_if_solved() == 1)
    {
        printf("Congratulations! You Solved it.\n");
    }
    else
    {
        if (pos == 0 && value == 0)
        {
            menu();
        }
        else
        {
            boolean = invalid(pos);
            switch (boolean)
            {
            case 1:
                update_scr();
                goto x;
                break;
            case 0:
                printf("a%d is permanent\n", pos);
                sleep(1);
                goto x;
                break;

            default:
                printf("Can't understand that input\n");
                sleep(1);
                goto x;
                break;
            }
        }
    }
}
void interface()
{
    char ch;
    int op;
    printf("1.Start a Game\n");
    printf("2.How to Play\n");
    printf("0.Exit\n");
    printf("Option:");
    scanf("%d", &op);
    switch (op)
    {
    case 0:
        break;
    case 1:
        print_matrix();
        user_prompt();
        break;
    case 2:
        how_to_play();
        break;

    default:
        printf("That option isn't available");
        break;
    }
}

int menu()
{
    system("clear");
    char *filename = "title.txt";
    FILE *fptr = NULL;

    if ((fptr = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error printing title! %s\n", filename);
        return 1;
    }

    print_text(fptr);
    fclose(fptr);
    interface();

    return 0;
}

int main()
{
    system("clear");
    sudoku();
    fill_values();
    //print_solved();
    menu();

    return 0;
}
