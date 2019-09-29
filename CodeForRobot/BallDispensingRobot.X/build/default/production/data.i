# 1 "data.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "data.c" 2








# 1 "./data.h" 1








# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdbool.h" 1 3
# 9 "./data.h" 2

# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 1 3



# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 2 3






# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\features.h" 1 3
# 10 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 2 3
# 21 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 3
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 22 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long int wchar_t;
# 127 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned size_t;
# 176 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef __int24 int24_t;
# 212 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef __uint24 uint24_t;
# 21 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 2 3


int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char *restrict, char **restrict);
double strtod (const char *restrict, char **restrict);
long double strtold (const char *restrict, char **restrict);





long strtol (const char *restrict, char **restrict, int);
unsigned long strtoul (const char *restrict, char **restrict, int);
long long strtoll (const char *restrict, char **restrict, int);
unsigned long long strtoull (const char *restrict, char **restrict, int);

int rand (void);
void srand (unsigned);
# 52 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 3
          void abort (void);
int atexit (void (*) (void));
          void exit (int);
          void _Exit (int);
# 65 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 3
void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));





__attribute__((nonreentrant)) void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);


typedef struct { unsigned int quot, rem; } udiv_t;
typedef struct { unsigned long quot, rem; } uldiv_t;
udiv_t udiv (unsigned int, unsigned int);
uldiv_t uldiv (unsigned long, unsigned long);
# 104 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdlib.h" 3
size_t __ctype_get_mb_cur_max(void);
# 10 "./data.h" 2


enum direction {Left = 0, Right = 1};
typedef enum direction direction;

struct canister{
    int id;
    direction facing;
    float distance;
    _Bool status;
    _Bool ballDeposited;
};

typedef struct canister canister;


void initCanisters(void);


void storeCanisterData(direction facing, float dist, _Bool status, _Bool deposit);

void updateCanData(int id, _Bool status, _Bool deposit);

canister getData(int id);
int getNumCanisters(void);
int getNumBallsGiven(void);
void increaseNumBalls(void);
# 9 "data.c" 2



volatile canister cans[10];
volatile int numCans;
volatile int numBalls;


void initCanisters(void)
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        (cans[i]).id = 0;
        (cans[i]).facing = Left;
        (cans[i]).distance = 0;
        (cans[i]).status = 0;
        (cans[i]).ballDeposited = 0;
    }
    numCans = 0;
}

void storeCanisterData(direction facing, float dist, _Bool status, _Bool deposit)
{
    numCans++;
    int id = numCans;
    (cans[id-1]).id = id;
    (cans[id-1]).facing = facing;
    (cans[id-1]).distance = dist;
    (cans[id-1]).status = status;
    (cans[id-1]).ballDeposited = deposit;
}

void updateCanData(int id, _Bool status, _Bool deposit)
{
    (cans[id-1]).status = status;
    (cans[id-1]).ballDeposited = deposit;
}

canister getData(int id)
{
    return cans[id - 1];
}

int getNumCanisters(void)
{
    return numCans;
}

int getNumBallsGiven(void)
{
# 70 "data.c"
    return numBalls;
}

void increaseNumBalls(void)
{
    numBalls++;
}
