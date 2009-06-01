/* -*- mode: c -*-
 *
 * Nagios check_memcached_paranoid plugin
 *
 * license: fixme
 * copyright: fixme
 */

const char *progname = "check_memcached_paranoid";
const char *revision = "1.0";
const char *copyright = "2009-";
const char *email = "hirose31 _at_ gmail.com";

#include "common.h"
#include "utils.h"
#include "utils_base.h"
#include "netutils.h"

#define MEMCACHED_PORT 11211

char        *mc_host = NULL;
unsigned int mc_port = MEMCACHED_PORT;

int  process_arguments(int, char **);
int  validate_arguments(void);
void print_help(void);
void print_usage(void);

int verbose = 0;
thresholds *my_thresholds = NULL;

#if 1
#define TRACE(fmt, ...)                         \
  {                                             \
    struct tm tm;                               \
    struct timeval tv;                          \
    gettimeofday(&tv, NULL);                    \
    localtime_r(&(tv.tv_sec), &tm);                            \
    printf("%02d:%02d:%02d.%06lu %.12s.%4d: (trace ) "fmt"\n", \
           tm.tm_hour,tm.tm_min,tm.tm_sec,                     \
           tv.tv_usec,                                         \
           __FILE__, __LINE__, __VA_ARGS__);                   \
  }
#define TRACE_LONG(fmt, ...)                    \
  {                                             \
    struct tm tm;                               \
    struct timeval tv;                          \
    gettimeofday(&tv, NULL);                    \
    localtime_r(&(tv.tv_sec), &tm);                                     \
    printf("%04d-%02d-%02d %02d:%02d:%02d.%06lu %s.%4d: (trace ) "fmt"\n", \
           tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,                      \
           tm.tm_hour,tm.tm_min,tm.tm_sec,                              \
           tv.tv_usec,                                                  \
           __FILE__, __LINE__, __VA_ARGS__);                            \
  }
#else
#define TRACE(fmt, ...)
#endif

#if 0
#define TRACE2(fmt, ...)                        \
  #define TRACE(fmt, ...)                       \
  {                                             \
    struct tm tm;                               \
    struct timeval tv;                          \
    gettimeofday(&tv, NULL);                    \
    localtime_r(&(tv.tv_sec), &tm);                            \
    printf("%02d:%02d:%02d.%06lu %.12s.%4d: (trace2) "fmt"\n", \
           tm.tm_hour,tm.tm_min,tm.tm_sec,                     \
           tv.tv_usec,                                         \
           __FILE__, __LINE__, __VA_ARGS__);                   \
  }
#else
#define TRACE2(fmt, ...)
#endif

int main(int argc, char ** argv)
{
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Parse extra opts if any */
  argv=np_extra_opts (&argc, argv, progname);

  if (process_arguments (argc, argv) == ERROR)
    usage4 (_("Could not parse arguments"));


  TRACE("%s",">>main");
  return 0;
}

/* process command-line arguments */
int
process_arguments (int argc, char **argv)
{
  int c;
  char *warning = NULL;
  char *critical = NULL;

  int option = 0;
  static struct option longopts[] = {
    {"hostname" ,required_argument, 0, 'H'},
    {"port"     ,required_argument, 0, 'P'},
    {"verbose"  ,no_argument,       0, 'v'},
    {"version"  ,no_argument,       0, 'V'},
    {"help"     ,no_argument,       0, 'h'},
    {"warning"  ,required_argument, 0, 'w'},
    {"critical" ,required_argument, 0, 'c'},
    {0, 0, 0, 0}
  };

  if (argc < 1)
    return ERROR;

  while (1) {
    c = getopt_long (argc, argv, "H:P:vVhw:c:", longopts, &option);

    if (c == -1 || c == EOF)
      break;

    switch (c) {
    case 'H':
      if (is_host (optarg)) {
        mc_host = optarg;
      }
      else {
        usage2(_("Invalid hostname/address"), optarg);
      }
      break;
    case 'P':
      mc_port = atoi(optarg);
      break;
    case 'v':
      verbose++;
      break;
    case 'V':
      print_revision(progname, revision);
      exit(STATE_OK);
    case 'h':
      print_help();
      exit (STATE_OK);
    case 'w':
      warning  = optarg;
      break;
    case 'c':
      critical = optarg;
      break;
    case '?':
      usage5();
    }
  }

  c = optind;

  set_thresholds(&my_thresholds, warning, critical);

  return validate_arguments();
}


int validate_arguments(void)
{
  if (mc_host == NULL)
    usage("Must specify memcached server");

  return OK;
}


void print_help(void)
{
  char *mcport;
  asprintf (&mcport, "%d", MEMCACHED_PORT);

  print_revision (progname, revision);

  printf (_(COPYRIGHT), copyright, email);

  printf ("%s\n", _("This program checks a query result of SET, GET, DELETE, GET"));

  printf ("\n\n");

  print_usage ();

  printf (_(UT_HELP_VRSN));
  printf (_(UT_EXTRA_OPTS));
  printf (_(UT_WARN_CRIT_RANGE));
  printf (_(UT_HOST_PORT), 'P', mcport);

#ifdef NP_EXTRA_OPTS
  printf ("\n");
  printf ("%s\n", _("Notes:"));
  printf (_(UT_EXTRA_OPTS_NOTES));
#endif

  printf (_(UT_SUPPORT));
}


void print_usage(void)
{
  printf (_("Usage:"));
  printf (" %s -H host [-P port] [-w warn] [-c crit]\n", progname);
}
