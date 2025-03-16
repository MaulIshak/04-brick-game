# Project Setup

Buka command line lalu eksekusi perintah dibawah ini. Pastikan eksekusi didalam git bash.

```sh
./init.bat
```

# Extensi

Untuk penglaman development yang lebih baik, gunakan vscode dan install extensi C/C++ dari microsoft. Lalu jalankan build.bat paling tidak 1x.

# Building

Untuk menjalankan program, buka command line lalu eksekusi perintah ini.

```sh
./build.sh
```

## Penjelasan

script `init.bat` akan mendowload zig compiler versi 0.13.0 dan menaruhnya di folder `bin/zig`. Ini dilakukan agar semua anggota memiliki compiler dengan jenis yang sama untuk menghindari masalah masalah mengenai compiler yang tidak kompatible.

script `build.bat` akan mengcompile project

# Arsitektur

TODO: