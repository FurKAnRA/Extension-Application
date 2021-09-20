#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

#define MAX_LINE_LENGTH 100
#define HEADER_LINE_COUNT 6
#define TARGET_FILE "veriler1.nkt"

struct alanlar
{
    char alan_tipi[MAX_LINE_LENGTH];
} alan;

struct noktalar
{
    int line_count;
} count;

struct sayi_degeri
{
    int deger;

} sayi;

struct three_member_array
{
    double x;
    double y;
    double z;

} coordinat_xyz[2000000];

struct six_member_array
{
    double x;
    double y;
    double z;
    int r;
    int g;
    int b;

} coordinat_xyzrgb[2000000];

struct coordinates
{
    double c_x;
    double c_y;
    double c_z;
    double c;
} coordinats[2000000];

struct maxmin
{
    double max_x;
    double max_y;
    double max_z;
    double min_x;
    double min_y;
    double min_z;
} maximini;

int control(FILE *veriler)
{
    fseek(veriler, 0, 0);
    int line_count = 0;
    int adet=0;
    char data[10000], line[1000];

    while (!feof(veriler))
    {
        char line_read_buffer[MAX_LINE_LENGTH]="";
        fgets(line_read_buffer, MAX_LINE_LENGTH, veriler);
        line_count++;

        if (line_count == 3)
        {
            strncpy(alan.alan_tipi, line_read_buffer, MAX_LINE_LENGTH);
            int ret_alan = value();

            if (ret_alan < 0)
            {
                printf("Alanlar tanimi dogru verilmemistir.\n");
                return ret_alan;
            }
            else
            {
                sayi.deger = ret_alan;
                printf(alan.alan_tipi);
            }
        }
        else if (line_count == 4)
        {
            sscanf(line_read_buffer, "%s%d", data, &adet);
        }
    }

    line_count -= HEADER_LINE_COUNT+1;

    if (adet != line_count + 1)
    {
        printf("Dosyada belirtilen nokta sayisi ve tespit edilen veri miktari farklidir.\nBelirtilen: %d | Tespit edilen: %d\n", adet, line_count+1);
        return -1;
    }
    else
    {
        count.line_count = adet;
        printf("Noktalar: %d\n", adet);
    }

    return 0;
}

int value()
{
    char data_type1[] = "ALANLAR x y z r g b\n";
    char data_type2[] = "ALANLAR x y z\n";

    if (strcmp(alan.alan_tipi, data_type1) == 0)
    {
        return 6;
    }
    else if (strcmp(alan.alan_tipi, data_type2) == 0)
    {
        return 3;
    }
    else
        return -1;
}

void nokta_kaydetme(d_name)
{
    FILE *veriler;
    veriler = fopen(d_name, "r");
    long line_count = 0, i = 0;
    char p;
    //coordinat_xyz *p;

    while (!feof(veriler))
    {
        char line_read_buffer[MAX_LINE_LENGTH];
        fgets(line_read_buffer, MAX_LINE_LENGTH, veriler);
        line_count++;

        if (line_count >= HEADER_LINE_COUNT && line_count < count.line_count + HEADER_LINE_COUNT)
        {
            // printf("line read buffer: %s | ", line_read_buffer);
            if (sayi.deger == 3)
            {
                int element_amount = sscanf(line_read_buffer, "%lf %lf %lf", &coordinat_xyz[i].x, &coordinat_xyz[i].y, &coordinat_xyz[i].z);
                //printf(" %d  %lf %lf %lf\n",i, coordinat_xyz[i].x, coordinat_xyz[i].y, coordinat_xyz[i].z);

                if (element_amount != sayi.deger)
                {
                    printf("%d. Satirda eksik nokta bilgisi vardir.\n", line_count - 5);
                }
            }
            else if (sayi.deger == 6)
            {
                int element_amount = sscanf(line_read_buffer, "%lf %lf %lf %d %d %d", &coordinat_xyzrgb[i].x, &coordinat_xyzrgb[i].y, &coordinat_xyzrgb[i].z, &coordinat_xyzrgb[i].r, &coordinat_xyzrgb[i].g, &coordinat_xyzrgb[i].b);

                if (element_amount != sayi.deger)
                {
                    printf("%d. Satirda eksik nokta bilgisi vardir.\n", line_count - 5);
                }
                else
                {
                    //printf("%d  %lf %lf %lf %d %d %d\n",i, coordinat_xyzrgb[i].x, coordinat_xyzrgb[i].y, coordinat_xyzrgb[i].z, coordinat_xyzrgb[i].r, coordinat_xyzrgb[i].g, coordinat_xyzrgb[i].b);
                }
            }
            i++;
        }
    }
}

