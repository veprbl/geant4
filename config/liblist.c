/* $Id: liblist.c,v 1.2 1999-03-02 15:22:59 fbehner Exp $ */
/*
Given libname.map file and a list of .d dependency files liblist produces:
  a) with no options, a library list ordered according to the libname.map,
     giving a warning if conflicting dependencies are found in the .d files.
  b) with -l option, another libname.map, ordered according firstly to the
     original libname.map file, then reordered according to the dependencies
     found in the .d files.  This option is used for compiling the file
     libname.map from all the dependencies.
The .d files are arguments; the libname.map is from standard input.
Usage:
  liblist *.d < libname.map
Frank Behner, John Allison  13th February 1999.
*/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1000000
#define TRIGSIZE 1000
#define NLIBMAX 200

extern char *optarg;
extern int optind, opterr, optopt;

char** parsedir(char *directory,int *argc)
{
  DIR *actualdir;
  FILE *actualfile;
  struct dirent *entry; 
  char buffer[256];
  struct stat status;
  char **targv,**ptr,**phelp;
  int len,targc;

  /*Open the actual directory*/
  actualdir=opendir(directory);

  /*Loop over all entries */
  for(entry=readdir(actualdir);entry!=NULL;entry=readdir(actualdir))
    {
      /* Obtain the status information of that entry */
      if(stat(entry->d_name,&status)==0)
	{
	  if(S_ISDIR(status.st_mode))
	    {
	      /* a directory, so we are going recursive*/
	      targc=0;
	      ptr=parsedir(entry->d_name,&targc);
	      if(targc)
		{
		  phelp=targv;
		  targv=(char**) malloc((*argc+targc)*sizeof(char*));
		  memcpy(targv,phelp,*argc*sizeof(char*));
		  memcpy(&targv[*argc],ptr,targc*sizeof(char*));
		  *argc+=targc;
		  free(phelp);
		  free(ptr);
		}
	    }
	  else if(S_ISREG(status.st_mode))
	    {
	      /* a regular file is it .d? */
	      len=strlen(entry->d_name);
	      if(entry->d_name[len-2]=='.' && entry->d_name[len-1]=='d')
		{
		  phelp=targv;
		  targv=(char**) malloc((*argc+1)*sizeof(char*));
		  memcpy(targv,phelp,*argc*sizeof(char*));
		  targv[*argc]=strdup(entry->d_name);
		  (*argc)++;
		  free(phelp);
		}
	    }
	}
    }

  closedir(actualdir);

  return targv;
}
		
 
int main (int argc, char** argv) {

  char buffer[BUFSIZE],*bufferPtr,buf[256];
  char *ptr,*p,**pp,**pp1,**pp2,*directory=0;
  char **rargv;
  int i,dbegin=1,optl=0,swapping,c,rargc;
  FILE *fp;

  struct libmap_
    {
      char *lib;
      char *trigger;
      int used;
      char **uses;
      struct libmap_ *next;
  };

  struct libmap_ *libmap=0,*libmapPtr=0,*libmapPtr1=0,*libmapPtr2=0,
    *prevPtr1,*prevPtr2,*tmpp,*userLibmapPtr;

  /*
  if(argc==1)
    {
      printf("You have not supplied any command line arguments.\n");
      exit(1);
    }
  */

  while((c=getopt(argc,argv,"ld:"))!=EOF)
    {
      switch(c)
	{
	case 'l':
	  optl=1;
	  dbegin++;
	  break;
	case 'd':
	  directory=strdup(optarg);
	  break;
	}
    }
	  
  /*Adjust parameters after parsing options */

  if(optind<argc)
    {
      rargv=&argv[optind];
      rargc=argc-optind;
    }
  else
    {
      rargv=0;
      rargc=0;
    }

  if(directory)
    {
      if(rargc==0)
	rargv=parsedir(directory,&rargc);
      else
	{
	  fprintf(stderr,"If you specify a directory don't also specify files\n");
	  exit(-1);
	}
    }

  if(optl)fprintf(stderr,"Reading library name map file...\n");
  while (!feof(stdin))
    {
      /* Get library name... */
      gets(buffer);
      if(feof(stdin)) break;
      if(libmap)
	{
	  libmapPtr->next=(struct libmap_*) malloc(sizeof(struct libmap_));
	  libmapPtr=libmapPtr->next;
	}
      else /* First time through anchor libmapPtr to libmap. */
	{
	  libmap=(struct libmap_*) malloc(sizeof(struct libmap_));
	  libmapPtr=libmap;
	}
      ptr=strtok(buffer,":\n");
      libmapPtr->lib=strdup(ptr);
      libmapPtr->used=0;
      libmapPtr->uses=(char**)calloc(NLIBMAX,sizeof(char*));
      libmapPtr->next=0;

      /* If option -l not specified, fill uses list... */
      if(!optl)
	{
	  pp=libmapPtr->uses;
	  if(ptr)
	    {
	      ptr=strtok(NULL," \n");
	      while (ptr)
		{
		  *pp=strdup(ptr);
		  pp++;
		  ptr=strtok(NULL," \n");
		}
	    }
	}

      /* Get directory name... */
      gets(buffer);
      ptr=strtok(buffer,"/");
      while(strcmp (ptr,"source"))ptr=strtok(NULL,"/");
      ptr=strtok(NULL,"/");
      libmapPtr->trigger=(char*)malloc(TRIGSIZE);
      strcpy(libmapPtr->trigger,ptr);
      ptr=strtok(NULL,"/");
      while(strcmp(ptr,"GNUmakefile"))
	{
	  strcat(libmapPtr->trigger,"/");
	  strcat(libmapPtr->trigger,ptr);
	  ptr=strtok(NULL,"/");
	}
    }

  if(optl)fprintf(stderr,"Reading dependency files...\n");
  for(i=dbegin;i<rargc;i++)
    {
      fp=fopen(rargv[i],"r");
      fgets(buffer,BUFSIZE,fp);
      
      /* Clip target out of dependency file... */
      ptr=strtok(buffer,":");
      
      /* Look for a "user" library... */
      for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	{
	  if(strstr(ptr,libmapPtr->lib)) break;
	}
      if(libmapPtr)
	{
	  userLibmapPtr=libmapPtr;
	}
      else
	{
	  userLibmapPtr=0;
	}
      
      /* Look for a "used" library and add it to the "user" uses list... */
      do
	{
	  for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	    {
	      strcpy(buf,libmapPtr->trigger);
	      strcat(buf,"/include");
	      ptr=strstr(buffer,buf);
	      if(ptr)
		{
		  libmapPtr->used=1;
		  if(userLibmapPtr)
		    {
		      for(pp=userLibmapPtr->uses;*pp;pp++)
			{
			  if(strcmp(*pp,libmapPtr->lib)==0)break;
			}
		      if(!*pp)*pp=libmapPtr->lib;
		    }
		}
	    }
	  fgets(buffer,BUFSIZE,fp);
	} while(!feof(fp));
      fclose(fp);
    }

  if(optl) /* This option is used for compiling the file libname.map
	      from all the dependencies. */
    {
      fprintf(stderr,"Checking for circular dependencies...\n");
      for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	{
	  for(pp=libmapPtr->uses;*pp;pp++)
	    {
	      for(libmapPtr1=libmap;libmapPtr1!=libmapPtr;
		  libmapPtr1=libmapPtr1->next)
		{
		  if(strcmp(libmapPtr1->lib,*pp)==0)
		    {
		      for(pp1=libmapPtr1->uses;*pp1;pp1++)
			{
			  if(strcmp(*pp1,libmapPtr->lib)==0)break;
			}
		      if(*pp1)
			{
			  fprintf
			    (stderr,
			     "WARNING: %s and %s use each other.\n",
			     libmapPtr->lib,
			     libmapPtr1->lib);
			}
		    }
		  else 
		    {
		      /* Not right yet...
		      for(pp1=libmapPtr1->uses;*pp1;pp1++)
			{
			  for(libmapPtr0=libmap;libmapPtr0!=libmapPtr1;
			      libmapPtr0=libmapPtr0->next)
			    {
			      if(libmapPtr0==*pp)
				{
				  fprintf
				    (stderr,
				     "WARNING: triangular dependecy:\n"
				     "  %s uses %s uses %s uses %s.\n",
				     libmapPtr->lib,
				     libmapPtr1->lib,
				     libmapPtr0->lib,
				     libmapPtr->lib);
				}
			    }
			}
		      */
		    }
		}
	    }
	}

      fprintf(stderr,"Reordering according to dependencies...\n");
      do
	{
	  swapping=0;
	  prevPtr2=0;
	  for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	    {
	      for(pp=libmapPtr->uses;*pp;pp++)
		{
		  prevPtr1=0;
		  for(libmapPtr1=libmap;libmapPtr1!=libmapPtr;
		      libmapPtr1=libmapPtr1->next)
		    {
		      if(strcmp(libmapPtr1->lib,*pp)==0)
			{
			  /* Check that 1st doesn't use 2nd... */
			  for(pp1=libmapPtr1->uses;*pp1;pp1++)
			    {
			      if(strcmp(*pp1,libmapPtr->lib)==0)break;
			    }
			  if(!*pp1) /* If not... */
			    {
			      swapping=1;
			      /* Make previous of 1st now point to 2nd... */
			      if(prevPtr1)
				{
				  prevPtr1->next=libmapPtr;
				}
			      else
				{
				  libmap=libmapPtr;
				}
			      /* Make 2nd now point to what 1st did, unless
				 it's adjacent, in which case make it point
				 to 1st itself... */
			      tmpp=libmapPtr->next;
			      if(libmapPtr1->next==libmapPtr)
				{
				  libmapPtr->next=libmapPtr1;
				}
			      else
				{
				  libmapPtr->next=libmapPtr1->next;
				}
			      /* Make previous of 2nd point to 1st, unless
				 it's adjacent, in which case leave it... */
			      if(libmapPtr1->next!=libmapPtr)
				{
				  prevPtr2->next=libmapPtr1;
				}
			      /* Make 1st now point to what 2nd did... */
			      libmapPtr1->next=tmpp;
			      break;
			    }
			}
		      prevPtr1=libmapPtr1;
		    }
		  if(swapping)break;
		}
	      prevPtr2=libmapPtr;
	      if(swapping)break;
	    }
	}while(swapping);

      fprintf(stderr,"Writing new library map file...\n");
      for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	{
	  printf("%s:",libmapPtr->lib);
	  for(pp=libmapPtr->uses;*pp;pp++)
	    {
	      printf(" %s",*pp);
	    }
	  printf("\n");
	  printf("source/%s/GNUmakefile\n",libmapPtr->trigger);
	}
    }
  else
    {
      /* Add dependent libraries... */
      for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	{
	  if(libmapPtr->used)
	    {
	      for(pp=libmapPtr->uses;*pp;pp++)
		{
		  for(libmapPtr1=libmap;libmapPtr1;libmapPtr1=libmapPtr1->next)
		    {
		      if(strcmp(libmapPtr1->lib,*pp)==0)
			{
			  libmapPtr1->used=1;
			}
		    }
		}	      
	    }
	}

      /* Write out library list... */
      for(libmapPtr=libmap;libmapPtr;libmapPtr=libmapPtr->next)
	{
	  if(libmapPtr->used)
	    {
	      printf("-l%s ",libmapPtr->lib);
	    }
	}
    }
      
  exit(0);
  
}
