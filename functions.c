#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int add() {
    FILE *fp = fopen("db", "ab");

    struct sessions session;

    // date
    char date[CHAR_MAX];

    do {
        printf("Введите дату начала сеанса (ДД.ММ.ГГ): ");
        scanf("%s", date);
    } while (!strptime(date, "%d.%m.%y", &session.dt));

    // time
    char time[CHAR_MAX];
    do {
        printf("Введите время начала сеанса (ЧЧ:ММ): ");
        scanf("%s", time);
    } while (!strptime(time, "%H:%M", &session.dt));

    // cinema
    printf("Введите название кинотеатра: ");
    scanf("\n%[^\n]", &session.cinema);

    // film
    printf("Введите название фильма: ");
    scanf("\n%[^\n]", &session.film);

    // hall
    int hall;
    do {
        printf("Введите номер зала: ");
        scanf("%i", &hall);
    } while (isdigit(hall));
    session.hall = hall;
    // BUG: seg fault

    // type
    do {
        printf("Введите тип сеанса (3D, Imax, Обычный): ");
        scanf("%s", &session.type);
    } while (strcmp(session.type, "3D") &
             strcmp(session.type, "Imax") &
             strcmp(session.type, "Обычный"));

    // write to file

    fwrite(&session, sizeof(struct sessions), 1, fp);

    fclose(fp);

    return 0;
}

int edit() {
    FILE *fp = fopen("db", "r+b");

    struct sessions session;

    int n;
    printf("Номер записи для редактирования: ");
    scanf("%i", &n);

    if (n <= 0) {
            printf("Записи под таким номером не существует.\n");
            fclose(fp);
            return 0;
    }

    for (int i = 1; i <= n; i++)
        if (!fread(&session, sizeof(struct sessions), 1, fp)) {
            printf("Записи под таким номером не существует.\n");
            fclose(fp);
            return 0;
        }

    // date
    char date[CHAR_MAX];
    strftime(date, CHAR_MAX, "%d.%m.%y", &session.dt);
    do {
        printf("Дата начала сеанса (%s): ", date);
        scanf("%s", date);
    } while (!strptime(date, "%d.%m.%y", &session.dt));

    // time
    char time[CHAR_MAX];
    strftime(time, CHAR_MAX, "%H:%M", &session.dt);
    do {
        printf("Время начала сеанса (%s): ", time);
        scanf("%s", time);
    } while (!strptime(time, "%H:%M", &session.dt));

    // cinema
    printf("Название кинотеатра (%s): ", session.cinema);
    scanf("\n%[^\n]", &session.cinema);

    // film
    printf("Название фильма (%s): ", session.film);
    scanf("\n%[^\n]", &session.film);

    // hall
    printf("Номер зала (%i): ", session.hall);
    scanf("%i", &session.hall);

    // type
    char type[CHAR_MAX];
    do {
        printf("Тип сеанса (%s): ", session.type);
        scanf("%s", &type);
    } while (strcmp(type, "3D") &
             strcmp(type, "Imax") &
             strcmp(type, "Обычный"));
    strcpy(session.type, type);

    // write to file

    fseek(fp, -(sizeof(struct sessions)), SEEK_CUR);
    fwrite(&session, sizeof(struct sessions), 1, fp);

    fclose(fp);

    return 0;
}

int rem() {
    FILE *fp = fopen("db", "rb");
    FILE *fp_ = fopen("db_", "wb");

    struct sessions session, Session;

    int n;
    printf("Номер записи для удаления: ");
    scanf("%i", &n);

    if (n <= 0) {
            printf("Записи под таким номером не существует.\n");
            fclose(fp);
            fclose(fp_);
            return 0;
    }

    for (int i = 1; i <= n; i++)
        if (!fread(&session, sizeof(struct sessions), 1, fp)) {
            printf("Записи под таким номером не существует.\n");
            fclose(fp);
            fclose(fp_);
            return 0;
        }

    fseek(fp, 0, SEEK_SET);

    int c = 1;
    while (fread(&session, sizeof(struct sessions), 1, fp)) {
        if (c != n) {
            memcpy(&Session, &session, sizeof(struct sessions));
            fwrite(&Session, sizeof(struct sessions), 1, fp_);
        };
        c++;
    }

    fclose(fp);
    fclose(fp_);

    remove("db");
    rename("db_", "db");

    return 0;
}

int show() {
    FILE *fp = fopen("db", "rb");

    struct sessions session;

    int count = 0, i = 0;
    char date[CHAR_MAX], time[CHAR_MAX];

    while (fread(&session, sizeof(struct sessions), 1, fp)) {
        strftime(date, CHAR_MAX, "%d.%m.%y", &session.dt);
        strftime(time, CHAR_MAX, "%H:%M", &session.dt);
        printf("%i: %s, %s, %s, %s, зал №%i, %s\n",
           ++i, date, time, session.cinema, session.film,
           session.hall, session.type);
        count++;
    };

    if (!count)
        puts("Нет записей.");

    fclose(fp);

    return 0;
}

int menu() {
    int i = 0;
    while (1) {
        puts("1: Добавить");
        puts("2: Изменить");
        puts("3: Удалить");
        puts("4: Показать таблицу");
        puts("5: Выйти");
        scanf("%i", &i);
        switch (i) {
            case 1: add(); break;
            case 2: edit(); break;
            case 3: rem(); break;
            case 4: show(); break;
            case 5: return 0;
            default: puts("Введите номер пункта от 1 до 5.");
        }
    }

    return 0;
}