void enuzak_enyakin(FILE *veriler,FILE *output)
{

    int i = 0, j;
    if(sayi.deger == 6)
    {
        for(i; i < count.line_count; i++)
        {
            coordinats[i].c_x = pow((coordinat_xyzrgb[i].x - coordinat_xyzrgb[i + 1].x), 2);
            coordinats[i].c_y = pow((coordinat_xyzrgb[i].y - coordinat_xyzrgb[i + 1].y), 2);
            coordinats[i].c_z = pow((coordinat_xyzrgb[i].z - coordinat_xyzrgb[i + 1].z), 2);
            coordinats[i].c   = sqrt(coordinats[i].c_x + coordinats[i].c_y + coordinats[i].c_z);
        }
        double max = coordinats[0].c;
        for(i = 1; i < count.line_count; i++)
        {
            if(coordinats[i].c > max)
            {
                max = coordinats[i].c;
                j = i + 1;
            }
        }

        fprintf(output,"\nBirbirine En uzak nokta koordinatlari\n%lf %lf %lf", coordinat_xyzrgb[j - 1].x, coordinat_xyzrgb[j - 1].y, coordinat_xyzrgb[j - 1].z);
        fprintf(output,"\n%lf %lf %lf\n", coordinat_xyzrgb[j].x, coordinat_xyzrgb[j].y, coordinat_xyzrgb[j].z);


        double min = coordinats[0].c;
        for(i = 1; i < count.line_count; i++)
        {
            if(coordinats[i].c < min)
            {
                min = coordinats[i].c;
                j = i + 1;

            }

        }
        fprintf(output,"\n\nBirbirine En yakin nokta koordinatlari\n\n%lf %lf %lf", coordinat_xyzrgb[j - 1].x, coordinat_xyzrgb[j-1].y, coordinat_xyzrgb[j - 1].z);
        fprintf(output,"\n%lf %lf %lf\n", coordinat_xyzrgb[j].x, coordinat_xyzrgb[j].y, coordinat_xyzrgb[j].z);
    }

    if(sayi.deger == 3)
    {
        for(i; i<count.line_count; i++)
        {
            coordinats[i].c_x = pow((coordinat_xyz[i].x - coordinat_xyz[i + 1].x), 2);
            coordinats[i].c_y = pow((coordinat_xyz[i].y - coordinat_xyz[i + 1].y), 2);
            coordinats[i].c_z = pow((coordinat_xyz[i].z - coordinat_xyz[i + 1].z), 2);
            coordinats[i].c = sqrt(coordinats[i].c_x + coordinats[i].c_y + coordinats[i].c_z);
            //printf("%d..Iki nokta arasi uzaklik: %lf\n",i+1,c[i]);
        }
        double max = coordinats[0].c;
        for(i = 1; i < count.line_count; i++)
        {
            if( coordinats[i].c > max)
            {
                max = coordinats[i].c;
                j = i + 1;

            }

        }

        fprintf(output,"\nEn uzak nokta koordinatlari\n\n%lf %lf %lf",coordinat_xyz[j - 1].x,coordinat_xyz[j - 1].y,coordinat_xyz[j-1].z);
        fprintf(output,"\n%lf %lf %lf\n\n", coordinat_xyz[j].x, coordinat_xyz[j].y, coordinat_xyz[j].z);

        double min = coordinats[0].c;
        for(i = 1; i < count.line_count; i++)
        {
            if(coordinats[i].c < min)
            {
                min = coordinats[i].c;
                j = i + 1;
            }
        }
        fprintf(output, "\nEn yakin nokta koordinatlari\n\n%lf %lf %lf", coordinat_xyz[j - 1].x, coordinat_xyz[j - 1].y, coordinat_xyz[j - 1].z);
        fprintf(output, "\n%lf %lf %lf\n\n", coordinat_xyz[j].x, coordinat_xyz[j].y, coordinat_xyz[j].z);

    }
}

