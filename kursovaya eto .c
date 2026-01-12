#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <locale.h>


double f(double x);
double fact2(int n);

void table(double a, double b, double step);
double solve(double y, double a, double b, double eps);
double derivative(double x);

double find_min(double a, double b, double step, double* minx);
double find_max(double a, double b, double step, double* maxx);

int table_to_file(double a, double b, double step, const char* filename);
int table_from_file(const char* filename);


int main() {
    setlocale(LC_ALL, "RUS");

    int choice;

    do {
        printf("\n====== Меню ======\n");
        printf("1. Значение функции в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Минимум или максимум на отрезке\n");
        printf("4. Найти x по Y\n");
        printf("5. Производная в точке\n");
        printf("6. Записать таблицу в файл\n");
        printf("7. Прочитать таблицу из файла\n");
        printf("0. Выход\n ");

        scanf("%d", &choice);

        switch (choice) {

        case 1: {
            double x;
            printf("x = ");
            scanf("%lf", &x);
            printf("f(%.5f) = %.6f\n", x, f(x));
            break;
        }

        case 2: {
            double a, b, step;
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);
            table(a, b, step);
            break;
        }

        case 3: {
            int subchoice;
            double a, b, step;
            double x_extremum, value;

            printf("\n1. Минимум\n");
            printf("2. Максимум\n");
            printf("Выбор: ");
            scanf("%d", &subchoice);

            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);

            if (subchoice == 1) {
                value = find_min(a, b, step, &x_extremum);
                printf("\nМинимум: f(%.5f) = %.6f\n", x_extremum, value);
            }
            else if (subchoice == 2) {
                value = find_max(a, b, step, &x_extremum);
                printf("\nМаксимум: f(%.5f) = %.6f\n", x_extremum, value);
            }
            else {
                printf("Неверный выбор!\n");
            }
            break;
        }

        case 4: {
            double y, a, b;
            printf("Y = ");
            scanf("%lf", &y);
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("x ~= %.6f\n", solve(y, a, b, 1e-6));
            break;
        }

        case 5: {
            double x;
            printf("x = ");
            scanf("%lf", &x);
            printf("f'(%.5f) = %.6f\n", x, derivative(x));
            break;
        }

        case 6: {
            char filename[100];
            double a, b, step;

            printf("Имя файла: ");
            scanf("%s", filename);
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);

            if (table_to_file(a, b, step, filename) != 0)
                printf("Ошибка записи файла!\n");
            else
                printf("Таблица записана в файл.\n");
            break;
        }

        case 7: {
            char filename[100];
            printf("Имя файла: ");
            scanf("%s", filename);

            if (table_from_file(filename) != 0)
                printf("Ошибка чтения файла!\n");
            break;
        }

        case 0:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный пункт меню!\n");
        }

    } while (choice != 0);

    return 0;
}



double f(double x) {
    if (x < 1) {
        return exp(-fabs(x)) / atan(1 + x * x);
    }
    else if (x < 6) {
        return (x * x * cos(3 * x) * tanh(x)) /
            sqrt(1 + pow(x, 6));
    }
    else {
        double s = 0;
        for (int n = 0; n <= 4; n++) {
            s += pow(-1, n) * pow(x, 2 * n + 1) /
                (fact2(2 * n + 1) * (n + 3));
        }
        return s;
    }
}

double fact2(int n) {
    double r = 1;
    for (int i = n; i > 0; i -= 2)
        r *= i;
    return r;
}

double solve(double y, double a, double b, double eps) {
    for (double x = a; x <= b; x += eps) {
        if (fabs(f(x) - y) < eps)
            return x;
    }
    return NAN;
}

double derivative(double x) {
    double h = 1e-6;
    return (f(x + h) - f(x)) / h;
}

void table(double a, double b, double step) {
    printf("\n----------------------------------------\n");
    printf("|      x       |        f(x)           |\n");
    printf("----------------------------------------\n");

    for (double x = a; x <= b; x += step)
        printf("| %12.5f | %21.6f |\n", x, f(x));

    printf("----------------------------------------\n");
}

double find_min(double a, double b, double step, double* minx) {
    double minv = f(a);
    *minx = a;

    for (double x = a; x <= b; x += step) {
        double v = f(x);
        if (v < minv) {
            minv = v;
            *minx = x;
        }
    }
    return minv;
}

double find_max(double a, double b, double step, double* maxx) {
    double maxv = f(a);
    *maxx = a;

    for (double x = a; x <= b; x += step) {
        double v = f(x);
        if (v > maxv) {
            maxv = v;
            *maxx = x;
        }
    }
    return maxv;
}

int table_to_file(double a, double b, double step, const char* filename) {
    FILE* fptr = fopen(filename, "w");
    if (!fptr)
        return 1;

    for (double x = a; x <= b; x += step) {
        fprintf(fptr, "%.6f %.6f\n", x, f(x));
    }

    fclose(fptr);
    return 0;
}

int table_from_file(const char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (!fptr)
        return 1;

    double x, y;

    printf("\n----------------------------------------\n");
    printf("|      x       |        f(x)           |\n");
    printf("----------------------------------------\n");

    while (fscanf(fptr, "%lf %lf", &x, &y) == 2) {
        printf("| %12.5f | %21.6f |\n", x, y);
    }

    printf("----------------------------------------\n");

    fclose(fptr);
    return 0;
}
