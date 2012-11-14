/**************************************************************
   Copyright © 2012 Ethan Laur (phyrrus9) <phyrrus9@gmail.com>
 dbuild, the debian dpkg control file writer. This application
 is a simple form using a few (3) ncurses windows to help you
 configure your debian control file and help you build a .deb
 installer file (very useful for mac os x, iPhone OS, and most
 linux builds. This program is open source, and you may use it
 in any way you choose. The one condition to this is that the
 source code of your program MUST include this block in order
 to explain to the developers what their rights are and how to
 use the source code they are given. This source code comes AS
 IS, without warranty. It is YOUR job to ensure that the code
 you distribute to your users is sufficient in protecting all
 of the vitals of their system. Below is a list of contributors
 followed by a changelog. You may add yourself to the list if
 you can add something to the changelog (that you changed).
 Contributors:
 Ethan Laur (phyrrus9) <phyrrus9@gmail.com>
 Changelog:
 VERSION   USER      CHANGE
 1.0       phyrrus9  Initial Commit
 1.0.1     phyrrus9  INTERNAL
 1.0.2     phyrrus9  Added section, homepage, and reduce code
 *************************************************************/
#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

#define VERSION "1.0.2"

typedef struct
{
    WINDOW *w;
    int height;
    int width;
    int xpos;
    int ypos;
} window;

window mainwin;
WINDOW *status;

void window_init(window &w, int h, int t, const char * str) //creats a centered window
{
    w.height = h;
    w.width = t;
    w.xpos = (LINES - h) / 2;
    w.ypos = (COLS - t) / 2;
    w.w = newwin(w.height, w.width, w.xpos, w.ypos);
    wclear(w.w);
    wprintw(w.w, "\n %s\n", str);
    box(w.w, 0, 0);
    wrefresh(w.w);
}

void prompt_field(const char * prompt, const char * userv, char * field)
{
    int i;
    char c;
    
    wclear(status);
    wprintw(status, "Getting %s...", prompt);
    wrefresh(status);
    
    wprintw(mainwin.w, "%s: ", userv);
    wrefresh(mainwin.w);
    c = 0;
    i = 0;
    while (c != '\n')
    {
        c = getch();
        if (c == 127)
        {
            if (i - 1 >= 0)
            {
                wprintw(mainwin.w, "\b \b");
                wrefresh(mainwin.w);
                i--;
            }
            continue;
        }
        wprintw(mainwin.w, "%c", c);
        wrefresh(mainwin.w);
        field[i] = c;
        i++;
    }
    field[i - 1] = '\0';
}

int main(int argc, const char * argv[])
{
    //system("pwd");
    //exit(1);
    initscr(); //initialize the main window
    refresh();
    noecho();
    if (COLS < 150)
    {
        endwin();
        printf("ERROR!, TERMINAL NOT WIDE ENOUGH!\n");
        exit(-1);
    }
    
    WINDOW *header;
    header = newwin(1, COLS, 0, COLS / 4);
    wprintw(header, "dbuild version %s copyright © 2012 phyrrus9 <phyrrus9@gmail.com> http://github.com/phyrrus9/dbuild", VERSION);
    wrefresh(header);
    
    window_init(mainwin, 14, 150, "Debian Package Builder");
    status = newwin(1, COLS, LINES - 1, 0);
    wrefresh(mainwin.w);
    
    char *pkgname = new char[50];
    char *pkgauthor = new char[50];
    char *pkgdesc = new char[150];
    char *pkgdep = new char[150];
    char *pkgver = new char[15];
    char *pkgsec = new char[15];
    char *pkghom = new char[50];
    
    prompt_field("name", "Name", pkgname);
    prompt_field("author", "Author", pkgauthor);
    prompt_field("description", "Description", pkgdesc);
    prompt_field("dependencies", "Depends (or none)", pkgdep);
    prompt_field("version", "Version", pkgver);
    prompt_field("section", "Section", pkgsec);
    prompt_field("homepage", "Homepage", pkghom);
    
    wclear(status);
    wprintw(status, "Creating directory...");
    wrefresh(status);
    
    system("mkdir -p DEBIAN");
    FILE *f;
    f = fopen("./DEBIAN/control", "w");
    if (f == NULL)
    {
        endwin();
        printf("ERROR!, FILE CAN NOT BE OPENED\n");
        exit(-2);
    }
    
    wclear(status);
    wprintw(status, "Writing file...");
    wrefresh(status);
    
    fprintf(f, "Package: %s\n", pkgname);
    fprintf(f, "Maintainer: %s\n", pkgauthor);
    fprintf(f, "Architecture: all\n");
    fprintf(f, "Description: %s\n", pkgdesc);
    if (strcmp(pkgdep, "none") != 0)
        fprintf(f, "Depends: %s\n", pkgdep);
    fprintf(f, "Version: %s\n", pkgver);
    fprintf(f, "Section: %s\n", pkgsec);
    fprintf(f, "Homepage: %s\n", pkghom);
    
    fclose(f);
    
    wclear(status);
    wprintw(status, "Prompting for build...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Build package (1=yes, 0=no): ");
    wrefresh(mainwin.w);
    char c = getch();
    
    if (c == '1')
    {
        
        wclear(status);
        wprintw(status, "Building package...");
        wrefresh(status);
        
        char *path = new char[50];
        char *cmd = new char[75];
        wprintw(mainwin.w, "\nFolder: ");
        wrefresh(mainwin.w);
        c = 0;
        int i = 0;
        while (c != '\n')
        {
            c = getch();
            if (c == 127)
            {
                if (i - 1 >= 0)
                {
                    wprintw(mainwin.w, "\b \b");
                    wrefresh(mainwin.w);
                    i--;
                }
                continue;
            }
            wprintw(mainwin.w, "%c", c);
            wrefresh(mainwin.w);
            path[i] = c;
            i++;
        }
        path[i - 1] = '\0';
        
        strcpy(cmd, "");
        strcat(cmd, "dpkg -b ");
        strcat(cmd, path);
        chdir("../");
        system(cmd);
    }
    
    wclear(status);
    wprintw(status, "Waiting...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Operation complete. Press any key to exit.");
    wrefresh(mainwin.w);
    
    getch();
    
    endwin(); //exit ncurses
    
}