#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
int main()
{

  int x_loop;
  int y_loop;
  char a_letter = 'a';
  initscr();
  move(5, 5);
  printw("%s", "Testing multiple windows");
  refresh();

  sleep(2);
  new_window_ptr = newwin( LINES-5, COLS-1,0, 0);
  box(new_window_ptr, '|', '-');

    scrollok(new_window_ptr,TRUE);


  wrefresh(new_window_ptr);
  sleep(2);
 

  sleep(2);
  wrefresh(new_window_ptr);
  sleep(2);
  touchwin(new_window_ptr);
  wrefresh(new_window_ptr);
  sleep(2);
  popup_window_ptr = newwin(4, COLS-1,LINES-4, 0);
  box(popup_window_ptr, '|', '-');
  
  wrefresh(popup_window_ptr);
  sleep(2);
  touchwin(new_window_ptr);
  wrefresh(new_window_ptr);
  sleep(2);
// wclear(new_window_ptr);
  wrefresh(new_window_ptr);
  char s[100];
  while(wgetstr(new_window_ptr,  s)!=-1){
  wgetstr(new_window_ptr,  s);
  wprintw(new_window_ptr,"\n%s\n" , s);
  }
  endwin();
  exit(EXIT_SUCCESS);
}