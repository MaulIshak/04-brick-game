# Control

`Arrow UP/DOWN` untuk navigasi di select menu, `Enter` untuk memilih musik.

`D/F/G/H` untuk kontrol pad (mulai dari kiri ke kanan). Selaint itu, arrow key juga bisa digunakan

# Project Setup

Buka command line dengan shell bash (git bash biasanya untuk windows) lalu eksekusi perintah dibawah ini.

```sh
./init.sh
```

# Extensi

Untuk penglaman development yang lebih baik, gunakan vscode dan install extensi C/C++ dari microsoft. Lalu jalankan build.bat paling tidak 1x.

# Building

Untuk menjalankan program, buka command line lalu eksekusi perintah ini.

```sh
./build.sh
```

## Penjelasan

script `init.sh` akan mendowload zig compiler versi 0.13.0 dan menaruhnya di folder `bin/zig`. Ini dilakukan agar semua anggota memiliki compiler dengan jenis yang sama untuk menghindari masalah masalah mengenai compiler yang tidak kompatible.

script `build.sh` akan mengcompile project

Recompile resource.rc harus menggunakan windres yang hanya tersedia di ekosistem `MSYS`. Jadi jika ingin mengganti icon dan deskripsi, pengguna harus mendownload windres dan gcc dengan output object file, yang nanti di compile lagi mengunakan compiler `bin/zig`

# Arsitektur

TODO:

# CREDITS:

Tools, Program and Library:  
- Zig Programming Language - Tools yang membundle C Compiler yang bisa kami vendor supaya tidak banyak konfigurasi lokal [Website](https://ziglang.org/)
- Raylib - Library grafis penompang project ini [Website](https://www.raylib.com/)
- Kiss FFT - Library fast fourier transfrom yang mudah digunakan [source](https://github.com/mborgerding/kissfft)

Font & Texture:  
TODO!!.

Musics: 
- Guardian of Former Seas : DM Dokuro [Youtube](https://youtu.be/hPEQLU-aD3I?si=9heevkGvYLgjFbVB)
- Pest of The Cosmos: DM Dokuro [Youtube](https://youtu.be/KJHNdl3R8GM?si=9-p5swVbVlLLFAuX)
- Polban PKKMB 2024: [Youtube](https://youtu.be/WRBgfxkUZPE?si=PesefKsDdmmXgz6l)
- Bad Apple: ZUN
- Line Lets Get Rich - Toy Land : ?
- Line Lets Get Rich - Title Theme : ?
- Infinite Cosmodynamics: Yang Lee [Youtube](https://youtu.be/tSOCwLjv3WQ?si=E2CxA5cTtO2vGeuS) 
- Outer Space: Muhammad Nabil Mudzaki