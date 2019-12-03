#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#pragma pack(push, 1)   // Выравнивание структуры под 1 байт, т.е. переменные будут идти друг за другом
typedef struct {
    unsigned char formatType;       // Тип формата, должен быть 255
    char satName[13];               // Название
    unsigned int satId;             // Id
    unsigned int revNum;            // Номер витка на орбите
    unsigned short year;            // Год начала приёма
    unsigned short day;             // День года
    unsigned int dayTime;           // Время с начала дня (в миллисекундах)
    unsigned short o_year;          /// Резерв
    unsigned short o_day;           /// Резерв
    unsigned int o_dayTime;         /// Резерв
    unsigned char reserved[22];     // Сервисные
    unsigned char receiver[2];      /// Резерв / Приёмник ??
    unsigned char dataType[2];      // Тип данных

    // Считано Байт: 64
    // Сдвиг: 64

    unsigned int processLevel;
    unsigned short channel;
    short maxPixelValue;             /// Возможно без unsigned
    unsigned short projType;

    // 74 Байт
    unsigned short scanNum;         /// Кол-во строк
    unsigned short pixNum;          /// Кол-во пикселей в строке

    // 78 Байт
    float lat;
    float lon;
    float latSize;
    float lonSize;
    float latRes;
    float lonRes;
    double ka;
    double kb;
    char channelName[10];
    unsigned int NORADrevNum;
    unsigned short setNum;
    unsigned short ephemType;
    unsigned short NORADyear;
    double yearTime;
    double n0;
    double bstar;
    double i0;
    double raan;
    double e0;
    double w0;
    double m0;
    char dataName[27];              // Един с нижним
    char dataUnitsName[27];         // Един с верхним
    unsigned short corVersion;
    short orbitModelType;
    short corTime;
    double corRoll;
    double corPitch;
    double corYaw;
    unsigned short gravitModel;     // Един с нижним
    char spare[224];                // Един с верхним
} Head;
#pragma pack(pop)       // Завершение выравнивания

// Errors codes
enum errors {
    FILE_NOT_OPENED = 1
};

// Вывод помощи
void Help();                                    // TODO Дописать помощь

/// @Чтение

// Открытие файла
FILE *Open_file(char *file_name, char *mode);

// Считывание заголовочного файла
Head *Read_head(FILE *fin);

// Считывание пикселей
short **Read_pixels(FILE *fin, unsigned short lines, unsigned short pix_count);

/// @Запись

// Вывод заголовочного файла на экран
void Print_head(Head *head);

// Записывает заголовочный файл в файл
void Write_head(FILE *fout, Head *head);    // TODO Немного подправить вывод последнего поля space

// Записывает пиксели в файл
void Write_pixels(FILE *fout, short **pixels, unsigned short lines, unsigned short pix_count);

// Копирование файла
void Copy_photo(FILE *fin, FILE *fout, Head *head);

/// @Проверки

// Проверяет файл для считывания
int Check_head(char *file_name);

// Проверяет формат второго файла
int Format_check(char *file_name);

/// @Редактирование

// Закрашивает пиксель в заданный цвет
void Paint_pixel(short **pixels, int y, int x, short color);

// Закршивает линию в заданный цвет
void Paint_line();              //TODO

// Закрашивает горизонтальную линию в заданный цвет
void Paint_horizontal_line(short **pixels, int y, int x, int x_step, short color);

// Закрашивает вертикальную линию в заданный цвет
void Paint_vertical_line(short **pixels, int y, int x, int y_step, short color);

// Рисует контур прямоугольника с заданным цветом
void Paint_outline(short **pixels, int y, int x, int y_step, int x_step, short color);

// Рисует прямоигольник с заданным цветом
void Paint_rectangle(short **pixels, int y, int x, int y_step, int x_step, short color);

/// @Обработки

// Обработка вывода заголовочного файла в консоль
void Print(char *file_name);        // TODO пересмотреть

// Обработка записи в файл и копирования
void Write_Copy(char *file_name, char *copy_name);

// Исходя из аргументов запускает нужный режим
void Mode(char **args);             // TODO переписать

// Очищает массив пикселей
void Clear_pixels(short **pixels, unsigned short lines);

/// @Main
int main(int argc, char *argv[]) {
    switch (argc) {
        case 1:
            Help();
            break;
        case 2:
            Print(argv[1]);
            break;
        case 3:
            Write_Copy(argv[1], argv[2]);
            break;
        case 4:
            Mode(argv);
            break;
        default:
            printf("Entered too much arguments\n"
                   "Run the program without additional arguments to view the help");
            break;
    }

    return 0;
}

/// @Functions

