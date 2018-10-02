#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

FILE *file;

struct _session {
    struct tm dt;
    char cinema[CHAR_MAX];
    char film[CHAR_MAX];
    int hall;
    char type[CHAR_MAX];
} session;

int add() {
    // date
    char date[CHAR_MAX];
    do {
        printf("Введите дату начала сеанса (ДД:ММ:ГГ): ");
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
    fwrite(&session, sizeof(struct _session), 1, file);

    return EXIT_SUCCESS;
}

int show() {
    char dt[CHAR_MAX], i = 1;
    while (fread(&session, sizeof(struct _session), 1, file)) {
            strftime(dt, CHAR_MAX, "%d.%m.%y, %H:%M", &session.dt);
            printf("%i: %s, %s, %s, зал №%i, %s\n",
                   i++, dt, session.cinema, session.film,
                   session.hall, session.type);
    };
}

int main () {
    file = fopen("db", "a+b");

    //add();
    show();

    fclose(file);
	return EXIT_SUCCESS;
}
