# Лабораторная работа №22. Табуляция функций и указатели

## Условие задачи

Разработать программу на языке C, которая выполняет операции над тремя математическими функциями. Программа должна быть реализована с использованием **указателей на функции**, чтобы передавать выбранную математическую зависимость в универсальные процедуры (табуляция, построение графика).

**Функции:**
1.  **Y(x)** $= \ln|\sqrt{x^3}|$, при $x > 0$
2.  **V(x)** (Кусочная функция):
    *   $\sqrt{\text{tg}(x^2 - 1)}$, при $x > 1$
    *   $-2x$, при $0 \le x \le 1$
    *   $e^{\cos(x)}$, при $x < 0$
3.  **S(x)** (Сумма ряда):
    *   $S(x) = t + \frac{1}{3}t^3 + \frac{1}{5}t^5 + \frac{1}{7}t^7$, где $t = \frac{x - 1}{x + 1}$

**Требуемые операции (Меню):**
*   Вычислить значение функции в точке.
*   Табулировать функцию (вывести таблицу значений на отрезке).
*   Построить график (псевдографика символами в консоли).

---

## 1. Алгоритм и блок-схема

### Алгоритм выполнения программы

1.  **Начало**.
2.  **Объявить константы и типы:**
    *   `SCREENW = 60`, `SCREENH = 40` — размеры поля для графика.
    *   `typedef double (*TFun)(double)` — тип указателя на математическую функцию.
3.  **Инициализация:** Установить локаль (русский язык).
4.  **Главный цикл (do-while):**
    *   Вывести меню на экран (1. Значение, 2. Табуляция, 3. График, 4. Выход).
    *   Считать выбор пользователя -> `choice`.
5.  **Обработка выбора (switch choice):**
    *   **Если choice = 1, 2 или 3:**
        *   Вызвать подпрограмму выбора функции (`select_function`).
        *   Пользователь выбирает (1-Y, 2-V, 3-S).
        *   Получить указатель на функцию -> `current_func`.
    *   **В зависимости от choice выполнить действие:**
        *   **Case 1 (Значение):** Ввести `x`. Вычислить `res = current_func(x)`. Вывести `res`.
        *   **Case 2 (Табуляция):** Ввести границы `a, b` и шаг `h`. В цикле от `a` до `b` вызывать `current_func(x)` и печатать строку таблицы.
        *   **Case 3 (График):** 
            *   Ввести интервал `x0, x1`.
            *   Создать массив символов `screen[][]`.
            *   Найти `min` и `max` значения функции на отрезке.
            *   Масштабировать значения и заполнить массив точками `*` и осями координат.
            *   Вывести массив на экран.
    *   **Case 4 (Выход):** Завершить цикл.
6.  **Конец**.

### Блок-схема
`![Блок-схема алгоритма](flowchart.png)`

---

## 2. Реализация программы

Код программы на языке C:

```c
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
```
## 3. Результаты работы программы.
### Пример 1
```
========== ГЛАВНОЕ МЕНЮ ==========
1. Вычислить значение (в точке)
2. Табулировать (таблица значений)
3. Выполнить операцию (График функции)
4. Выход
==================================
Выберите пункт: 1

--- Выберите функцию ---
1. Y(x) = ln|sqrt(x^3)|
2. V(x) (Кусочная)
3. S(x) (Ряд)
Выбор > 2
Введите значение x: 5
Результат V(x) при x=5.00: Не определен (NAN)
```
### Пример 2
```
========== ГЛАВНОЕ МЕНЮ ==========
1. Вычислить значение (в точке)
2. Табулировать (таблица значений)
3. Выполнить операцию (График функции)
4. Выход
==================================
Выберите пункт: 2

--- Выберите функцию ---
1. Y(x) = ln|sqrt(x^3)|
2. V(x) (Кусочная)
3. S(x) (Ряд)
Выбор > 1
Введите a, b, h: 1 4 1

Табуляция Y(x):
---------------------------
|    x     |      y       |
---------------------------
|   1.0000 |     0.000000 |
|   2.0000 |     1.039721 |
|   3.0000 |     1.647918 |
|   4.0000 |     2.079442 |
---------------------------
```
### Пример 3
```
========== ГЛАВНОЕ МЕНЮ ==========
1. Вычислить значение (в точке)
2. Табулировать (таблица значений)
3. Выполнить операцию (График функции)
4. Выход
==================================
Выберите пункт: 3

--- Выберите функцию ---
1. Y(x) = ln|sqrt(x^3)|
2. V(x) (Кусочная)
3. S(x) (Ряд)
Выбор > 3
Введите интервал для графика [a, b]: 1 10

График функции (ось Y направлена вверх ^):
Max Y: 1.1091
                                                         ***
                                                     ****   
                                                 ****       
                                              ***           
                                          ****              
                                       ***                  
                                     **                     
                                  ***                       
                                **                          
                             ***                            
                           **                               
                          *                                 
                        **                                  
                      **                                    
                    **                                      
                   *                                        
                  *                                         
                **                                          
               *                                            
              *                                             
             *                                              
            *                                               
           *                                                
          *                                                 
         *                                                  
        *                                                   
       *                                                    
                                                            
      *                                                     
     *                                                      
                                                            
    *                                                       
   *                                                        
                                                            
  *                                                         
                                                            
                                                            
 *                                                          
                                                            
*-----------------------------------------------------------
Min Y: 0.0000
```
## 4. Информация о разработчике
### Рыжков С.М. бИЦ-252
