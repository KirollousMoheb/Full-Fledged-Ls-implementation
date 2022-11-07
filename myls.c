#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#define __USE_XOPEN
#include <time.h>

char buf[255];
char mytime[255];
struct group *grp;
struct passwd *pswd;
struct dirent *entry;
struct stat mystat;
struct tm tm;
DIR *dirptr;
char cwd[100];
static int
filetypeletter (int mode)
{
  char c;

  if (S_ISREG (mode))
    c = '-';
  else if (S_ISDIR (mode))
    c = 'd';
  else if (S_ISBLK (mode))
    c = 'b';
  else if (S_ISCHR (mode))
    c = 'c';
  else
    {
      /* Unknown type -- possibly a regular file? */
      c = '?';
    }
  return (c);
}

/* Convert a mode field into "ls -l" type perms field. */
static char *
lsperms (int mode)
{
  static const char *rwx[] = { "---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"
  };
  static char bits[11];

  bits[0] = filetypeletter (mode);
  strcpy (&bits[1], rwx[(mode >> 6) & 7]);
  strcpy (&bits[4], rwx[(mode >> 3) & 7]);
  strcpy (&bits[7], rwx[(mode & 7)]);
  if (mode & S_ISUID)
    bits[3] = (mode & S_IXUSR) ? 's' : 'S';
  if (mode & S_ISGID)
    bits[6] = (mode & S_IXGRP) ? 's' : 'l';
  if (mode & S_ISVTX)
    bits[9] = (mode & S_IXOTH) ? 't' : 'T';
  bits[10] = '\0';
  return (bits);
}

// ls -l -a -i
int
longFormatwithInode (int show_hidden)
{

  while ((entry = readdir (dirptr)) != NULL)
    {
      if (stat (entry->d_name, &mystat) < 0)
	{

	  return -1;
	}
      if (!show_hidden && entry->d_name[0] == '.')
	{
	  continue;
	}

      grp = getgrgid (mystat.st_gid);
      pswd = getpwuid (mystat.st_uid);
      printf ("%-10lu", entry->d_ino);
      printf ("%-10s  %-3lu %-5s %-5s %-5lu ", lsperms (mystat.st_mode),
	      mystat.st_nlink, pswd->pw_name, grp->gr_name, mystat.st_size);
      sprintf (mytime, "%ld", mystat.st_mtime);
      memset (&tm, 0, sizeof (struct tm));
      strptime (mytime, "%s", &tm);
      strftime (buf, sizeof (buf), "%b %d %H:%M ", &tm);
      printf ("%-7s", buf);
      printf ("%-7s", entry->d_name);
      printf ("\n");

    }
  chdir (cwd);

}

// ls -l -a 
int
longFormat (int show_hidden)
{

  while ((entry = readdir (dirptr)) != NULL)
    {
      if (stat (entry->d_name, &mystat) < 0)
	{

	  return -1;
	}
      if (!show_hidden && entry->d_name[0] == '.')
	{
	  continue;
	}

      grp = getgrgid (mystat.st_gid);
      pswd = getpwuid (mystat.st_uid);
      printf ("%-10s  %-3lu %-5s %-5s %-5lu ", lsperms (mystat.st_mode),
	      mystat.st_nlink, pswd->pw_name, grp->gr_name, mystat.st_size);
      sprintf (mytime, "%ld", mystat.st_mtime);
      memset (&tm, 0, sizeof (struct tm));
      strptime (mytime, "%s", &tm);
      strftime (buf, sizeof (buf), "%b %d %H:%M ", &tm);
      printf ("%-7s", buf);
      printf ("%-7s", entry->d_name);
      printf ("\n");

    }
  chdir (cwd);

}

int
normalFormatwithInode (int show_hidden)
{

  while ((entry = readdir (dirptr)) != NULL)
    {
      if (stat (entry->d_name, &mystat) < 0)
	{

	  return -1;
	}
      if (!show_hidden && entry->d_name[0] == '.')
	{
	  continue;
	}

      printf ("%ld %s    ", entry->d_ino, entry->d_name);


    }
  printf ("\n");
  chdir (cwd);

}

int
normalFormat(int show_hidden)
{

  while ((entry = readdir (dirptr)) != NULL)
    {
      if (stat (entry->d_name, &mystat) < 0)
	{

	  return -1;
	}
      if (!show_hidden && entry->d_name[0] == '.')
	{
	  continue;
	}

      printf ("%s    ", entry->d_name);


    }
  printf ("\n");
  chdir (cwd);

}

int
main (int argc, char **argv)
{

  getcwd (cwd, 100);

  int all_options=0;
  int show_hidden=0;
  int recursive=0;
  int option;

  while ((option = getopt (argc, argv, "ilaR")) != -1)
    {
      switch (option)
	{

	case 'R':
	  recursive+='R';
	  break;
	case 'i':
	  all_options+='i';
	  break;
	case 'l':
	  all_options+='l';
	  break;
	case 'a':
	  show_hidden=1;;
	  break;

	}
    }

  if (argc - optind == 0)
    {
      dirptr = opendir (".");
      chdir (".");
      	  if(recursive=='R'){
	  
	  }
	  if(all_options=='i'+'l'){
	  longFormatwithInode(show_hidden);
	  }else if(all_options=='l'){
	  longFormat(show_hidden);
	  }else if(all_options=='i'){
	  normalFormatwithInode(show_hidden);
	  }else if(all_options==0){
	  normalFormat(show_hidden);
	  }
    }
  else
    {
      for (; optind < argc; optind++)
	{
	  printf ("\n");
	  printf ("%s:\n", argv[optind]);
	  dirptr = opendir (argv[optind]);
	  chdir (argv[optind]);
	  if(recursive=='R'){
	  
	  }
	  if(all_options=='i'+'l'){
	  longFormatwithInode(show_hidden);
	  }else if(all_options=='l'){
	  longFormat(show_hidden);
	  }else if(all_options=='i'){
	  normalFormatwithInode(show_hidden);
	  }else if(all_options==0){
	  normalFormat(show_hidden);
	  }

	}
    }

  return 0;
}
