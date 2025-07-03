#include "teclado.hpp"
#include "stm.hpp"

void teclado() {
    initscr();              // Inicia o modo ncurses
    cbreak();               // Desativa o buffer de linha (não precisa pressionar Enter)
    noecho();               // Não exibe os caracteres digitados
    keypad(stdscr, TRUE);   // Habilita teclas especiais (como setas)

    printw("Pressione qualquer tecla. Pressione 'q' para sair.\n");
    refresh();

    while (true) {
        int ch = getch();   // Espera por uma tecla

        // Mostrar a tecla pressionada
        clear();
        printw("Controles:\n");
        printw("w - Aumentar Vx\n");
        printw("s - Diminuir Vx\n");
        printw("a - Aumentar Vy\n");
        printw("d - Diminuir Vy\n");
        printw("p - Parar\n");
        printw("Pressione um número (0-9) para definir o ID do robô.\n\n");
        printw("Você pressionou: %c (%d)\n", ch, ch);
        printw("Pressione 'q' para sair.");
        refresh();
        if(ch > 47 && ch < 58)
        {
            pct.Vx = 0.0;
            pct.Vy = 0.0;
            pct.Vang = 0.0;
            pct.id = ch - 48;

        }
        switch (ch) {
            case 119: // 'w'
                pct.Vx = 1.0;
                break;
            case 115: // 's'
                pct.Vx = -1.0;
                break;
            case 97: // 'a'
                pct.Vy = 1.0;
                break;
            case 100: // 'd'
                pct.Vy = -1.0;
                break;
            case 112: // 'p', no caso, parar
                pct.Vx = 0.0;
                pct.Vy = 0.0;
                pct.Vang = 0.0;
                break;
            default:
                break;
        }
        if (ch == 'q') {
            break;
        }
        printw("\nID atual: (%d)\n", pct.id);
        printw("Vx: %.2f, Vy: %.2f, Vang: %.2f\n", pct.Vx, pct.Vy, pct.Vang);
    }

    endwin();               // Encerra o modo ncurses
}