void kup_okuma(FILE *veriler, FILE *output)
{
    int i;
    double max_x, min_x;
    double max_y, min_y;
    double max_z, min_z;

    if(sayi.deger == 6)
    {
        min_x = coordinat_xyzrgb[0].x;
        max_x = coordinat_xyzrgb[0].x;

        for(i = 0; i < count.line_count; i++)
        {
            if( coordinat_xyzrgb[i].x > max_x)
            {
                max_x = coordinat_xyzrgb[i].x;
            }
            else if (coordinat_xyzrgb[i].x < min_x)
            {
                min_x = coordinat_xyzrgb[i].x;
            }
        }
        fprintf(output, "max(x): %lf  min(x): %lf ", max_x, min_x);
        double uzunluk_x = max_x - min_x;
        fprintf(output, "max(x) - min(x)= %lf\n", uzunluk_x);

        double max_y, min_y;
        min_y = coordinat_xyzrgb[0].y;
        max_y = coordinat_xyzrgb[0].y;

        for(i = 0; i < count.line_count; i++)
        {
            if( coordinat_xyzrgb[i].y > max_y)
            {
                max_y = coordinat_xyzrgb[i].y;
            }
            else if (coordinat_xyzrgb[i].y < min_y)
            {
                min_y = coordinat_xyzrgb[i].y;
            }
        }
        fprintf(output, "max(y): %lf  min(y): %lf ", max_y, min_y);
        double uzunluk_y = max_y - min_y;
        fprintf(output, "max(y) - min(y)= %lf\n", uzunluk_y);

        double max_z, min_z;
        min_z = coordinat_xyzrgb[0].z;
        max_z = coordinat_xyzrgb[0].z;

        for(i = 0; i < count.line_count; i++)
        {
            if( coordinat_xyzrgb[i].z > max_z)
            {
                max_z = coordinat_xyzrgb[i].z;
            }
            else if (coordinat_xyzrgb[i].z < min_z)
            {
                min_z = coordinat_xyzrgb[i].z;
            }
        }
        fprintf(output, "max(z): %lf  min(z): %lf ", max_z, min_z);
        double uzunluk_z = max_z - min_z;
        fprintf(output, "max(z) - min(z)= %lf\n", uzunluk_z);

        double kupun_kenar_uzunlugu;
        if (uzunluk_x > uzunluk_y && uzunluk_x > uzunluk_z)
        {
            kupun_kenar_uzunlugu = uzunluk_x;
            /* fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_x);
             fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_x);
             fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_x);*/
            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_x, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_x, min_z + uzunluk_x);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y, min_z + uzunluk_x);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y + uzunluk_x, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y + uzunluk_x, min_z + uzunluk_x);
        }
        else if (uzunluk_y > uzunluk_x && uzunluk_y > uzunluk_z)
        {
            kupun_kenar_uzunlugu = uzunluk_y;
            /* fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_y);
             fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_y);
             fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_y);*/
            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y +uzunluk_y, min_z + uzunluk_y);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y, min_z + uzunluk_y);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y + uzunluk_y, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y +uzunluk_y, min_z + uzunluk_y);
        }
        else if (uzunluk_z > uzunluk_x && uzunluk_z > uzunluk_y)
        {
            kupun_kenar_uzunlugu = uzunluk_z;
            /*fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_z);
            fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_z);
            fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_z);*/
            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_z, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_z, min_z + uzunluk_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y, min_z + uzunluk_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y + uzunluk_z, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y + uzunluk_z, min_z + uzunluk_z);
        }

        fprintf(output, "\nTum noktalari icine alan kupun kenar uzunlugu = %lf\n", kupun_kenar_uzunlugu);
        maximini.max_x = max_x;
        maximini.max_y = max_y;
        maximini.max_z = max_z;
        maximini.min_x = min_x;
        maximini.min_y = min_y;
        maximini.min_z = min_z;

    }

    if(sayi.deger == 3)
    {
        min_x = coordinat_xyz[0].x;
        max_x = coordinat_xyz[0].x;

        for(i = 0; i < count.line_count; i++)
        {
            if(coordinat_xyz[i].x > max_x)
            {
                max_x = coordinat_xyz[i].x;
            }
            else if (coordinat_xyz[i].x < min_x)
            {
                min_x = coordinat_xyz[i].x;
            }
        }
        fprintf(output, "max(x): %lf  min(x): %lf ", max_x, min_x);
        double uzunluk_x = max_x - min_x;
        fprintf(output, "max(x) - min(x)= %lf\n", uzunluk_x);

        double max_y, min_y;
        min_y = coordinat_xyz[0].y;
        max_y = coordinat_xyz[0].y;

        for(i = 0; i < count.line_count; i++)
        {
            if( coordinat_xyz[i].y > max_y)
            {
                max_y = coordinat_xyz[i].y;
            }
            else if (coordinat_xyz[i].y < min_y)
            {
                min_y = coordinat_xyz[i].y;
            }
        }
        fprintf(output, "max(y): %lf  min(y): %lf ", max_y, min_y);
        double uzunluk_y = max_y - min_y;
        fprintf(output, "max(y) - min(y)= %lf\n", uzunluk_y);

        double max_z, min_z;
        min_z = coordinat_xyz[0].z;
        max_z = coordinat_xyz[0].z;

        for(i = 0; i < count.line_count; i++)
        {
            if( coordinat_xyz[i].z > max_z)
            {
                max_z = coordinat_xyz[i].z;
            }
            else if (coordinat_xyz[i].z < min_z)
            {
                min_z = coordinat_xyz[i].z;
            }
        }
        fprintf(output, "max(z): %lf  min(z): %lf ", max_z, min_z);
        double uzunluk_z = max_z - min_z;
        fprintf(output, "max(z) - min(z)= %lf\n", uzunluk_z);

        double kupun_kenar_uzunlugu;
        if (uzunluk_x > uzunluk_y && uzunluk_x > uzunluk_z)
        {
            kupun_kenar_uzunlugu = uzunluk_x;
            /*fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_x);
            fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_x);
            fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_x);*/

            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_x, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_x, min_z + uzunluk_x);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y, min_z + uzunluk_x);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y + uzunluk_x, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_x, min_y + uzunluk_x, min_z + uzunluk_x);
        }
        else if (uzunluk_y > uzunluk_x && uzunluk_y > uzunluk_z)
        {
            kupun_kenar_uzunlugu = uzunluk_y;
            /*fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_y);
            fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_y);
            fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_y);*/

            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y +uzunluk_y, min_z + uzunluk_y);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y, min_z + uzunluk_y);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y + uzunluk_y, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_y, min_y +uzunluk_y, min_z + uzunluk_y);
        }
        else if (uzunluk_z > uzunluk_x && uzunluk_z > uzunluk_y)
        {
            kupun_kenar_uzunlugu = uzunluk_z;
            /*fprintf(output, "\nKupun bir kenarinin X kordinatlari min= %lf max= %lf ",min_x, min_x + uzunluk_z);
            fprintf(output, "\nKupun bir kenarinin Y kordinatlari min= %lf max= %lf ",min_y, min_y + uzunluk_z);
            fprintf(output, "\nKupun bir kenarinin Z kordinatlari min= %lf max= %lf\n ",min_z, min_z + uzunluk_z);*/

            fprintf(output, "\nKoselerin koordinatlari\n");
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y, max_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_z, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x, min_y + uzunluk_z, min_z + uzunluk_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y, min_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y, min_z + uzunluk_z);
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y + uzunluk_z, min_z );
            fprintf(output, "\nKupun bir kosesinin koordinatlari = %lf %lf %lf", min_x + uzunluk_z, min_y + uzunluk_z, min_z + uzunluk_z);
        }

        fprintf(output, "\nTum noktalari icine alan kupun kenar uzunlugu = %lf\n", kupun_kenar_uzunlugu);
        maximini.max_x = max_x;
        maximini.max_y = max_y;
        maximini.max_z = max_z;
        maximini.min_x = min_x;
        maximini.min_y = min_y;
        maximini.min_z = min_z;

    }



}

