#include <raylib.h>
#include <stdlib.h>

#ifndef CONTEXT_H
#define CONTEXT_H

typedef enum NoteDirection {
    NOTE_LEFT,
    NOT_RIGHT,
    NOTE_UP,
    NOTE_DOWN,
} NoteDirection;

// Note adalah struktur dari note yang akan digunkana
typedef struct Note {
    // arah dari note
    NoteDirection direction;
    // kapan note itu harus di hit
    double hit_at_ms;
} Note;

// Beatmap adalah map yang akan digunakan pada saat gameplay. isinya terdiri dari array of notes
typedef struct Beatmap {
    Note *notes;
    int len,cap;
} Beatmap;


// Track adalah abstraksi dari musik, yang berisi music itu sendiri, nama dari musik dan high skor dari musik.
typedef struct Track {
    // referensi ke music yang dibutuhkan raylib
    Music music;
    // nama dari musik
    char music_name[50];
    // skor tertinggi dari musik ini
    int high_score;
} Track;

// Tracks berisi list dari Track yang tersedia
typedef struct Tracks {
    int len, cap;
    Track *track;
} Tracks;

// Score adlaah skor yang telah didapatkan. biasanya ini digunakan dalam State Playing dan State result
typedef struct Score {
    // point yang sudah didapatkan
    long int point;
    // persentase dari akurasi
    double accuracy;
    // Streak
    int streak;
} Score;

Tracks InitTracks();

// State dari aplikasi, biasanya, ini digunakan untuk menentukan scene mana yang akan di gambar di layar.
typedef enum State {
    // aplikasi sedang loading
    APP_LOADING,
    // pengguna sedang memilih musik yang akan dimainkan 
    APP_SELECT,
    // pengguna sedang bermain
    APP_PLAYING,
    // aplikasi dalam mode title screen
    APP_PRESS_TO_PLAY,
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
} AppContext;

AppContext CreateContext(int, int);
// Mengambil beatmap dari music saat ini.
Beatmap GetSelectedMusicBeatmap(AppContext* ctx);
// prosedur cleanup. tidak digunakan dalam scene, tapi di akhir aplikasi
void DestroyTracks(Tracks *tracks);
// prosedur cleanup. tidak digunakan dalam scene, tapi di akhir aplikasi
void DestroyContext(AppContext *ctx);

void SelectMusic(AppContext* ctx);
// Update context. digunakan pada saat gameloop
void UpdateContext(AppContext* ctx);
// Menjalankan music untuk track yang dipilih
void PlaySelectedTrack(AppContext* ctx);
// Stop Track track yang dipilih
void StopSelectedTrack(AppContext* ctx);
// Ubah posisi waktu musik dijalankan
void SeekSelectedTrack(AppContext* ctx, float second);

#endif // CONTEXT_H