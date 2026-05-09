#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Personel ve Kullanicilar icin ortak struct yapisi:
typedef struct {
    char kullaniciAdi[30];
    char sifre[10];
} Kisi;

// Kayit islemini yapan fonksiyon:
void kayitOl(char *kayit) {
    Kisi yeniKisi;
    FILE *dosya = fopen(kayit, "a");
    
    if (dosya == NULL) {
        printf("Hata: %s dosyasi acilamadi!\n", kayit);
        return;
    }
    
    printf("\nYeni Kayit:\n");
    printf("Kullanici Adi: ");
    scanf("%s", yeniKisi.kullaniciAdi);
    
    printf("Sifre: ");
    scanf("%s", yeniKisi.sifre);
    
    // Verilerin dosyaya yazdırılması:
    fprintf(dosya, "%s %s\n", yeniKisi.kullaniciAdi, yeniKisi.sifre);
    
    printf("Kayit tamamlandi!\n");
}

// Giris islemini yapan fonksiyon:
void girisYap(char *kayit) {
    Kisi girilenKisi, okunanKisi;
    int girisBasarili = 0;
    
    FILE *dosya = fopen(kayit, "r");
    
    if (dosya == NULL) {
        printf("Hata: Kayit bulunamadi.\n", kayit);
        return;
    }
    
    printf("\nGiris Ekrani:\n");
    printf("Kullanici Adi: ");
    scanf("%s", girilenKisi.kullaniciAdi);
    
    printf("Sifre: ");
    scanf("%s", girilenKisi.sifre);
    system("cls");
    
    while (fscanf(dosya, "%s %s", okunanKisi.kullaniciAdi, okunanKisi.sifre) != EOF) {
        if (strcmp(girilenKisi.kullaniciAdi, okunanKisi.kullaniciAdi) == 0 && 
            strcmp(girilenKisi.sifre, okunanKisi.sifre) == 0) {
            girisBasarili = 1;
            break;
        }
    }

    if (girisBasarili)
        printf("Giris basarili.\n");
    else 
        printf("Hata: Kullanici adi veya sifre yanlis!\n");
    
}

/*
kitapKayit()
{

}

kitapAl()
{

}
*/

int main() {
    int anaSecim, altSecim;
    
    while (1) {
        printf("1. Kullanici Islemleri\n");
        printf("2. Personel Islemleri\n");
        printf("3. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &anaSecim);
        system("cls");

        if (anaSecim == 3) 
            break;

        switch (anaSecim) {
            case 1: // Kullanıcı İşlemleri Alt Menüsü
                printf("\nKULLANICI ISLEMLERI:\n");
                printf("1. Kayit Ol\n");
                printf("2. Giris Yap\n");
                printf("3. Kitap kirala\n");
                printf("4. Ust Menuye Don\n");
                printf("Seciminiz: ");
                scanf("%d", &altSecim);
                system("cls");

                if (altSecim == 1)
                    kayitOl("kullanici.txt");
                else if (altSecim == 2) 
                    girisYap("kullanici.txt");
                //else if (altSecim == 3)
                //   kitapAl();
                else {
                    printf("Ana menuye donuluyor...\n");
                    system("cls");
                }
                break;

            case 2: // Personel İşlemleri Alt Menüsü
                printf("\nPERSONEL ISLEMLERI:\n");
                printf("1. Personel Kaydi Olustur\n");
                printf("2. Personel Girisi Yap\n");
                printf("3. Kitap kayit\n");
                printf("4. Ust Menuye Don\n");
                printf("Seciminiz: ");
                scanf("%d", &altSecim);
                system("cls");

                if (altSecim == 1)
                    kayitOl("personel.txt");
                else if (altSecim == 2)
                    girisYap("personel.txt");
                //else if (altSecim == 3)
                //    kitapAl();
                else {
                    printf("Ana menuye donuluyor...\n");
                    system("cls");
                }
                break;

            default:
                printf("Hatali secim! Lutfen 1-3 arasi bir deger girin.\n");
                system("cls");
                break;
        }
    }
    
    return 0;
}