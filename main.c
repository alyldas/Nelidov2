#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct _session {
    struct tm dt;
    char cinema[CHAR_MAX];
    char film[CHAR_MAX];
    int hall;
    char type[CHAR_MAX];
} session;

int add() {
    FILE *fp = fopen("db", "wb");

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
    printf("Введите номер зала: ");
    scanf("%i", &session.hall);

    // type
    do {
        printf("Введите тип сеанса (3D, Imax, Обычный): ");
        scanf("%s", &session.type);
    } while (strcmp(session.type, "3D") &
             strcmp(session.type, "Imax") &
             strcmp(session.type, "Обычный"));

    // write to file
    fwrite(&session, sizeof(struct _session), 1, fp);

    fclose(fp);

    return EXIT_SUCCESS;
}

int edit(int n) {
    FILE *fp = fopen("db", "r+b");

    for (int i = 1; i <= n; i++) {
        if (!fread(&session, sizeof(struct _session), 1, fp)) {
            printf("Записи под таким номером не существует.\n");
            fclose(fp);
            return EXIT_SUCCESS;
        }
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
    do {
        printf("Тип сеанса (%s): ", session.type);
        scanf("%s", &session.type);
    } while (strcmp(session.type, "3D") &
             strcmp(session.type, "Imax") &
             strcmp(session.type, "Обычный"));


    // write to file
    fseek(fp, -(sizeof(struct _session)), SEEK_CUR);
    fwrite(&session, sizeof(struct _session), 1, fp);

    fclose(fp);

    return EXIT_SUCCESS;
}

int rem(int n) {
    FILE *fp = fopen("db", "rb");
    FILE *fp_ = fopen("db_", "wb");
    struct _session session_;

    int i = 1;
    while (fread(&session, sizeof(struct _session), 1, fp)) {
        if (i != n) {
            memcpy(&session_, &session, sizeof(struct _session));
            fwrite(&session_, sizeof(struct _session), 1, fp_);
        };
        i++;
//            printf("Записи под таким номером не существует.\n");
//            return EXIT_SUCCESS;
    }

    fclose(fp);
    fclose(fp_);

    remove("db");
    rename("db_", "db");

    return EXIT_SUCCESS;
}

int show() {
    FILE *fp = fopen("db", "rb");

    char date[CHAR_MAX], time[CHAR_MAX], i = 1;
    while (fread(&session, sizeof(struct _session), 1, fp)) {
        strftime(date, CHAR_MAX, "%d.%m.%y", &session.dt);
        strftime(time, CHAR_MAX, "%H:%M", &session.dt);
        printf("%i: %s, %s, %s, %s, зал №%i, %s\n",
           i++, date, time, session.cinema, session.film,
           session.hall, session.type);
    };

    fclose(fp);

    return EXIT_SUCCESS;
}

int main () {
    //fp = fopen("db", "a+b");

    //add();
    show();

    //fclose(fp);
	return EXIT_SUCCESS;
}
