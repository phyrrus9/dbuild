#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

typedef struct
{
    WINDOW *w;
    int height;
    int width;
    int xpos;
    int ypos;
} window;

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

int main(int argc, const char * argv[])
{
    //system("pwd");
    //exit(1);
    initscr(); //initialize the main window
    refresh();
    noecho();
    window mainwin;
    WINDOW *status;
    if (COLS < 150)
    {
        endwin();
        printf("ERROR!, TERMINAL NOT WIDE ENOUGH!\n");
        exit(-1);
    }
    window_init(mainwin, 11, 150, "Debian Package Builder");
    status = newwin(1, COLS, LINES - 1, 0);
    wprintw(mainwin.w, "Name: ");
    wrefresh(mainwin.w);
    char c = 0;
    int i = 0;
    
    
    wclear(status);
    wprintw(status, "Getting name...");
    wrefresh(status);
    
    char *pkgname = new char[50];
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
        pkgname[i] = c;
        i++;
    }
    pkgname[i - 1] = '\0';
    
    wclear(status);
    wprintw(status, "Getting author...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Author: ");
    wrefresh(mainwin.w);
    char *pkgauthor = new char[50];
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
        pkgauthor[i] = c;
        i++;
    }
    pkgauthor[i - 1] = '\0';
    
    wclear(status);
    wprintw(status, "Getting description...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Description: ");
    wrefresh(mainwin.w);
    char *pkgdesc = new char[150];
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
        pkgdesc[i] = c;
        i++;
    }
    pkgdesc[i - 1] = '\0';
    
    wclear(status);
    wprintw(status, "Getting dependencies...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Depends (or none): ");
    wrefresh(mainwin.w);
    char *pkgdep = new char[150];
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
        pkgdep[i] = c;
        i++;
    }
    pkgdep[i - 1] = '\0';
    
    wclear(status);
    wprintw(status, "Getting version...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Version: ");
    wrefresh(mainwin.w);
    char *pkgver = new char[15];
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
        pkgver[i] = c;
        i++;
    }
    pkgver[i - 1] = '\0';
    
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
    fclose(f);
    
    wclear(status);
    wprintw(status, "Prompting for build...");
    wrefresh(status);
    
    wprintw(mainwin.w, "Build package (1=yes, 0=no): ");
    wrefresh(mainwin.w);
    c = getch();
    
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