#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Video
{
    char judul[100];
    int durasi;
    int status; // 0: tersedia, 1: dalam antrean, 2: sedang diputar
    Video *kiri, *kanan;
};

struct Node
{
    Video *video;
    Node *next;
};

// Stack untuk riwayat tontonan
struct Stack
{
    Node *top = NULL;

    void push(Video *v)
    {
        Node *baru = new Node;
        baru->video = v;
        baru->next = top;
        top = baru;
    }

    void tampil()
    {
        Node *bantu = top;
        if (!bantu)
        {
            cout << "(Kosong)\n";
            return;
        }
        while (bantu != NULL)
        {
            cout << "- " << bantu->video->judul << endl;
            bantu = bantu->next;
        }
    }
};

// Queue untuk playlist
struct Queue
{
    Node *depan = NULL, *belakang = NULL;

    void enqueue(Video *v)
    {
        Node *baru = new Node;
        baru->video = v;
        baru->next = NULL;
        if (!depan)
            depan = belakang = baru;
        else
        {
            belakang->next = baru;
            belakang = baru;
        }
    }

    Video *dequeue()
    {
        if (!depan)
            return NULL;
        Node *hapus = depan;
        Video *v = hapus->video;
        depan = depan->next;
        delete hapus;
        return v;
    }

    void tampil()
    {
        Node *bantu = depan;
        if (!bantu)
        {
            cout << "(Playlist kosong)\n";
            return;
        }
        while (bantu != NULL)
        {
            cout << "- " << bantu->video->judul << endl;
            bantu = bantu->next;
        }
    }
};

Video *akar = NULL;
Queue playlist;
Stack riwayat;

bool cekDuplikat(Video *akar, char judul[])
{
    if (!akar)
        return false;
    int cmp = strcmp(judul, akar->judul);
    if (cmp == 0)
        return true;
    if (cmp < 0)
        return cekDuplikat(akar->kiri, judul);
    return cekDuplikat(akar->kanan, judul);
}

Video *tambahVideo(Video *akar, Video *baru)
{
    if (!akar)
        return baru;
    int cmp = strcmp(baru->judul, akar->judul);
    if (cmp < 0)
        akar->kiri = tambahVideo(akar->kiri, baru);
    else if (cmp > 0)
        akar->kanan = tambahVideo(akar->kanan, baru);
    return akar;
}

void tampilAscending(Video *akar)
{
    if (!akar)
        return;
    tampilAscending(akar->kiri);
    cout << "Judul: " << akar->judul << " | Durasi: " << akar->durasi << " menit | Status: ";
    if (akar->status == 0)
        cout << "Tersedia\n";
    else if (akar->status == 1)
        cout << "Dalam Antrean\n";
    else
        cout << "Sedang Diputar\n";
    tampilAscending(akar->kanan);
}

Video *cariVideo(Video *akar, char judul[])
{
    if (!akar)
        return NULL;
    int cmp = strcmp(judul, akar->judul);
    if (cmp == 0)
        return akar;
    if (cmp < 0)
        return cariVideo(akar->kiri, judul);
    return cariVideo(akar->kanan, judul);
}

void simpanKeFile(Video *akar, ofstream &file)
{
    if (!akar)
        return;
    simpanKeFile(akar->kiri, file);
    file << akar->judul << "," << akar->durasi << "," << akar->status << endl;
    simpanKeFile(akar->kanan, file);
}

int main()
{
    int pilih;
    do
    {
        cout << "\n=== Menu IDLIX Tube ===\n";
        cout << "1. Tambah Video\n2. Tampilkan Daftar Video\n3. Tambahkan ke Playlist\n4. Tonton Video\n5. Riwayat Tontonan\n6. Simpan & Keluar\nPilih: ";
        cin >> pilih;
        cin.ignore();
        system("cls");

        if (pilih == 1)
        {
            Video *baru = new Video;
            cout << "Judul: ";
            cin.getline(baru->judul, 100);
            if (cekDuplikat(akar, baru->judul))
            {
                cout << "Judul sudah ada.\n";
                delete baru;
                continue;
            }
            cout << "Durasi (menit): ";
            cin >> baru->durasi;
            cin.ignore();
            baru->status = 0;
            baru->kiri = baru->kanan = NULL;
            akar = tambahVideo(akar, baru);
            cout << "Video berhasil ditambahkan!\n";
        }
        else if (pilih == 2)
        {
            tampilAscending(akar);
            char cari;
            cout << "Cari video berdasarkan judul? (y/t): ";
            cin >> cari;
            cin.ignore();
            if (cari == 'y')
            {
                char judul[100];
                cout << "Masukkan judul: ";
                cin.getline(judul, 100);
                Video *v = cariVideo(akar, judul);
                if (v)
                    cout << "Ditemukan: " << v->judul << ", durasi: " << v->durasi << " menit\n";
                else
                    cout << "Video tidak ditemukan.\n";
            }
        }
        else if (pilih == 3)
        {
            char judul[100];
            cout << "Masukkan judul video untuk playlist: ";
            cin.getline(judul, 100);
            Video *v = cariVideo(akar, judul);
            if (!v)
                cout << "Video tidak ditemukan.\n";
            else if (v->status != 0)
                cout << "Video tidak tersedia.\n";
            else
            {
                v->status = playlist.depan == NULL ? 2 : 1;
                playlist.enqueue(v);
                cout << "Video ditambahkan ke playlist!\n";
            }
        }
        else if (pilih == 4)
        {
            if (playlist.depan == NULL)
            {
                cout << "Playlist kosong.\n";
                continue;
            }
            Video *v = playlist.dequeue();
            cout << "Memutar video: " << v->judul << endl;
            v->status = 0;
            riwayat.push(v);
            if (playlist.depan)
                playlist.depan->video->status = 2;
        }
        else if (pilih == 5)
        {
            cout << "Riwayat Tontonan:\n";
            riwayat.tampil();
        }
        else if (pilih == 6)
        {
            ofstream file("data_video.txt");
            simpanKeFile(akar, file);
            file.close();
            cout << "Data disimpan. Keluar...\n";
        }

    } while (pilih != 6);

    return 0;
}