void Help() {
    printf("The program takes 2 additional arguments:\n");
    printf("1) The path to the image file of the satellite (includes .pro)\n");
    printf("2) File name to copy satellite image (includes .pro) or"
           "text file (includes .txt), to which the contents will be written\n"
           "the header block of the input image");

}

/// @Чтение

FILE *Open_file(char *file_name, char *mode) {
    FILE *file = fopen(file_name, mode);
    if (file == NULL) {
        printf("Could not opened file (%s). Possible errors:\n"
               "1) File name entered incorrectly\n"
               "2) File not exist\n", file_name);
        exit(FILE_NOT_OPENED);
    }

    return file;
}

Head *Read_head(FILE *fin) {
    Head *head = (Head *) malloc(sizeof(Head));
    char *c = (char *) head;
    int g;
    for (int i = 0; i < 512; i++) {
        g = fgetc(fin);
        *c = (char) g;
        c++;
    }

    return head;
}

short **Read_pixels(FILE *fin, unsigned short lines, unsigned short pix_count) {
    short **pixels = (short **) malloc(lines * sizeof(short *));
    for (int i = 0; i < lines; i++)
        pixels[i] = (short *) malloc(pix_count * sizeof(short));

    for (int i = 0; i < lines; i++)
        fread(pixels[lines - i - 1], sizeof(short), pix_count, fin);

    return pixels;
}

/// @Запись

void Print_head(Head *head) {
    printf("formatType: %d\n", head->formatType);
    printf("satName: '%s'\n", head->satName);
    printf("satId: %d\n", head->satId);
    printf("revNum: %d\n", head->revNum);
    printf("year: %d\n", head->year);
    printf("day: %d\n", head->day);
    printf("dayTime: %d\n", head->dayTime);
    printf("o_year: %d\n", head->o_year);
    printf("o_day: %d\n", head->o_day);
    printf("o_datTime: %d\n", head->o_dayTime);

    printf("reserved: [");
    for (int i = 0; i < 22; i++)
        printf("%d ", head->reserved[i]);
    printf("]\n");

    printf("receiver: [%d %d]\n", head->receiver[0], head->receiver[1]);
    printf("dataType: [%d %d]\n", head->dataType[0], head->dataType[1]);

    /// 64 Байта

    printf("processLevel: %d\n", head->processLevel);
    printf("channel: %d\n", head->channel);
    printf("maxPixelValue: %d\n", head->maxPixelValue);
    printf("projType: %d\n", head->projType);
    printf("scanNum: %d\n", head->scanNum);
    printf("pixNum: %d\n", head->pixNum);
    printf("lat: %.3f\n", head->lat);
    printf("lon: %.3f\n", head->lon);
    printf("latSize: %.3f\n", head->latSize);
    printf("lonSize: %.15f\n", head->lonSize);
    printf("latRes: %.14f\n", head->latRes);
    printf("lonRes: %.14f\n", head->lonRes);
    printf("ka: %.3f\n", head->ka);
    printf("kb: %.3f\n", head->kb);
    printf("channelName: '%s'\n", head->channelName);
    printf("NORADrevNum: %d\n", head->NORADrevNum);
    printf("setNum: %d\n", head->setNum);
    printf("ephemType: %d\n", head->ephemType);
    printf("NORADyear: %d\n", head->NORADyear);
    printf("yearTime: %.8f\n", head->yearTime);
    printf("n0: %.17f\n", head->n0);
    printf("bstar: %0.9f\n", head->bstar);
    printf("i0: %.16f\n", head->i0);
    printf("raan: %.16f\n", head->raan);
    printf("e0: %.7f\n", head->e0);
    printf("w0: %.16f\n", head->w0);
    printf("m0: %.15f\n", head->m0);
    printf("dataName: '%s'\n", head->dataName);
    printf("dataUnitsName: '%s'\n", head->dataUnitsName);
    printf("corVersion: %d\n", head->corVersion);
    printf("orbitModelType: %d\n", head->orbitModelType);
    printf("corTime: %d\n", head->corTime);
    printf("corRoll: %.18f\n", head->corRoll);
    printf("corPitch: %.18f\n", head->corPitch);
    printf("corYaw: %.20f\n", head->corYaw);
    printf("gravitModel: %d\n", head->gravitModel);

    printf("spare: [");
    for (int i = 0; i < 224; ++i)
        printf("%d ", head->spare[i]);
    printf("]\n");

}

