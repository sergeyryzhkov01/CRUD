#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

#define SCREENW 60
#define SCREENH 40

typedef double (*TFun)(double);

double func_Y(double x);
double func_V(double x);
double func_S(double x);

TFun select_function(char **func_name);
void action_calc_value(TFun f, char *name);
void action_tabulate(TFun f, char *name);
void action_plot(double x0, double x1, TFun f);

int main() {
    setlocale(LC_ALL, "");

    int choice;
    TFun current_func = NULL;
    char *func_name = NULL;

    do {
        printf("\n========== ГЛАВНОЕ МЕНЮ ==========\n");
        printf("1. Вычислить значение (в точке)\n");
        printf("2. Табулировать (таблица значений)\n");
        printf("3. Выполнить операцию (График функции)\n");
        printf("4. Выход\n");
        printf("==================================\n");
        printf("Выберите пункт: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                current_func = select_function(&func_name);
                if (current_func) action_calc_value(current_func, func_name);
                break;

            case 2:
                current_func = select_function(&func_name);
                if (current_func) action_tabulate(current_func, func_name);
                break;

            case 3:
                current_func = select_function(&func_name);
                if (current_func) {
                    double a, b;
                    printf("Введите интервал для графика [a, b]: ");
                    scanf("%lf %lf", &a, &b);
                    action_plot(a, b, current_func);
                }
                break;

            case 4:
                printf("Выход из программы.\n");
                break;

            default:
                printf("Неверный пункт меню. Попробуйте снова.\n");
        }

    } while (choice != 4);

    return 0;
}

TFun select_function(char **name_ptr) {
    int key;
    printf("\n--- Выберите функцию ---\n");
    printf("1. Y(x) = ln|sqrt(x^3)|\n");
    printf("2. V(x) (Кусочная)\n");
    printf("3. S(x) (Ряд)\n");
    printf("Выбор > ");
    scanf("%d", &key);

    switch (key) {
        case 1: *name_ptr = "Y(x)"; return func_Y;
        case 2: *name_ptr = "V(x)"; return func_V;
        case 3: *name_ptr = "S(x)"; return func_S;
        default: printf("Ошибка выбора функции.\n"); return NULL;
    }
}

// --- Реализация действий ---

// 1. Вычисление значения в точке
void action_calc_value(TFun f, char *name) {
    double x, y;
    printf("Введите значение x: ");
    scanf("%lf", &x);
    y = f(x);
    if (isnan(y))
        printf("Результат %s при x=%.2f: Не определен (NAN)\n", name, x);
    else
        printf("Результат %s при x=%.2f: %.6f\n", name, x, y);
}

// 2. Табуляция
void action_tabulate(TFun f, char *name) {
    double a, b, h;
    printf("Введите a, b, h: ");
    scanf("%lf %lf %lf", &a, &b, &h);

    printf("\nТабуляция %s:\n", name);
    printf("---------------------------\n");
    printf("|    x     |      y       |\n");
    printf("---------------------------\n");
    for (double x = a; x <= b + h/1000.0; x += h) {
        double y = f(x);
        if (isnan(y)) printf("| %8.4f |      -       |\n", x);
        else          printf("| %8.4f | %12.6f |\n", x, y);
    }
    printf("---------------------------\n");
}

// 3. Операция: Построение графика (использует двумерный массив (вариант 7))
void action_plot(double x0, double x1, TFun f) {
    char screen[SCREENW][SCREENH];
    double x, y[SCREENW];
    double ymin = 0, ymax = 0;
    double hx, hy;
    int i, j;
    int xz, yz;
    int first_valid = 1;

    hx = (x1 - x0) / (SCREENW - 1);

    for (i = 0, x = x0; i < SCREENW; ++i, x += hx) {
        y[i] = f(x);

        if (isnan(y[i])) continue;

        if (first_valid) {
            ymin = ymax = y[i];
            first_valid = 0;
        } else {
            if (y[i] < ymin) ymin = y[i];
            if (y[i] > ymax) ymax = y[i];
        }
    }

    if (first_valid) {
        printf("Функция не определена на данном отрезке.\n");
        return;
    }

    if (ymax - ymin < 0.000001) {
        hy = 1.0;
    } else {
        hy = (ymax - ymin) / (SCREENH - 1);
    }

    yz = (int)floor(ymax / hy + 0.5);
    xz = (int)floor((0. - x0) / hx + 0.5);

    for (j = 0; j < SCREENH; ++j) {
        for (i = 0; i < SCREENW; ++i) {
            if (j == yz && i == xz) screen[i][j] = '+';
            else if (j == yz)       screen[i][j] = '-';
            else if (i == xz)       screen[i][j] = '|';
            else                    screen[i][j] = ' ';
        }
    }

    for (i = 0; i < SCREENW; ++i) {
        if (isnan(y[i])) continue;

        j = (int)floor((ymax - y[i]) / hy + 0.5);

        if (j >= 0 && j < SCREENH) {
            screen[i][j] = '*';
        }
    }

    printf("\nГрафик функции (ось Y направлена вверх ^):\n");
    printf("Max Y: %.4f\n", ymax);
    for (j = 0; j < SCREENH; ++j) {
        for (i = 0; i < SCREENW; ++i) putchar(screen[i][j]);
        putchar('\n');
    }
    printf("Min Y: %.4f\n", ymin);
}

double func_Y(double x) {
    if (x <= 0) return NAN;
    return log(fabs(sqrt(pow(x, 3))));
}

double func_V(double x) {
    if (x > 1) {
        double arg = pow(x, 2) - 1;
        if (tan(arg) < 0) return NAN;
        return sqrt(tan(arg));
    } else if (x >= 0 && x <= 1) {
        return -2 * x;
    } else {
        return exp(cos(x));
    }
}

double func_S(double x) {
    if (x == -1) return NAN;
    double t = (x - 1) / (x + 1);
    return t + (1.0/3.0)*pow(t, 3) + (1.0/5.0)*pow(t, 5) + (1.0/7.0)*pow(t, 7);
}