void kure(FILE *veriler, FILE *output)
{
    int i;
    float yaricap;
    double merkez_x, merkez_y, merkez_z;
    double x1, x2, y1, y2, z1, z2;

    printf("Kurenin yaricapini giriniz=");
    scanf("%f", &yaricap);

    printf("\nKurenin merkez degerlerini giriniz=");
    scanf("%lf %lf %lf", &merkez_x, &merkez_y, &merkez_z);

    x1 = merkez_x + yaricap;
    x2 = merkez_x - yaricap;
    y1 = merkez_y + yaricap;
    y2 = merkez_y - yaricap;
    z1 = merkez_z + yaricap;
    z2 = merkez_z - yaricap;

    fprintf(output, "\nKurenin icinde bulunan noktalar:\n");

    for(i = 0; i < count.line_count; i++)
    {
        if(sayi.deger == 6)
        {
            if(coordinat_xyzrgb[i].x < x1 && coordinat_xyzrgb[i].x > x2 && coordinat_xyzrgb[i].y < y1 && coordinat_xyzrgb[i].y > y2 && coordinat_xyzrgb[i].z < z1 && coordinat_xyzrgb[i].z > z2)
            {
                fprintf(output,"%lf %lf %lf\n", coordinat_xyzrgb[i].x, coordinat_xyzrgb[i].y,coordinat_xyzrgb[i].z);
            }
        }
        if(sayi.deger == 3)
        {
            if(coordinat_xyz[i].x < x1 && coordinat_xyz[i].x > x2 && coordinat_xyz[i].y < y1 && coordinat_xyz[i].y > y2 && coordinat_xyz[i].z < z1 && coordinat_xyz[i].z > z2)
            {
                fprintf(output,"%lf %lf %lf\n", coordinat_xyz[i].x, coordinat_xyz[i].y, coordinat_xyz[i].z);
            }

        }
    }


}

