#include <cstdlib>
#include <iostream>

#include <windows.h>
#include <locale.h>


enum DOS_COLORS {
        PRETO, AZUL, VERDE, CIANO ,VERMELHO, MAGENTA, MARRON,
        LIGHT_GRAY, DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN,
        LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE
        };


void teclado_portugues(void)
{
    setlocale(LC_ALL,"portuguese");
}

void gotoxy(int coluna, int linha)
{
        COORD point;
        point.X = coluna; point.Y = linha;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

//---------------------------------------------------

void textcolor(DOS_COLORS iColor)
{
        HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
        bufferInfo.wAttributes &= 0x00F0;
        SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= iColor);
}

// -------------------------------------------------------------------------

void backcolor (DOS_COLORS iColor)
{
        HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
        bufferInfo.wAttributes &= 0x000F;
        SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= (iColor << 4));
}

char wielkie(char ch)
{ //wielkie = uppercase
  //esta função converte (retorna) sempre
  //letra maiúscula

  if(ch >= 97 && ch <=122)
     return(ch-32);
  else if(ch >= 65 && ch<= 90)
    return(ch);
  else
    return('*');//em caso de erro.

}
