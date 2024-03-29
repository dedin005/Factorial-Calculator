#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <gmp.h>
#include <unistd.h>
#include <locale.h>

#define str(x) #x
#define stringify(x) str(x)

#define THREAD_NUM 7
#define FACTORIAL 1000000

unsigned long progs[THREAD_NUM] = {0};

typedef struct
{
  mpz_t *n;
  uint64_t start;
} args_t;

typedef struct
{
  mpz_t *result;
  mpz_t *multiplier;
} cleanup_t;

void *cleanup(void *a)
{
  cleanup_t c = *(cleanup_t *)a;
  mpz_mul(*c.result, *c.multiplier, *c.result);

  return NULL;
}

void *factorial_part(void *a)
{
  args_t args = *(args_t *)a;

  mpz_t *p = args.n;

  int64_t start = args.start;

  for (int64_t i = FACTORIAL - start + 1; i > 0; i -= THREAD_NUM)
  {
    mpz_mul_ui(*p, *p, i);
    progs[start - 1]++;
  }

  return NULL;
}

int main()
{
  if (THREAD_NUM <= 0)
  {
    printf("You need at least 1 thread to run this program.\n");
    return 1;
  }
  else if (FACTORIAL < 0)
  {
    printf("This function doesn't work for negative factorials.\n");
    return 1;
  }

  setlocale(LC_NUMERIC, "");

  mpz_t n[THREAD_NUM + THREAD_NUM % 2];
  args_t a[THREAD_NUM];
  pthread_t p[THREAD_NUM];
  uint64_t sum = 0;

  // if odd number of threads, pad for cleanup later
  if (THREAD_NUM % 2)
  {
    mpz_init(n[THREAD_NUM]);
    mpz_set_ui(n[THREAD_NUM], 1);
  }

  for (uint64_t i = 0; i < THREAD_NUM; i++)
  {
    mpz_init(n[i]);
    mpz_set_ui(n[i], 1);

    a[i].n = &n[i];
    a[i].start = i + 1;

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
  printf("\n");

  // cleanup and do multiplication of all the parts
  int count = THREAD_NUM + THREAD_NUM % 2;
  int half = count / 2;

  cleanup_t cleanupArgs[half];
  pthread_t cleanupThreads[half];

  while (count > 1)
  {
    printf("Remaining parts: %'d\r", count);
    fflush(stdout);

    for (int i = 0; i < half; i++)
    {
      cleanupArgs[i].result = &n[i];
      cleanupArgs[i].multiplier = &n[count - 1 - i];
    }

    for (int i = 0; i < half; i++)
    {
      pthread_create(&cleanupThreads[i], NULL, cleanup, (void *)&cleanupArgs[i]);
    }

    // wait for all the threads to finish before continuing cleanup
    for (int i = 0; i < half; i++)
    {
      pthread_join(cleanupThreads[i], NULL);
    }

    count = count / 2;
    if (count % 2 && count > 1)
    {
      mpz_set_ui(n[count++], 1);
    }
    half = count / 2;
  }
  printf("\n");

  FILE *fp = fopen("Factorial of " stringify(FACTORIAL) ".txt", "w+");

  mpz_out_str(fp, 10, n[0]);

  fclose(fp);

  return 0;
}
