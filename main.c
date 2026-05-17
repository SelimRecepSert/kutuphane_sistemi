#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Personel ve Kullanicilar icin ortak struct yapisi:
typedef struct {
    char kullaniciAdi[30];
    char sifre[10];
    char hex_sifre[30];
} Kisi;

// Kitaplar icin struct yapisi:
typedef struct {
    int id;
    char ad[50];
    char yazar[50];
    int durum; // 1 = Rafta (Mevcut), 0 = Kiralandi
} Kitap;

// Kullanici ve kiraladigi kitap icin struct:
typedef struct {
    char kullaniciAdi[30];
    int kitapId;
    char kitapAd[50];
} KullaniciKitap;

//sifreleme fonksiyonu(XOR)
void sifreleme(char normalSifre[10],char HexSifre[30]){
    int i=0;
    int j=0;
    int key=5;

while (normalSifre[i]!='\0')
{
    int sifrex=normalSifre[i] ^ key;         //burada sifreyi xor'layıp int değişkeni olarak tutuyoruz 
    //cünkü bazı space ve bosuklarda scanf okuma atsı verebiliyor bunun nüne gecmek icin aslında
    sprintf(&HexSifre[j], "%02x", sifrex);
    i++;
    j+=2;
}
HexSifre[j] = '\0';
}
//Sifreyi cözdügümz kisim
void sifreCoz(char normalSifre[10],char HexSifre[30]){

    int i= 0; 
    int j = 0; 
    int key= 5;
        
    // Şifreli metin bitene kadar döngü
    while (HexSifre[j] != '\0') {
        int sayi;
        sscanf(&HexSifre[j], "%02x", &sayi); 
            
        // Çıkan sayıyı anahtarla tekrar XOR'la ve char dizigeri çevir
        normalSifre[i] = sayi ^ key;
        j+= 2;
        i++;
        }
        
        
        normalSifre[i] = '\0';       //bu önceki çöp değerleri kullanmıyacağımız kısımlar icin.
    }

// Kucuk harfe cevirme fonksiyonu:
void kucukHarfCevir(char *kaynak, char *hedef) {
    int i = 0;
    while (kaynak[i] != '\0') {
        hedef[i] = tolower(kaynak[i]);
        i++;
    }
    hedef[i] = '\0';
}

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

    sifreleme(yeniKisi.sifre,yeniKisi.hex_sifre);
    
    // Kullanıcı verilerinin dosyaya yazdirilmasi:
    fprintf(dosya, "%s %s\n", yeniKisi.kullaniciAdi, yeniKisi.hex_sifre);
    
    printf("Kayit tamamlandi!\n");

    fclose(dosya);
}

// Giris islemini yapan fonksiyon 
int girisYap(char *kayit, char *aktifKullanici) {
    Kisi girilenKisi, okunanKisi;
    FILE *dosya = fopen(kayit, "r");
    
    int girisBasarili = 0;

    if (dosya == NULL) {
        printf("Hata: Kayit bulunamadi veya dosya acilamadi.\n");
        return 0;
    }
    
    printf("\nGiris Ekrani:\n");
    printf("Kullanici Adi: ");
    scanf("%s", girilenKisi.kullaniciAdi);
    
    printf("Sifre: ");
    scanf("%s", girilenKisi.sifre);
    system("cls");
    
    // Dosyayi satir satir okuyup eslesme aranir:
    while (fscanf(dosya, "%s %s", okunanKisi.kullaniciAdi, okunanKisi.hex_sifre) != EOF) {
        sifreCoz(okunanKisi.sifre, okunanKisi.hex_sifre);
        if (strcmp(girilenKisi.kullaniciAdi, okunanKisi.kullaniciAdi) == 0 && strcmp(girilenKisi.sifre, okunanKisi.sifre) == 0) {
            girisBasarili = 1;
            strcpy(aktifKullanici, girilenKisi.kullaniciAdi); // Giris yapan kisinin adini sakla
            break;
        }
    }

    if (girisBasarili) {
        printf("Giris basarili. Hos geldiniz, %s!\n", aktifKullanici);
        fclose(dosya);
        return 1;
    } 
    else {
        printf("Hata: Kullanici adi veya sifre yanlis!\n");
        fclose(dosya);
        return 0;
    }
}

