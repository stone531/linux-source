#include <stdio.h>
#include <string.h>

char *files[500];

int
main (int argc, char *argv[])
{
  char buf[FILENAME_MAX];
  char *fn;
  FILE *fp;
  int i;

  for (i = 0; i < 500; i++) {
    fn = __stdio_gen_tempname(buf, sizeof (buf), (const char *) NULL,
	"file", 0, (size_t *) NULL, (FILE **) NULL);
    if (fn == NULL) {
      printf ("__stdio_gen_tempname failed\n");
      exit (1);
    }
    files[i] = strdup (fn);
    printf ("file: %s\n", fn);
    fp = fopen (fn, "w");
    fclose (fp);
  }

  for (i = 0; i < 500; i++)
    remove (files[i]);

  exit (0);
}
