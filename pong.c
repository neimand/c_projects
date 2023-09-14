#include <stdio.h>

#define WIDTH 82 // Ширина арены. константа
#define HEIGHT 27 // Высота арены. константа
#define PLATFORM 12 // Начальное положение ракетки по y;
// Функция отвечающая за то, уходит ли ракетка за пределы игрового поля. Если уходят, то игра возвращает ракетку на максимальную возможную позицию по этому направлению.
int validLeft(int leftPlatf_y) {
    int valid = leftPlatf_y;
    // Левая ракетка
    if (leftPlatf_y < 1) valid = 1;
    if (leftPlatf_y > 22) valid = 23;
    return valid;
}
// Функция отвечающая за то, уходит ли ракетка за пределы игрового поля. Если уходят, то игра возвращает ракетку на максимальную возможную позицию по этому направлению.
int validRight(int rightPlatf_y) {
    int valid = rightPlatf_y;
    // Правая ракетка
    if (rightPlatf_y < 1) valid = 1;
    if (rightPlatf_y > 22) valid = 23;
    return valid;
}

//Функция определяющая то, что конкретно нарисовать в определенной точке. Возвращает статус, который в свою очередь используется в render()
int grid(int i, int j, int leftPlatf_y, int rightPlatf_y, int ball_x, int ball_y) {
    int status = 0;
    if (i == 0 || i == (HEIGHT - 1)) status++;
    if (j == 0) status += 2;
    if (j == 5 && (i >= leftPlatf_y && i <= (leftPlatf_y + 2))) status += 9;
    if (j == WIDTH - 6 && (i >= rightPlatf_y && i <= (rightPlatf_y + 2))) status += 9;
    if (j == (WIDTH - 1)) status += 4;
    if (j == WIDTH / 2) status += 11;
    if ((i == ball_y) && (j == ball_x)) status += 10;
    return status;
}
// Функция которая конкретно рисует выводя определенные вещи в зависимости от статуса переданного функцией grid()
void render(int leftPlatf_y, int rightPlatf_y, int ball_x, int ball_y) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int status = grid(i, j, leftPlatf_y, rightPlatf_y, ball_x, ball_y);
            switch (status) {
                case 1:
                    printf("/");
                    break;
                case 2:
                    printf("|");
                    break;
                case 3:
                    printf("#");
                    break;
                case 4:
                    printf("|\n");
                    break;
                case 5:
                    printf("#\n");
                    break;
                case 0:
                    printf(" ");
                    break;
                case 9:
                    printf("|");
                    break;
                case 10:
                    printf("*");
                    break;
                case 11:
                    printf("|");
                    break;
                case 12:
                    printf("/");
                    break;
                case 21:
                    printf("@");
                    break;
            }
        }
    }
}
// Функция отвечающая за ввод. Возвращает символ который был введён. Если после этого символа не был нажат энтер(10) то уходит в рекурсию вызывая сама себя пока пользователь не введёт разрешенный символ.
char input_helper() {
    char key;
    char enter;
    key = getchar();
    enter = getchar();
    if (enter != 10) {
        printf("\x1b[40;1HError; Use enter to drop your current input and then try once more.");
        printf("\x1b[1F");
        return input_helper();
    }
    printf("%c", key);
    return key;
}
// Функция отвечающая за рассмотрения ввода и принятия решения что делать игре в зависимости от этого.
int get_input(int turn) {
    char key;
    int ret = 0;
    key = input_helper();
    switch (turn) {
        case 1:
            if (key == ' ') ret = 0;
            if (key == 'Z' || key == 'z') ret = 1;
            if (key == 'A' || key == 'a') ret = -1;
            break;
        case -1:
            if (key == ' ') ret = 0;
            if (key == 'm' || key == 'M') ret = 2;
            if (key == 'k' || key == 'K') ret = -2;
            break;
        default:
            ret = 0;
    }
    return ret;
}
// Функция отвечающая за пропечатывание счёта и стрелки которая показывает чей прямо сейчас ход сверху. В зависимости от хода поворачивает стрелку в правильном направлении.
void announcer(int turn, int player1_score, int player2_score) {
    printf("\n\n\x1b[39CPONG\n");
    if (turn == 1) printf("\x1b[35C%d\x1b[4C<--\x1b[4C%d\n\n", player1_score, player2_score);
    if (turn == -1) printf("\x1b[35C%d\x1b[4C-->\x1b[4C%d\n\n", player1_score, player2_score);
    if (turn == 0) printf("\x1b[35C%d\x1b[11C%d\n\n", player1_score, player2_score);
}
// Функция отвечающая за то, чтобы напечатать справку по управлению и тд снизу.
void help() {
    printf(
        "Controls:\n A or Z buttons for the left player, K or M buttons for the right player.\nSpace works "
        "as a skip of turn for both players. \n To confirm "
        "your turn or skip it press Enter.\n");
}
// Функция отвечающая за прикосновение мяча по X к ракетке или к воротам. При условии что мяч трогает ракетку возвращается 1 если это левая ракетка или -1 если правая. При голе правого игрока возвращается 2, голе левого -2
int x_collision(int ball_x, int ball_y, int leftPlatf_y, int rightPlatf_y, int dir_x) {
    if (ball_x - 1 == 5 && dir_x == -1) {                                // Левый край
        if (ball_y >= (leftPlatf_y - 1) && ball_y <= leftPlatf_y + 3) {  // Проверка высоты мяча
            dir_x = 1;
        } else {
            dir_x = 2;
        }
    }
    if (ball_x + 1 == WIDTH - 6 && dir_x == 1) {                           // Правый край
        if (ball_y >= (rightPlatf_y - 1) && ball_y <= rightPlatf_y + 3) {  // Снова проверка высоты
            dir_x = -1;
        } else {
            dir_x = -2;
        }
    }
    return dir_x;
}
// Функция отвечающая за прикосновение мяча по Y к стенам. При прикосновении меняет направление мяча на противоположное
int y_collision(int ball_y, int dir_y) {
    if (ball_y - 1 == 0 && dir_y == -1) {
        dir_y = 1;
    }
    if (ball_y + 1 == 26 && dir_y == 1) {
        dir_y = -1;
    }
    return dir_y;
}
// Основная функция вызывающая остальные функции в своем теле при смене ходов.
void update(int leftPlatf_y, int rightPlatf_y, int ball_x, int ball_y) {
    int input = 1; // хак чтобы при первом входе в цикл инпут был инициализирован
    int turn = 1; // хак чтобы при первом входе в цикл ход был инициализирован и начинал правый игрок(перед первым использованием в цикле этот параметр умножается на -1 поэтому ходит первым правый)
    int score_left = 0; // счёт игрока слева
    int score_right = 0; // счёт игрока справа
    int dir_x = -1; // начальные направления шарика. он летит налево вверх.
    int dir_y = -1;
    printf("\x1b[J"); // очистка экрана
    while (score_left < 21 && score_right < 21) {
        printf("\x1b[1J"); // очистка экрана
        printf("\r\x1b[H"); // курсор возвращается к нулевому символу
        turn *= -1; // переход хода
        announcer(turn, score_left, score_right); // прорисовывается надпись сверху со счётом и стрелочкой показывающей ход
        rightPlatf_y = validRight(rightPlatf_y); //проверка ракеток на нахождение в валидных местах
        leftPlatf_y = validLeft(leftPlatf_y);
        dir_x = x_collision(ball_x, ball_y, leftPlatf_y, rightPlatf_y, dir_x); // проверка шарика на вхождение в ворота или на соударение с ракеткой
        if (dir_x == 2 || dir_x == -2) { // если гол в один из ворот
            ball_x = WIDTH / 2; //возвращает в начальное положение
            ball_y = HEIGHT / 2; // возвращает в начальное положение
            leftPlatf_y = PLATFORM; // возвращает в начальное положение 
            rightPlatf_y = PLATFORM; // возвращает в начальное положение
            if (dir_x == 2)  // прибавление счёта при голе
                score_right++;
            else
                score_left++;
            dir_x = (dir_x / 2); // хак с помощью которого после гола правильно определяется направление шарика при начале следующего раунда
            continue; // если произошёл гол то все остальные действия цикла пропускаются и начинается следующий цикл
        }
        dir_y = y_collision(ball_y, dir_y); // проверка шарика на столкновение с стенами
        ball_x = ball_x + (1 * dir_x); // изменение местонахождения шарика 
        ball_y = ball_y + (1 * dir_y);
        switch (input) { // движение платформ и общая отрисовка всего игрового поля 
            case 1:
                render(leftPlatf_y + 1, rightPlatf_y, ball_x, ball_y);
                leftPlatf_y++;
                break;
            case 2:
                render(leftPlatf_y, rightPlatf_y + 1, ball_x, ball_y);
                rightPlatf_y++;
                break;
            case -1:
                render(leftPlatf_y - 1, rightPlatf_y, ball_x, ball_y);
                leftPlatf_y--;
                break;
            case -2:
                render(leftPlatf_y, rightPlatf_y - 1, ball_x, ball_y);
                rightPlatf_y--;
                break;
            case 0:
                render(leftPlatf_y, rightPlatf_y, ball_x, ball_y);
                break;
        }
        help(); // вывод справки
        input = get_input(turn); // ввод пользователя
    }
    int result = score_left > score_right ? 1 : 2; // если счёт левого игрока больше правого возвращает единицу иначе 2
    printf("\x1b[1J\n\n\x1b[39CPLAYER %d WON! CONGRATULATIONS!", result); // пишет результаты игры
}
// Начальные параметры
void startup() {
    int leftPlatf_y = PLATFORM;
    int rightPlatf_y = PLATFORM;
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    update(leftPlatf_y, rightPlatf_y, ball_x, ball_y);
}

int main() {
    startup();
    return 0;
}