// Personelin sisteme yeni kitap eklemesi:
void kitapKayit() {
    Kitap yeniKitap;
    FILE *dosya = fopen("kitaplar.txt", "a");
    
    if (dosya == NULL) {
        printf("Hata: kitaplar.txt acilamadi!\n");
        return;
    }
    
    printf("\nYeni Kitap Ekleme:\n");
    printf("Kitap ID (Sayisal): ");
    scanf("%d", &yeniKitap.id);

    printf("Kitap adi giriniz: ");
    scanf("%s", yeniKitap.ad);
    
    printf("Yazar adi giriniz: ");
    scanf("%s", yeniKitap.yazar);
    
    yeniKitap.durum = 1;

    fprintf(dosya, "%d %s %s %d\n", yeniKitap.id, yeniKitap.ad, yeniKitap.yazar, yeniKitap.durum);
    
    printf("Kitap basariyla sisteme eklendi!\n");
    fclose(dosya);
}

// Kullanicinin sistemden kitap kiralamasi
void kitapKirala(char *aktifKullanici, char *arananKelime) {
    int kiralikKitapSayisi = 0;
    char okunanKullanici[30];
    int r_kitapId;
    char r_kitapAd[50];

    // Kullanicinin mevcut kiralik kitap ssayisii kontrol edilir:
    FILE *kkKontrol = fopen("kullanici_kitaplari.txt", "r");
    if (kkKontrol != NULL) {
        while (fscanf(kkKontrol, "%s %d %s", okunanKullanici, &r_kitapId, r_kitapAd) != EOF) {
            if (strcmp(okunanKullanici, aktifKullanici) == 0) {
                kiralikKitapSayisi++;
            }
        }
        fclose(kkKontrol);
    }

    if (kiralikKitapSayisi >= 3) {
        printf("\nHata: Ayni anda en fazla 3 kitap kiralayabilirsiniz! Lutfen once kitap iade ediniz.\n");
        system("pause");
        return;
   }

    Kitap k;
    int istenenId, bulundu = 0;
    char arananKucuk[50], adKucuk[50];

    kucukHarfCevir(arananKelime, arananKucuk);
    
    FILE *dosya = fopen("kitaplar.txt", "r");
    FILE *gecici = fopen("gecici.txt", "w");//Degisiklikler icin gecici dosya.
    
    if (dosya == NULL) {
        printf("Hata: Sistemde henuz kayitli kitap yok!\n");
        if(gecici != NULL) fclose(gecici);
        return;
    }

    printf("\nMevcut Kitaplar:\n");
    while (fscanf(dosya, "%d %s %s %d", &k.id, k.ad, k.yazar, &k.durum) != EOF) {
        if (k.durum == 1) {
            kucukHarfCevir(k.ad, adKucuk);
            if (strlen(arananKelime) == 0 || strstr(adKucuk, arananKucuk) != NULL) {
                printf("ID: %d | Kitap: %s | Yazar: %s\n", k.id, k.ad, k.yazar);
            }
        }   
    }
    
    rewind(dosya);//Dosya imleci basa alinir.

    printf("\nKiralamak istediginiz kitabin ID'sini girin (Iptal icin 0): ");
    scanf("%d", &istenenId);

    if (istenenId == 0) {
        printf("Kiralama iptal edildi.\n");
        fclose(dosya);
        fclose(gecici);
        remove("gecici.txt");
        return;
    }
    // Kiralanan kitabin durumu degistirilir ve kullaniciya kayıt edilir:
    while (fscanf(dosya, "%d %s %s %d", &k.id, k.ad, k.yazar, &k.durum) != EOF) {
        if (k.id == istenenId && k.durum == 1) {
            k.durum = 0; 
            bulundu = 1;

            system("cls");
            printf("\n'%s' adli kitabi basariyla kiraladiniz!\n", k.ad);

            KullaniciKitap kk;
            strcpy(kk.kullaniciAdi, aktifKullanici);

            kk.kitapId = k.id;
            strcpy(kk.kitapAd, k.ad);

            FILE *kullaniciKitapDosya = fopen("kullanici_kitaplari.txt", "a");
            if (kullaniciKitapDosya != NULL) {
                fprintf(kullaniciKitapDosya, "%s %d %s\n", kk.kullaniciAdi, kk.kitapId,k.ad);
                fclose(kullaniciKitapDosya);
            }
        }
        // Guncel bilgiler gecici dosyaya yazilir:
        fprintf(gecici, "%d %s %s %d\n", k.id, k.ad, k.yazar, k.durum);
    }

    if (!bulundu)
        printf("\nHata: Girdiginiz ID'ye ait kitap bulunamadi veya zaten kiralanmis.\n");

    fclose(dosya);
    fclose(gecici);
    // Orijinal dosya guncellenir:
    remove("kitaplar.txt");
    rename("gecici.txt", "kitaplar.txt");
}

