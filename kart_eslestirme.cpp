#include <iostream> // Giriş ve çıkış işlemleri için standart kütüphane.
#include <vector> // Vektör veri yapısını kullanmak için kütüphane.
#include <algorithm> // Algoritma işlemleri için standart kütüphane.
#include <random> // Rastgele sayı üretimi için kullanılan kütüphane.
#include <ctime> // Zaman bilgisi için kullanılan kütüphane.
#include <cstdlib> // Genel amaçlı yardımcı fonksiyonlar için kütüphane.
#include <string> // String işlemleri için kullanılan kütüphane.
#include <stdexcept> // Hata yönetimi için standart kütüphane.

using namespace std;

// Temel Kart Sınıfı
// Bu sınıf soyutlama ve kalıtım için temel olarak kullanılır.
class Kart {
protected:
    string tur; // Kapsülleme: "tur" değişkeni protected olarak tanımlandı ve doğrudan erişim engellendi.

public:
    Kart(const string& tur) : tur(tur) {} // Sınıf yapıcı metodu ile "tur" özelliği başlatılıyor.
    //Virtual olarak tanımlama yapılarak türetilmiş sınıfların bu metodu kendi ihtiyaçlarına göre yeniden tanımlayabilmesi sağlandı.
    virtual ~Kart() = default; // Sanal yıkıcı: Polimorfizm altyapısı için gerekli. Belleği temizlemek için kullanılır.
    virtual string getTur() const { // Polimorfizm: Türetilmiş sınıflar bu metodu özelleştirebilir.
        return tur;
    }
};

// Meyve Kartı Sınıfı (Kalıtım Örneği)
// "Kart" sınıfından türetilmiş bir sınıf. Kalıtım burada kullanılıyor.
class MeyveKart : public Kart {
public:
    MeyveKart(const string& tur) : Kart(tur) {} // MeyveKart yapıcı metodu, "Kart" sınıfından gelen "tur" parametresini alır.
};

// Oyuncu Sınıfı
// Oyuncuların özelliklerini ve puanlarını kapsüllemek için kullanılır.
class Oyuncu {
private:
    int id; // Oyuncunun kimliği (private: Kapsülleme sağlanmış).
    int puan; // Oyuncunun puanı.

public:
    Oyuncu(int id) : id(id), puan(0) {} // Sınıf yapıcı metodu ile id ve puan başlatılıyor.

    int getId() const { // Oyuncu kimliğini döndüren metot (enkapsülasyon).
        return id;
    }

    void puanEkle() { // Oyuncunun puanını artıran metot.
        puan++;
    }

    int getPuan() const { // Oyuncunun mevcut puanını döndüren metot.
        return puan;
    }
};

// Oyun Tahtası Sınıfı
// Oyun tahtasının ve kartların yönetimi için soyutlama sağlar.
class Tahta {
private:
    vector<vector<MeyveKart>> kartlar; // Kartların yer aldığı 2 boyutlu vektör (private: kapsülleme).
    vector<vector<bool>> acikMi; // Kartların açık olup olmadığını belirten 2 boyutlu vektör.

public:
    Tahta() : kartlar(4, vector<MeyveKart>(4, MeyveKart(""))), acikMi(4, vector<bool>(4, false)) {
        // Tahtayı ve kartları rastgele düzenlemek için gerekli başlatma işlemleri.
        srand(static_cast<unsigned int>(time(nullptr))); // Rastgelelik başlatılır.
        vector<string> turler = {" Elma", " Elma", "Armut", "Armut", "Cilek", "Cilek", "Kiraz", "Kiraz", "Kivi ", "Kivi ", "Erik ", "Erik ", "Kavun", "Kavun", " Muz ", " Muz "};

        for (size_t i = turler.size() - 1; i > 0; --i) { // Kart türlerini rastgele karıştırır.
            size_t j = rand() % (i + 1);
            swap(turler[i], turler[j]);
        }

        int k = 0;
        for (size_t i = 0; i < kartlar.size(); ++i) {
            for (size_t j = 0; j < kartlar[i].size(); ++j) {
                kartlar[i][j] = MeyveKart(turler[k++]); // Her kart türü bir MeyveKart nesnesine atanır.
            }
        }
    }

    void goster() const { // Oyun tahtasını gösteren metot.
        cout << "\n     0           1           2           3\n";
        cout << "   +-----------+-----------+-----------+-----------+\n";
        for (size_t i = 0; i < kartlar.size(); ++i) {
            cout << " " << i << " |";
            for (size_t j = 0; j < kartlar[i].size(); ++j) {
                if (acikMi[i][j]) {
                    cout << "   " << kartlar[i][j].getTur() << "   |"; // Kart açık ise türü gösterilir.
                } else {
                    cout << "     X     |"; // Kart kapalı ise "X" gösterilir.
                }
            }
            cout << "\n   +-----------+-----------+-----------+-----------+\n";
        }
    }