void Write_head(FILE *fout, Head *head) {
    fprintf(fout, "formatType: %d\n", head->formatType);
    fprintf(fout, "satName: '%s'\n", head->satName);
    fprintf(fout, "satId: %d\n", head->satId);
    fprintf(fout, "revNum: %d\n", head->revNum);
    fprintf(fout, "year: %d\n", head->year);
    fprintf(fout, "day: %d\n", head->day);
    fprintf(fout, "dayTime: %d\n", head->dayTime);
    fprintf(fout, "o_year: %d\n", head->o_year);
    fprintf(fout, "o_day: %d\n", head->o_day);
    fprintf(fout, "o_datTime: %d\n", head->o_dayTime);

    fprintf(fout, "reserved: [");
    for (int i = 0; i < 22; i++)
        fprintf(fout, "%d ", head->reserved[i]);
    fprintf(fout, "]\n");

    fprintf(fout, "receiver: [%d %d]\n", head->receiver[0], head->receiver[1]);
    fprintf(fout, "dataType: [%d %d]\n", head->dataType[0], head->dataType[1]);

    /// 64 Байта

    fprintf(fout, "processLevel: %d\n", head->processLevel);
    fprintf(fout, "channel: %d\n", head->channel);
    fprintf(fout, "maxPixelValue: %d\n", head->maxPixelValue);
    fprintf(fout, "projType: %d\n", head->projType);
    fprintf(fout, "scanNum: %d\n", head->scanNum);
    fprintf(fout, "pixNum: %d\n", head->pixNum);
    fprintf(fout, "lat: %.3f\n", head->lat);
    fprintf(fout, "lon: %.3f\n", head->lon);
    fprintf(fout, "latSize: %.3f\n", head->latSize);
    fprintf(fout, "lonSize: %.15f\n", head->lonSize);
    fprintf(fout, "latRes: %.14f\n", head->latRes);
    fprintf(fout, "lonRes: %.14f\n", head->lonRes);
    fprintf(fout, "ka: %.3f\n", head->ka);
    fprintf(fout, "kb: %.3f\n", head->kb);
    fprintf(fout, "channelName: '%s'\n", head->channelName);
    fprintf(fout, "NORADrevNum: %d\n", head->NORADrevNum);
    fprintf(fout, "setNum: %d\n", head->setNum);
    fprintf(fout, "ephemType: %d\n", head->ephemType);
    fprintf(fout, "NORADyear: %d\n", head->NORADyear);
    fprintf(fout, "yearTime: %.8f\n", head->yearTime);
    fprintf(fout, "n0: %.17f\n", head->n0);
    fprintf(fout, "bstar: %0.9f\n", head->bstar);
    fprintf(fout, "i0: %.16f\n", head->i0);
    fprintf(fout, "raan: %.16f\n", head->raan);
    fprintf(fout, "e0: %.7f\n", head->e0);
    fprintf(fout, "w0: %.16f\n", head->w0);
    fprintf(fout, "m0: %.15f\n", head->m0);
    fprintf(fout, "dataName: '%s'\n", head->dataName);
    fprintf(fout, "dataUnitsName: '%s'\n", head->dataUnitsName);
    fprintf(fout, "corVersion: %d\n", head->corVersion);
    fprintf(fout, "orbitModelType: %d\n", head->orbitModelType);
    fprintf(fout, "corTime: %d\n", head->corTime);
    fprintf(fout, "corRoll: %.18f\n", head->corRoll);
    fprintf(fout, "corPitch: %.18f\n", head->corPitch);
    fprintf(fout, "corYaw: %.20f\n", head->corYaw);
    fprintf(fout, "gravitModel: %d\n", head->gravitModel);

    fprintf(fout, "spare: [");
    for (int i = 0; i < 224; ++i)
        fprintf(fout, "%d ", head->spare[i]);
    fprintf(fout, "]\n");
}

void Write_pixels(FILE *fout, short **pixels, unsigned short lines, unsigned short pix_count) {
    for (int i = 0; i < lines; i++)
        fwrite(pixels[lines - i - 1], sizeof(short), pix_count, fout);
}

void Copy_photo(FILE *fin, FILE *fout, Head *head) {
    // Копирование головы
    fwrite(head, sizeof(char), 512, fout);

    // Копирование пикселей
    short **pixels = Read_pixels(fin, head->scanNum, head->pixNum);
    Write_pixels(fout, pixels, head->scanNum, head->pixNum);

    fclose(fin);
    fclose(fout);
    Clear_pixels(pixels, head->scanNum);
}

/// @Проверки

int Check_head(char *file_name) {
    unsigned long long len = strlen(file_name);
    char check[] = ".pro\0";

    for (unsigned long long i = len - 4; i <= len; i++)
        if (file_name[i] != check[i + 4 - len]) {
//            printf("Incorrect format file .pro\n");
            return 0;
        }

    return 1;
}

int Format_check(char *file_name) {

    /*char txt[] = ".txt\0";
    char pro[] = ".pro\0";
    int len = strlen(file_name);
    int c_txt = 0, c_pro = 0;

    for (int i = len - 4; i <= len; i++) {
        if (file_name[i] == txt[i + 4 - len])
            c_txt++;
        if (file_name[i] == pro[i + 4 - len])
            c_pro++;
    }

    if (c_txt == 5)
        return 1;
    else if (c_pro == 5)
        return 2;

    return 0;*/

    if (strstr(file_name, ".txt") != NULL)
        return 1;
    else if (strstr(file_name, ".pro") != NULL)
        return 2;
    return 0;
}