// Kitap ismine gore metin arama islemi:
void kitapAra(char *aktifKullanici) {
    Kitap k;
    char arananKelime[50],arananKucuk[50],adKucuk[50];
    int bulundu = 0;
    
    FILE *dosya = fopen("kitaplar.txt", "r");
    
    if (dosya == NULL) {
        printf("Hata: Sistemde henuz kayitli kitap yok!\n");
        return;
    }

    printf("\nKitap Arama:\n");
    printf("Aradiginiz kitabin adi (veya adinin bir kismi):");
    scanf("%s", arananKelime);
    
    kucukHarfCevir(arananKelime, arananKucuk);

    printf("\nArama Sonuclari:\n");

    // Dosya satir satir okunur:
    while (fscanf(dosya, "%d %s %s %d", &k.id, k.ad, k.yazar, &k.durum) != EOF) {
        kucukHarfCevir(k.ad, adKucuk); // Listedeki kitap adini kucuk harfe cevir
        
        if (strstr(adKucuk, arananKucuk) != NULL) {

            if (k.durum == 1) 
                printf("ID: %d | Kitap: %s | Yazar: %s | Durum: Rafta (Mevcut)\n", k.id, k.ad, k.yazar);
            else
                printf("ID: %d | Kitap: %s | Yazar: %s | Durum: Kirada (Mevcut Degil)\n", k.id, k.ad, k.yazar);          
            bulundu = 1;
        }
    }

    if (!bulundu)
        printf("'%s' ismini iceren herhangi bir kitap bulunamadi.\n", arananKelime);

    fclose(dosya);

    if (bulundu && strcmp(aktifKullanici, "") != 0) {
        int secim;
        printf("\n1. Bulunan kitaplardan birini kirala\n");
        printf("2. Menuye don\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);
        system("cls");

        if (secim == 1 ) 
            kitapKirala(aktifKullanici, arananKelime);
        else if(secim==2)
            return ;
        else
            printf("Hatali secim.");
    }
}

// Kullanicinin kiraladigi kitabi iade etmesi:
void kitapIade(char *aktifKullanici) {
    char okunanKullanici[30], r_kitapAd[50];
    int r_kitapId, istenenId, bulundu = 0, kitapSayisi = 0 ;

    FILE *kkDosya = fopen("kullanici_kitaplari.txt", "r");
    
    if (kkDosya == NULL) {
        printf("\nHata: Kiraladiginiz herhangi bir kitap kaydi bulunamadi!\n");
        return;
    }

    printf("\nKiraladiginiz Kitaplar:\n");
    
    while (fscanf(kkDosya, "%s %d %s", okunanKullanici, &r_kitapId, r_kitapAd) != EOF) {
        if (strcmp(okunanKullanici, aktifKullanici) == 0) {
            printf("ID: %d | Kitap Adi: %s\n", r_kitapId, r_kitapAd);
            kitapSayisi++;
        }
    }
    
    rewind(kkDosya);

    if (kitapSayisi == 0) {
        printf("Su anda uzerinizde kiralik kitap bulunmamaktadir.\n");
        fclose(kkDosya);
        return;
    }

    printf("\nIade etmek istediginiz kitabin ID'sini girin (Iptal icin 0): ");
    scanf("%d", &istenenId);

    if (istenenId == 0) {
        printf("Iade islemi iptal edildi.\n");
        fclose(kkDosya);
        return;
    }

    FILE *geciciKk = fopen("gecici_kk.txt", "w");
    // Iade edilen kitap disindakileri gecici dosyaya yaz (kaydi silmek icin)
    while (fscanf(kkDosya, "%s %d %s", okunanKullanici, &r_kitapId, r_kitapAd) != EOF) {
        if (strcmp(okunanKullanici, aktifKullanici) == 0 && r_kitapId == istenenId && bulundu == 0)
            bulundu = 1; 
        else 
            fprintf(geciciKk, "%s %d %s\n", okunanKullanici, r_kitapId, r_kitapAd);
    }

    fclose(kkDosya);
    fclose(geciciKk);

    if (bulundu) {
        remove("kullanici_kitaplari.txt");
        rename("gecici_kk.txt", "kullanici_kitaplari.txt");

        FILE *kitapDosya = fopen("kitaplar.txt", "r");
        FILE *geciciKitap = fopen("gecici_kitap.txt", "w");
        Kitap k;

        if (kitapDosya != NULL && geciciKitap != NULL) {
            while (fscanf(kitapDosya, "%d %s %s %d", &k.id, k.ad, k.yazar, &k.durum) != EOF) {
                if (k.id == istenenId)
                    k.durum = 1; 

                fprintf(geciciKitap, "%d %s %s %d\n", k.id, k.ad, k.yazar, k.durum);
            }
            fclose(kitapDosya);
            fclose(geciciKitap);
            remove("kitaplar.txt");
            rename("gecici_kitap.txt", "kitaplar.txt");
        }
        
        system("cls");
        printf("\nKitap basariyla iade edildi. Tesekkur ederiz!\n");
    } 
    else {
        remove("gecici_kk.txt");
        printf("\nHata: Girdiginiz ID'ye ait kiraladiginiz bir kitap bulunamadi.\n");
    }
}
// Ana menüler:
int main() {
    int anaSecim, altSecim, oturumSecim;
    char aktifKullanici[30];
    
    while (1) {
        printf("\nKUTUPHANE SISTEMI\n");
        printf("1. Kullanici Islemleri\n");
        printf("2. Personel Islemleri\n");
        printf("3. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &anaSecim);
        system("cls");

        if (anaSecim == 3) 
            break;

        switch (anaSecim) {
            case 1: // Kullanici Islemleri Ana Menusu
                while(1) {
                    printf("\nKULLANICI ISLEMLERI:\n");
                    printf("1. Kayit Ol\n");
                    printf("2. Giris Yap\n");
                    printf("3. Ana Menuye Don\n");
                    printf("Seciminiz: ");
                    scanf("%d", &altSecim);
                    system("cls");

                    if (altSecim == 1)
                        kayitOl("kullanici.txt");
                    else if (altSecim == 2) {
                        strcpy(aktifKullanici, "");
                        if (girisYap("kullanici.txt", aktifKullanici)) {
                            while (1) {
                                printf("\nKullanici: %s \n", aktifKullanici);
                                printf("1. Kitap Kirala\n");
                                printf("2. Kitap Ara\n");
                                printf("3. Kitap Iade\n"); 
                                printf("4. Oturumu Kapat (Ust Menuye Don)\n");
                                printf("Seciminiz: ");
                                scanf("%d", &oturumSecim);
                                system("cls");

                                if (oturumSecim == 1) 
                                    kitapKirala(aktifKullanici, "");
                                else if (oturumSecim == 2) 
                                    kitapAra(aktifKullanici); 
                                else if (oturumSecim == 3) 
                                    kitapIade(aktifKullanici); // Iade islemi cagiriliyor
                                else if (oturumSecim == 4) {
                                    printf("Oturum kapatildi. Ust menuye donuluyor...\n");
                                    break; 
                                }
                                else 
                                    printf("Hatali secim!\n");
                            }
                        }
                    } 
                    else if (altSecim == 3) 
                        break; 
                }
                break;

            case 2: // Personel Islemleri Ana Menusu
                while(1) {
                    printf("\nPERSONEL ISLEMLERI:\n");
                    printf("1. Personel Kaydi Olustur\n");
                    printf("2. Personel Girisi Yap\n");
                    printf("3. Ana Menuye Don\n");
                    printf("Seciminiz: ");
                    scanf("%d", &altSecim);
                    system("cls");

                    if (altSecim == 1)
                        kayitOl("personel.txt");
                    else if (altSecim == 2) {
                        strcpy(aktifKullanici, "");
                        if (girisYap("personel.txt", aktifKullanici)) {
                            while (1) {
                                printf("\n--- Personel: %s ---\n", aktifKullanici);
                                printf("1. Kitap Kayit (Sisteme Ekle)\n");
                                printf("2. Kitap Ara\n");
                                printf("3. Oturumu Kapat (Ust Menuye Don)\n");
                                printf("Seciminiz: ");
                                scanf("%d", &oturumSecim);
                                system("cls");

                                if (oturumSecim == 1) 
                                    kitapKayit();
                                else if (oturumSecim == 2) 
                                    kitapAra(""); 
                                else if (oturumSecim == 3) {
                                    printf("Oturum kapatildi. Ust menuye donuluyor...\n");
                                    break;
                                }
                                else 
                                    printf("Hatali secim!\n");
                            }
                        }
                    } 
                    else if (altSecim == 3) 
                        break; 
                }
                break;

            default:
                printf("Hatali secim! Lutfen 1-3 arasi bir deger girin.\n");
                break;
        }
    }   
    return 0;
}
