#pragma once
#include <raylib.h>
#include <stdlib.h>
#include "sqlite3.h"
#ifndef CONTEXT_H
#define CONTEXT_H
typedef void* opaque;
// #define LINKED_LIST_TYPE opaque
#include "linked-list.h"


typedef enum NoteDirection {
    NOTE_LEFT,
    NOTE_RIGHT,
    NOTE_UP,
    NOTE_DOWN,
} NoteDirection;

// Note adalah struktur dari note yang akan digunkana
typedef struct Note {
    // arah dari note
    NoteDirection direction;
    // kapan note itu harus di hit
    double hit_at_ms;
    // posisi dari note
    Vector2 position;
    // apakah note sudah didraw
    bool isSpawned;
    double duration_in_ms;
} Note;

// Beatmap adalah map yang akan digunakan pada saat gameplay. isinya terdiri dari array of notes
// Implement array_list protocol.
typedef struct Beatmap {
    Note *items;
    int len,cap;
} Beatmap;


// Track adalah abstraksi dari musik, yang berisi music itu sendiri, nama dari musik dan high skor dari musik.
typedef struct Track {
    int music_id;
    // referensi ke music yang dibutuhkan raylib
    Music music;
    // nama dari musik
    char music_name[255];
    // skor tertinggi dari musik ini
    int high_score;
    // akurasi dari musik ini
    float accuracy;
    // cover dari musik
    Texture2D cover;
    char* file_path;
} Track;

// Tracks berisi list dari Track yang tersedia
typedef struct Tracks {
    int len, cap;
    NodeAddress track;
} Tracks;

// Score adlaah skor yang telah didapatkan. biasanya ini digunakan dalam State Playing dan State result
typedef struct Score {
    // point yang sudah didapatkan
    long int point;
    // persentase dari akurasi
    double accuracy;
    int perfect, good, miss;
    // Streak
    int streak;
} Score;

Tracks InitTracks();

// State dari aplikasi, biasanya, ini digunakan untuk menentukan scene mana yang akan di gambar di layar.
// PENTING!!!!!!! Mohon untuk menambahkan nama pembuat jika membuat state baru.
typedef enum State {
    // aplikasi sedang loading
    APP_LOADING,
    // pengguna sedang memilih musik yang akan dimainkan 
    APP_SELECT,
    // pengguna sedang bermain
    APP_PLAYING,
    // aplikasi dalam mode title screen
    APP_PRESS_TO_PLAY,
    // aplikasi dalam mode pembuatan beatmap
    APP_BEATMAP_CREATOR,
    // menampilkan hasil score setelah permainan selesai, ditambakan oleh Nabil
    END_OF_THE_GAME,
} State;

// App Context adalah seluruh data yang diperlukan dalam jalannya aplikasi
// Untuk membuat context, gunakan fungsi CreateContext()
typedef struct AppContext {
    // lebar dari window
    int screen_width;
    // tinggi dari window
    int screen_height;
    // State dari aplikasi, ingfo lebih lanjut lihat enum State.
    State app_state;
    // List dari music yang tersedia, info lebih lanjut lihat struct Tracks
    Tracks tracks;
    // Skor dari music yang dimainkan, info lebih lanjut lihat struct Score
    Score score;
    // Track yang dipilih
    int selected_track;
    // Menentukan apakah music sedang berjalan.
    bool is_music_playing;
    // Font yang kita gunakan
    Font font;

    // Private Field: buffer untuk `GetSelectedMusicBeatmap()`
    Beatmap _beatmap;
    // Private Field: buffer untuk cek jika load beatmap yang sama 2x gak load lagi (lambat soalnya)
    // Deprecated
    char* _beatmap_name;
    // Private Field: int untuk cek jika load beatmap yang sama 2x gak load lagi (lambat soalnya)
    int _beatmap_music_id;
    // databse tempat menaruh beatmap
    sqlite3 *beatmap_db;
    // databse tempat menaruh skor  
    sqlite3 *score_db;
} AppContext;

AppContext CreateContext(int, int);
// Mengambil beatmap dari music saat ini.
Beatmap GetSelectedMusicBeatmap(AppContext* ctx);
// prosedur cleanup. tidak digunakan dalam scene, tapi di akhir aplikasi
void DestroyTracks(Tracks *tracks);
// prosedur cleanup. tidak digunakan dalam scene, tapi di akhir aplikasi
void DestroyContext(AppContext *ctx);

// 
Track GetTrack(Tracks tracks, int index) ;
// 
Track GetSelectedTrack(AppContext* ctx);

// Update context. digunakan pada saat gameloop
void UpdateContext(AppContext* ctx);
// Menjalankan music untuk track yang dipilih
void PlaySelectedTrack(AppContext* ctx);
// Stop Track track yang dipilih
void StopSelectedTrack(AppContext* ctx);
// Ubah posisi waktu musik dijalankan
void SeekSelectedTrack(AppContext* ctx, float second);
// Cek apakah musik selesai
bool IsSelectedMusicEnd(AppContext* ctx);

// Mengambil panjang waktu dari musik (dalam detik)
float GetSelectedMusicLength(AppContext* ctx);
// Mengambil time played dari musik
float GetSelectedMusicTimePlayed(AppContext* ctx);

// ambil nama musik.
char *GetSelectedMusicName(AppContext* ctx);
// Private fn:
// Untuk mengambil skor dan akurasi, gunakan `ctx.score`
// ambil skor dan akurasi dari file map. 
void GetScoreAndAccuracy(const char* file_name, int *scoreOut, float *accuracyOut);
void WriteSelectedMusicBeatmapToFile(Beatmap* btm, const char* music_name, int score, float accuracy);

// Set skor ke file .map
void SetScoreAndAccuracy(AppContext* ctx, int score, int acc);
#endif // CONTEXT_H