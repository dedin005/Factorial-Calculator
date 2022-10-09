#include <gmp.h>
#include <fcntl.h>
#include <gmp.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
typedef struct {
  mpz_t *n;
  int start;
  int stop;
} args_t;

args_t init_args(mpz_t *n, int start, int stop) {
  args_t t;
  t.n = n;
  t.start = start;
  t.stop = stop;
  return t;
}

void *factorial_part(void *a) {

  args_t args = *(args_t *)a;

  mpz_t *p = args.n;

  for (int i = args.start; i <= args.stop; i += 20) {
    mpz_mul_ui(*p, *p, i);
  }
}
*/

int main()
{
  /*
   */
  mpz_t n0, n1;

  mpz_inits(n0, n1, NULL);

  mpz_set_ui(n0, 4);
  mpz_set_ui(n1, 20);

  mpz_mul(n0, n0, n1);

  FILE *f = fopen("test.txt", "w+");

  mpz_out_str(f, 10, n0);

  // for (int j = 0; j < 10000; j++)
  // {
  //   for (int i = 0; i < 100000; i++)
  //   {
  //     mpz_mul_ui(n0, n0, 99999999);
  //   }
  //   mpz_set_ui(n0, 1);
  // }

  // int fd = open("output.txt", O_RDWR);

  // struct stat sb;
  // fstat(fd, &sb);

  // char *nums = (char *)mmap(NULL, (__uint64_t)sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);

  // printf("size: %lu\n", (__uint64_t)sb.st_size);
  // printf("first 100 values:\n");
  // for (int i = 0; i < 100; i++) {
  //   printf("%d\n", ((int*)nums)[i]);
  // }

  // mpz_export((void *)nums, NULL, 1, 1, 0, 0, n0);

  // munmap(nums, (__uint64_t)sb.st_size);

  //*/
  /*
  FILE *f = fopen("output.txt", "w+");

  // 756570556
  int j = 0b00000000000000000000000000000000;
  for (long i = 0; i < 3400000000; i++)
  {
    fwrite(&j,1,1,f);
  }
  fclose(f);
  //*/

  mpz_clear(n0);

  return 0;
}