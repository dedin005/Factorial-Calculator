#include <pthread.h>
#include <stdio.h>
#include <gmp.h>
#include <unistd.h>
#include <locale.h>

#define THREAD_NUM 8
#define FACTORIAL 1000000

unsigned long progs[THREAD_NUM] = {0};

typedef struct
{
  mpz_t *n;
  int start;
  int stop;
} args_t;

args_t init_args(mpz_t *n, int start, int stop)
{
  args_t t;
  t.n = n;
  t.start = start;
  t.stop = stop;
  return t;
}

void *factorial_part(void *a)
{

  args_t args = *(args_t *)a;

  mpz_t *p = args.n;

  for (int i = args.start; i <= args.stop; i += THREAD_NUM)
  {
    mpz_mul_ui(*p, *p, i);
    progs[args.start - 1]++;
  }
  return NULL;
}

// compile with gcc -Ofast -o p p.c -lgmp -lpthread
int main()
{
  setlocale(LC_NUMERIC, "");

  mpz_t n[THREAD_NUM];
  args_t a[THREAD_NUM];
  pthread_t p[THREAD_NUM];
  unsigned long sum = 0;

  for (int i = 0; i < THREAD_NUM; i++)
  {
    mpz_init(n[i]);
    mpz_set_ui(n[i], 1);
    a[i] = init_args(&n[i], i + 1, FACTORIAL);
    pthread_create(&p[i], NULL, factorial_part, (void *)&a[i]);
  }

  printf("%'ld / %'d\r", sum, FACTORIAL);
  fflush(stdout);

  while (1)
  {
    sum = 0;
    for (int i = 0; i < THREAD_NUM; i++)
    {
      sum += progs[i];
    }

    printf("%'ld / %'d\r", sum, FACTORIAL);
    fflush(stdout);

    if (sum >= FACTORIAL)
    {
      break;
    }

    sleep(1);
  }

  mpz_t result;
  mpz_init(result);
  mpz_set_ui(result, 1);

  for (int i = 0; i < THREAD_NUM; i++)
  {
    printf("\nfinal multiplication %d / %d\r", i, THREAD_NUM);
    fflush(stdout);
    pthread_join(p[i], NULL);
    mpz_mul(result, result, n[i]);
  }

  printf("writing\n");

  FILE *fp = fopen("output.txt", "w+");

  mpz_out_str(fp, 10, result);

  fclose(fp);

  return 0;
}
