#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#include "ezxml.h"


int main() {

  ezxml_t bible = ezxml_parse_file("kjv.xml"),book,chap,vers;

  char *bname=NULL;
  size_t bnum=0;
  size_t cnum=0,vnum=0;
  char *text=NULL;

  size_t pbnum=0,pcnum=0;

  char folder[PATH_MAX];
  char url[PATH_MAX];

  FILE *fp;

  char *books[66];
  char line[256];
  size_t bctr=0;

/*
  fp=fopen("books","r");

  while(fgets(line,256,fp)) {
    char *p=strchr(line,'\n');
    if(p) *p='\0';
    books[bctr++]=strdup(line);
  }

  fclose(fp);
*/


  for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {
    for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {
      for (vers = ezxml_child(chap, "VERS"); vers; vers = vers->next) {

        bname =strdup(ezxml_attr(book,"bname"));
        bnum = atoi(ezxml_attr(book,"bnumber"));
        cnum = atoi(ezxml_attr(chap,"cnumber"));
        vnum = atoi(ezxml_attr(vers,"vnumber"));
        text = strdup(ezxml_txt(vers));

        strcpy(url,bname);
        char *p=NULL;
        while((p=strchr(url,' '))) {
          *p='_';
        }

        if(bnum!=pbnum) {

          if(bnum==1) {
            fp=fopen("bible/index.html","a");

            fprintf(fp,"<meta name='viewport' content='width=device-width, initial-scale=1.0' />");

            fprintf(fp,"<style>* {font-family: sans;}</style>");

            fprintf(fp,"<h1>King James Version</h1>");
            fprintf(fp,"<h2>Old Testament</h2>");

            fclose(fp);

          } else if(bnum==40) {

            fp=fopen("bible/index.html","a");

            fprintf(fp,"<h2>New Testament</h2>");
            fclose(fp);
          }

          sprintf(folder,"bible/%s",url);

          if (mkdir(folder, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
            printf("Error: %s\n", strerror(errno));
          }

          fp=fopen("bible/index.html","a");

          fprintf(fp,"<a href='%s/index.html'>%s</a><br/>",url,bname);

          fclose(fp);

        }



        if(bnum!=pbnum || cnum!=pcnum) {

          if(cnum==1) {
            sprintf(folder,"bible/%s/index.html",url);
            fp=fopen(folder,"a");

            fprintf(fp,"<meta name='viewport' content='width=device-width, initial-scale=1.0' />");

            fprintf(fp,"<style>* {font-family: sans;}</style>");

            fprintf(fp,"<h1>%s</h1>",bname);
            fclose(fp);
          }

          sprintf(folder,"bible/%s/index.html",url);
          fp=fopen(folder,"a");
          fprintf(fp,"<a href='Chapter_%zu.html'>Chapter %zu</a><br/>",cnum,cnum);
          fclose(fp);
        }


        sprintf(folder,"bible/%s/Chapter_%zu.html",url,cnum);

        fp=fopen(folder,"a");

        if(vnum==1) {

          fprintf(fp,"<meta name='viewport' content='width=device-width, initial-scale=1.0' />");

          fprintf(fp,"<style>* {font-family: sans;}\nbody,html {max-width: 400px;text-align: justify;}</style>");

          fprintf(fp,"<h1>%s</h1><h2>Chapter %zu</h2>",bname,cnum);
        }

        fprintf(fp,"%zu %s<br/><br/>",vnum,text);

        fclose(fp);

        free(bname);
        bname=NULL;

        free(text);
        text=NULL;

        pbnum=bnum;
        pcnum=cnum;

      }
    }
  }

  ezxml_free(book);

/*
  for(int i=0;i<bctr;i++) {
    free(books[i]);
  }

*/


  return 0;
}

