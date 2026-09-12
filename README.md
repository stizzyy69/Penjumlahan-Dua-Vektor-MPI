# Paralelisasi Penjumlahan Dua Vektor Menggunakan MPI

Program ini melakukan penjumlahan dua vektor secara paralel menggunakan
Message Passing Interface (MPI).

# Cara Menjalankan

Compile:
cl vector_mpi.c /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib

Jalankan:
mpiexec -n 2 vector_mpi.exe