/// @Редактирование

void Paint_pixel(short **pixels, int y, int x, short color) {
    pixels[y][x] = color;
}

//void Paint_line(int **pixels, int y, int x, int y_step, int x_step, int color) {
//    for (int i = 0; i < y_step || i < x_step; i++) {
//        Paint_pixel(pixels, y, x, color);
//    }
//}

void Paint_horizontal_line(short **pixels, int y, int x, int x_step, short color) {
    for (int i = 0; i <= x_step; i++)
        Paint_pixel(pixels, y, x + i, color);
}

void Paint_vertical_line(short **pixels, int y, int x, int y_step, short color) {
    for (int i = 0; i <= y_step; i++)
        Paint_pixel(pixels, y + i, x, color);
}

void Paint_outline(short **pixels, int y, int x, int y_step, int x_step, short color) {
    Paint_horizontal_line(pixels, y, x, x_step, color);
    Paint_vertical_line(pixels, y, x, y_step, color);

    Paint_horizontal_line(pixels, y + y_step, x, x_step, color);
    Paint_vertical_line(pixels, y, x + x_step, y_step, color);
}

void Paint_rectangle(short **pixels, int y, int x, int y_step, int x_step, short color) {
    for (int i = 0; i <= y_step; i++)
        Paint_horizontal_line(pixels, y + i, x, x_step, color);
}

/// @Обработки

void Print(char *file_name) {
    if (Check_head(file_name)) {

        FILE *fin = Open_file(file_name, "rb");
        Head *head = Read_head(fin);
        Print_head(head);

        fclose(fin);
        free(head);
    } else
        printf("incorrect format file\n");
}

void Write_Copy(char *file_name, char *copy_name) {
    if (Check_head(file_name)) {
        FILE *fin = Open_file(file_name, "rb");
        FILE *fout = NULL;

        Head *head = Read_head(fin);

        switch (Format_check(copy_name)) {
            case 1:
                fout = Open_file(copy_name, "wt");
                Write_head(fout, head);
                printf("Recording was successful");
                break;
            case 2:
                fout = Open_file(copy_name, "wb");
                Copy_photo(fin, fout, head);
                printf("Copying was successful");
                break;
            default:
                printf("Entered incorrect format second file (format second file: .txt / .pro)");
                break;
        }

        free(head);
        fclose(fin);
        fclose(fout);
    }
}

void Mode(char **args) {
    int x, y, x_step, y_step;
    short color;

    FILE *fin, *fout;
    Head *head;
    short **pixels;
    unsigned short pix_count, lines;

    if (args[1][0] == '-') {
        switch (args[1][1]) {
            case 'o':
                printf("Enter start 'x'");
                scanf("%d", &x);

                printf("Enter start 'y'");
                scanf("%d", &y);

                printf("Enter the 'x' offset");
                scanf("%d", &x_step);

                printf("Enter the 'y' offset");
                scanf("%d", &y_step);

                printf("Enter color");
                scanf("%hd", &color);

                fin = Open_file(args[2], "rb");
                head = Read_head(fin);
                pix_count = head->pixNum;
                lines = head->scanNum;

                pixels = Read_pixels(fin, lines, pix_count);

                Paint_outline(pixels, y, x, y_step, x_step, color);

                fout = Open_file(args[3], "wb");
                fwrite(head, sizeof(char), 512, fout);
                Write_pixels(fout, pixels, lines, pix_count);

                printf("Outline has drawn");
                break;
            case 'r':
                printf("Enter start 'x'");
                scanf("%d", &x);

                printf("Enter start 'y'");
                scanf("%d", &y);

                printf("Enter the 'x' offset");
                scanf("%d", &x_step);

                printf("Enter the 'y' offset");
                scanf("%d", &y_step);

                printf("Enter color");
                scanf("%hd", &color);

                fin = Open_file(args[2], "rb");
                head = Read_head(fin);

                pix_count = head->pixNum;
                lines = head->scanNum;

                pixels = Read_pixels(fin, lines, pix_count);

                Paint_rectangle(pixels, y, x, y_step, x_step, color);

                fout = Open_file(args[3], "wb");
                fwrite(head, sizeof(char), 512, fout);
                Write_pixels(fout, pixels, lines, pix_count);

                printf("Rectangle has drawn");

                break;
        }
    }
}

void Clear_pixels(short **pixels, unsigned short lines) {
    for (int i = 0; i < lines; i++)
        free(pixels[i]);
    free(pixels);
}