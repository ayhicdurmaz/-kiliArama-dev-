// Ahmet Yavuz Hiçdurmaz - 22120205019
// fseek() methodu kullanılmadı çünkü eldeki dosyalar sabit satır karakterlerine sahip değiller.
// Bu yüzden bir arraye kaydetmek yerine dosyadan okuyarak yaptım. Amacın arraye kaydetmemek olduğunu düşünerek.

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// İki stringin eşitliğini kontrol eder.
int strEsit(char *_aranan, char *_satir) {
    for(int i = 0; _aranan[i] != '\0'; i++){
        if(_satir[i] != _aranan[i]){
            return 0;
        }
    }
    return 1;
}

// Dosyadaki satır sayısını hesaplayan fonksiyon:
int satirSayisiHesapla(FILE *_dosya)
{
    char karakter;
    int satir = 0;

    while ((karakter = fgetc(_dosya)) != EOF)
    {
        if (karakter == '\n')
        {
            satir++;
        }
    }

    fseek(_dosya, 1, SEEK_SET);

    return satir;
}

// İkili arama işlemini gerçekleştiren fonksiyon. Hatalı sonuçlar -1, doğru sonuçlar satir çıktısı verilir.
int ikiliArama(FILE *_dosya, char *aranan, int _bas_satir, int _son_satir)
{
    int hedef = (int)((_bas_satir + _son_satir) / 2);

    char satir[25] = "";
    int i = 0;

    while (fgets(satir, 25, _dosya))
    {
        if (hedef == i)
        {
            int sonuc = strcmp(satir, aranan);

            // Aranılan ifadenin olması gereken adres
            if (_son_satir == _bas_satir)
            {
                fseek(_dosya, 1, SEEK_SET);
                // Aranılan isime ulaşıldı mı, yoksa olmayan bir ismin yeri tahmin mi edildi kontrolü yapılır.
                if(strEsit(aranan, satir) == 1){
                    // Satırlar 1'den indisler 0'dan başladığı için bir ekleyerek satır sayısına ulaşıyoruz.
                    return _bas_satir + 1;
                }else{
                    return -1;
                }
            }

            // Aranılan ifade seçilen satırdan alfabetik olarak gerideyse yeni aralığın son satırı o anki seçilen satıra eşitlenir.
            // Fonksiyon bu yeni alt ve üst sınırlarda tekrar çalıştırılır.
            else if (sonuc > 0)
            {
                _son_satir = hedef;
                fseek(_dosya, 1, SEEK_SET);
                return ikiliArama(_dosya, aranan, _bas_satir, _son_satir);
            }

            // Aranılan ifade seçilen satırdan alfabetik olarak ilerideyse yeni aralığın ilk satırı o anki seçilen satırın bir fazlasına eşitlenir.
            // Fonksiyon bu yeni alt ve üst sınırlarda tekrar çalıştırılır.
            else if (sonuc < 0)
            {
                _bas_satir = hedef + 1;
                fseek(_dosya, 1, SEEK_SET);
                return ikiliArama(_dosya, aranan, _bas_satir, _son_satir);
            }
            else
            {
                return -1;
            }
        }
        i++;
    }
    return -1;
}

int main()
{
    // Dosya adı alınır.
    char dosya_adi[20];
    printf("Dosya adi giriniz : \n");
    scanf("%s", dosya_adi);
    printf("%s aciliyor \n", dosya_adi);
    FILE *dosya;
    dosya = fopen(dosya_adi, "r");

    // Dosyaya doğru erişilebildiği kontrol edilir.
    if (dosya == NULL)
    {
        printf("Dosya Acma Hatasi !!! Dosya Adini Kontrol Ediniz !!! \n");
        return 1;
    }
    printf("Dosya Acildi !!! \n");

    // Kullanıcıdan aranılacak isim alınır.
    char isim[20] = "Tammara";
    printf("Aranacak ismi giriniz (Harfin buyuk kucuk olmasına dikkat edin): \n");
    scanf("%s", isim);
    printf("%s ismi aranacak. \n", isim);

    // İkili arama için alt ve üst sınırlar verilir.
    int baslangıc_satiri = 0;
    int son_satir = satirSayisiHesapla(dosya);

    // Fonksiyon sonucunda aranılan satıra erişilir.
    int aranilan_satir = ikiliArama(dosya, isim, baslangıc_satiri, son_satir);

    // Ekrana yazdırılır.
    if (aranilan_satir != -1)
    {
        printf("%s ismi %d. satirda.", isim, aranilan_satir);
    }
    else
    {
        printf("Aranilan isim dosyada mevcut degil.", isim, aranilan_satir);
    }

    // Dosya kapatılıp program sonlandırılır.
    fclose(dosya);

    return 0;
}