void birbirlerineuzaklik(FILE *veriler, FILE *output)
{
    int i = 0, j;
    double temp = 0;
    fprintf(output, "\nBirbirlerine olan uzakliklari:\n");

    for(i = 0; i < count.line_count; i++)
    {
        if(sayi.deger == 6)
        {
            if(j > i)
            {
                for(j = 1; j < count.line_count; j++)
                {
                    temp = temp + (coordinats[j].c - coordinats[i].c);
                }
            }
        }
        if(sayi.deger == 3)
        {
            if(j > i)
            {
                for(j = 1; j < count.line_count; j++)
                {
                    temp = temp + (coordinats[j].c - coordinats[i].c);
                }
            }
        }
    }
    temp = temp / count.line_count;
    fprintf(output, "\n%lf", temp);
}

int main()
{

    FILE *veriler, *output;
    int deger, secim = 1;
    int i;
    char d;
    char out[20]="output.nkt";

    if (veriler == NULL)
    {
        printf("Dosya Okunamadi\n");
        return 1;
    }

    printf("\n1) Dosya Kontrolu\n");
    printf("2) En Yakin/Uzak Noktalar\n");
    printf("3) Kup\n");
    printf("4) Kure\n");
    printf("5) Nokta Uzakliklari\n");
    printf("6) Cikis\n");


    while(secim!=6)
    {
        printf("\n\nSeciminizi giriniz: ");
        scanf("%d", &secim);

        if(secim == 1)
        {
            i = 0;
            DIR *c;
            struct dirent *dire;
            c = opendir(".");
            if (c)
            {
                while ((dire = readdir(c)) != NULL)
                {
                    int length = strlen(dire->d_name);
                    if (strncmp(dire->d_name + length - 4, ".nkt", 4) == 0)
                    {
                        veriler = fopen(dire->d_name,"r");
                        printf("Hedef Dosya: %s\n",dire->d_name);
                        control(veriler);
                        nokta_kaydetme(dire->d_name);
                    }
                }
            }
        }

        if(secim==2)
        {
            i=0;
            DIR *d;
            struct dirent *direr;
            d = opendir(".");
            if (d)
            {
                while ((direr = readdir(d)) != NULL)
                {
                    int length = strlen(direr -> d_name);
                    if (strncmp(direr -> d_name + length - 4, ".nkt", 4) == 0)
                    {
                        // veriler =fopen(direr -> d_name,"r");
                        printf("Hedef Dosya: %s\n", direr -> d_name);
                        value(veriler);
                        //nokta_kaydetme(direr -> d_name);
                        strcat(direr -> d_name,out);
                        output = fopen(direr -> d_name,"w+");
                        enuzak_enyakin(veriler, output);
                    }
                }
            }
        }

        if(secim==3)
        {
            kup_okuma(veriler, output);
            printf("\n");
        }
        if(secim==4)
        {
            kure(veriler, output);
        }
        if(secim==5)
        {
            birbirlerineuzaklik(veriler, output);

        }
        if(secim==6)
        {
            break;
        }
    }

    fclose(veriler);
    fclose(output);
    return 0;


}