    bool ac(int satir, int sutun) { // Kartı açmaya çalışan metot.
        if (satir >= 0 && satir < 4 && sutun >= 0 && sutun < 4 && !acikMi[satir][sutun]) {
            acikMi[satir][sutun] = true; // Kart açılır.
            return true;
        }
        return false; 
    }

    string getKartTur(int satir, int sutun) const { // Bir kartın türünü döndüren metot.
        return kartlar[satir][sutun].getTur(); // İstenen kartın türü döndürülür.
    }

    void kaliciAc(int satir, int sutun) { // Kartı kalıcı olarak açık hale getiren metot.
        acikMi[satir][sutun] = true; // Kart artık kapatılmayacak şekilde açık kalır.
    }

    void kapat(int satir, int sutun) { // Kartı tekrar kapatan metot.
        acikMi[satir][sutun] = false; // Kart tekrar "kapalı" hale getirilir.
    }

    bool tumKartlarAcikMi() const { // Tüm kartların açık olup olmadığını kontrol eden metot.
        return all_of(acikMi.begin(), acikMi.end(), [](const vector<bool>& satir) {
            return all_of(satir.begin(), satir.end(), [](bool val) { return val; });
        }); // Tüm kartlar açık ise true döner.
    }
};

// Oyun Sınıfı
// Oyunun ana akışını ve oyuncular arası etkileşimi kontrol eder.
class KartEslestirmeOyunu {
private:
    Tahta tahta; // Oyun tahtası.
    Oyuncu oyuncu1; // İlk oyuncu.
    Oyuncu oyuncu2; // İkinci oyuncu.
    Oyuncu* aktifOyuncu; // Sıradaki oyuncu.

public:
    KartEslestirmeOyunu() : oyuncu1(1), oyuncu2(2), aktifOyuncu(&oyuncu1) {} // Oyuncular ve aktif oyuncu başlatılır.

    void oynat() { // Oyunun ana akışını kontrol eden metot.
        while (!tahta.tumKartlarAcikMi()) { // Tüm kartlar açık değilken oyun devam eder.
            tahta.goster(); 

            cout << "\nOyuncu " << aktifOyuncu->getId() << "'nin sirasi.\n"; 
            int satir1, sutun1, satir2, sutun2; 

            while (true) {
                try {
                    cout << "Birinci kartin satir ve sutununu girin: ";
                    cin >> satir1 >> sutun1; 
                    if (cin.fail() || !tahta.ac(satir1, sutun1)) {
                        throw runtime_error("Gecersiz girdi. Lutfen tekrar deneyin."); //Hatalı giriş yapılırsa bir runtime_error fırlatılır. 
                    }
                    break;
                } catch (const runtime_error& e) {
                    cout << e.what() << endl; // Hata mesajı gösterilir.
                    cin.clear(); //Hatalı girişten sonra cin akışındaki hata durumunu temizler. Bu işlem, kullanıcının bir sonraki girişini kabul edebilmek için gereklidir.
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Kullanıcı bir harf veya geçersiz bir ifade girmişse, bu karakterler giriş akışından silinir.

                }
            }

            tahta.goster(); 

            while (true) {
                try {
                    cout << "Ikinci kartin satir ve sutununu girin: ";
                    cin >> satir2 >> sutun2; 
                    if (cin.fail() || !tahta.ac(satir2, sutun2) || (satir1 == satir2 && sutun1 == sutun2)) {
                        throw runtime_error("Gecersiz girdi. Lutfen tekrar deneyin."); 
                    }
                    break;
                } catch (const runtime_error& e) {
                    cout << e.what() << endl; 
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            tahta.goster(); 

            if (tahta.getKartTur(satir1, sutun1) == tahta.getKartTur(satir2, sutun2)) { // Kart türleri eşleşiyor mu kontrol edilir.
                cout << "\nEslesme bulundu! Oyuncu " << aktifOyuncu->getId() << " bir puan kazandi!\n";
                aktifOyuncu->puanEkle(); 
                tahta.kaliciAc(satir1, sutun1);
                tahta.kaliciAc(satir2, sutun2);
            } else {
                cout << "\nEslesme yok. Kartlar tekrar kapatilacak.\n";
                tahta.kapat(satir1, sutun1); 
                tahta.kapat(satir2, sutun2);
                aktifOyuncu = (aktifOyuncu == &oyuncu1) ? &oyuncu2 : &oyuncu1; // Sıradaki oyuncu değiştirilir.
            }
        }

        cout << "\nOyun bitti!\n";
        cout << "Oyuncu 1 puan: " << oyuncu1.getPuan() << "\n";
        cout << "Oyuncu 2 puan: " << oyuncu2.getPuan() << "\n";

        if (oyuncu1.getPuan() > oyuncu2.getPuan()) {
            cout << "Oyuncu 1 kazandi!\n";
        } else if (oyuncu2.getPuan() > oyuncu1.getPuan()) {
            cout << "Oyuncu 2 kazandi!\n";
        } else {
            cout << "Berabere!\n";
        }
    }
};

int main() {
    KartEslestirmeOyunu oyun; // Oyun nesnesi oluşturulur.
    oyun.oynat(); // Oyunu başlatır.
    return 0; 
}
