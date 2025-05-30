#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

struct Video {
    char judul[100];
    int durasi;
    char status[20];
    Video *kiri;
    Video *kanan;
};

struct SimpulRiwayat {
    Video* video;
    SimpulRiwayat* berikut;
};

struct SimpulPlaylist {
    Video* video;
    SimpulPlaylist* berikut;
};

Video* akar = NULL;
SimpulRiwayat* atasRiwayat = NULL;
SimpulPlaylist* kepalaPlaylist = NULL;
SimpulPlaylist* ekorPlaylist = NULL;

bool cariJudul(Video* akar, const char* judul) {
    if (akar == NULL) return false;
    if (strcmp(akar->judul, judul) == 0) return true;
    if (strcmp(judul, akar->judul) < 0)
        return cariJudul(akar->kiri, judul);
    else
        return cariJudul(akar->kanan, judul);
}

Video* tambahVideoKePohon(Video* akar, const char* judul, int durasi) {
    if (akar == NULL) {
        Video* baru = new Video;
        strcpy(baru->judul, judul);
        baru->durasi = durasi;
        strcpy(baru->status, "tersedia");
        baru->kiri = baru->kanan = NULL;
        return baru;
    }
    if (strcmp(judul, akar->judul) < 0)
        akar->kiri = tambahVideoKePohon(akar->kiri, judul, durasi);
    else if (strcmp(judul, akar->judul) > 0)
        akar->kanan = tambahVideoKePohon(akar->kanan, judul, durasi);
    return akar;
}

void inOrder(Video* akar) {
    if (akar != NULL) {
        inOrder(akar->kiri);
        cout << "Judul: " << akar->judul << ", Durasi: " << akar->durasi << ", Status: " << akar->status << endl;
        inOrder(akar->kanan);
    }
}

Video* cariVideo(Video* akar, const char* judul) {
    if (akar == NULL) return NULL;
    if (strcmp(akar->judul, judul) == 0) return akar;
    if (strcmp(judul, akar->judul) < 0)
        return cariVideo(akar->kiri, judul);
    else
        return cariVideo(akar->kanan, judul);
}

void tambahVideo() {
    char judul[100];
    int durasi;
    cout << "Masukkan judul video: ";
    cin.ignore();
    cin.getline(judul, 100);
    if (cariJudul(akar, judul)) {
        cout << "Judul sudah ada." << endl;
        return;
    }
    cout << "Masukkan durasi (menit): ";
    cin >> durasi;
    akar = tambahVideoKePohon(akar, judul, durasi);
    cout << "Video berhasil ditambahkan." << endl;
}

void tampilDaftarVideo() {
    cout << "\n== Daftar Video ==" << endl;
    inOrder(akar);
    char pilih;
    cout << "\nApakah Anda ingin mencari video (y/t)? ";
    cin >> pilih;
    if (pilih == 'y' || pilih == 'Y') {
        char cari[100];
        cout << "Masukkan judul: ";
        cin.ignore();
        cin.getline(cari, 100);
        Video* hasil = cariVideo(akar, cari);
        if (hasil)
            cout << "Ditemukan: " << hasil->judul << " | Durasi: " << hasil->durasi << " | Status: " << hasil->status << endl;
        else
            cout << "Video tidak ditemukan." << endl;
    }
}

void tambahKePlaylist() {
    char judul[100];
    cout << "Masukkan judul video untuk ditambahkan ke playlist: ";
    cin.ignore();
    cin.getline(judul, 100);
    Video* vid = cariVideo(akar, judul);
    if (!vid) {
        cout << "Video tidak ditemukan." << endl;
        return;
    }
    if (strcmp(vid->status, "dalam antrian") == 0 || strcmp(vid->status, "sedang diputar") == 0) {
        cout << "Video sudah di playlist atau sedang diputar." << endl;
        return;
    }
    SimpulPlaylist* baru = new SimpulPlaylist;
    baru->video = vid;
    baru->berikut = NULL;
    if (ekorPlaylist == NULL) {
        kepalaPlaylist = ekorPlaylist = baru;
        strcpy(vid->status, "sedang diputar");
    } else {
        ekorPlaylist->berikut = baru;
        ekorPlaylist = baru;
        strcpy(vid->status, "dalam antrian");
    }
    cout << "Video berhasil ditambahkan ke playlist." << endl;
}

void tontonVideo() {
    if (!kepalaPlaylist) {
        cout << "Playlist kosong." << endl;
        return;
    }
    Video* vid = kepalaPlaylist->video;
    cout << "Menonton: " << vid->judul << endl;
    strcpy(vid->status, "tersedia");

    SimpulRiwayat* riw = new SimpulRiwayat;
    riw->video = vid;
    riw->berikut = atasRiwayat;
    atasRiwayat = riw;

    SimpulPlaylist* temp = kepalaPlaylist;
    kepalaPlaylist = kepalaPlaylist->berikut;
    delete temp;
    if (kepalaPlaylist == NULL) ekorPlaylist = NULL;
    if (kepalaPlaylist) strcpy(kepalaPlaylist->video->status, "sedang diputar");
}

void tampilRiwayat() {
    cout << "\n== Riwayat Tontonan ==" << endl;
    SimpulRiwayat* temp = atasRiwayat;
    while (temp) {
        cout << "Judul: " << temp->video->judul << " | Durasi: " << temp->video->durasi << endl;
        temp = temp->berikut;
    }
}

void menuUtama() {
    int menu;
    do {
        cout << "\n===== MENU IDLIX TUBE =====" << endl;
        cout << "1. Tambah Video" << endl;
        cout << "2. Tampilkan Daftar Video" << endl;
        cout << "3. Tambahkan ke Playlist" << endl;
        cout << "4. Tonton Video" << endl;
        cout << "5. Riwayat Tontonan" << endl;
        cout << "6. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> menu;
        cin.ignore();
        system("cls");

        switch(menu) {
            case 1:
                tambahVideo();
                break;
            case 2:
                tampilDaftarVideo();
                break;
            case 3:
                tambahKePlaylist();
                break;
            case 4:
                tontonVideo();
                break;
            case 5:
                tampilRiwayat();
                break;
            case 6:
                cout << "Keluar dari program..." << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while(menu != 6);
}

int main() {
    menuUtama();
    return 0;
}