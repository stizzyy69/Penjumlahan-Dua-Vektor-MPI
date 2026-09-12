#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N;

    // Array maks 26 elemen (karena a - z)
    int A[26];
    int B[26];
    int C[26];

    int localA[26];
    int localB[26];
    int localC[26];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Input
    if (rank == 0) {
        printf("Masukkan jumlah elemen vektor (1-26): ");
        fflush(stdout);
        scanf("%d", &N);

        // Input ukuran vektor
        if (N < 1 || N > 26) {
            printf("Jumlah elemen harus antara 1 sampai 26.\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Mengecek pembagian sebelum meminta elemen
    if (N % size != 0) {
        if (rank == 0){
            printf("Jumlah elemen harus habis dibagi jumlah proses.\n");
            printf("Jumlah elemen = %d, jumlah proses = %d\n", N, size);
            fflush(stdout);
        }
        MPI_Finalize();
        return 0;
    }

    // Input vektor
    if (rank == 0) {
        printf("\nMasukkan Vektor A: ");
        fflush(stdout);

        for (int i = 0; i < N; i++) {
            scanf("%d", &A[i]);
        }

        while (getchar() != '\n');

        // Input vektor B
        printf("Masukkan Vektor B: ");
        fflush(stdout);

        for (int i = 0; i < N; i++) {
            scanf("%d", &B[i]);
        }

        while (getchar() != '\n');
    }

    // Membagikan ukuran dan data
    int local_N = N / size;

    MPI_Scatter(A, local_N, MPI_INT, localA, local_N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_N, MPI_INT, localB, local_N, MPI_INT, 0, MPI_COMM_WORLD);

    // Penjumlahan bagian tiap proses
    for (int i = 0; i < local_N; i++) {
        localC[i] = localA[i] + localB[i];
    }

    // Menampilkan vektor
    if (rank == 0) {
        printf("\nVektor A : ");

        for (int i = 0; i < N; i++) {
            printf("%d%c ", A[i], 'a' + i);
        }

        printf("\n");

        printf("Vektor B : ");

        for (int i = 0; i < N; i++) {
            printf("%d%c ", B[i], 'a' + i);
        }

        printf("\n\n");
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Menampilkan proses
    for (int p = 0; p < size; p++) {
        if (rank == p) {
            printf("Proses %d : ", rank);

            for (int i = 0; i < local_N; i++) {
                int index = rank * local_N + i;

                printf("%d%c + %d%c = %d%c,  ",
                       localA[i], 'a' + index,
                       localB[i], 'a' + index,
                       localC[i], 'a' + index);
            }

            printf("\n");
            fflush(stdout);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Menggabungkan hasil
    MPI_Gather(localC, local_N, MPI_INT, C, local_N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nHasil penjumlahan : ");

        for (int i = 0; i < N; i++) {
            printf("%d%c ", C[i], 'a' + i);
        }

        printf("\n");
    }

    MPI_Finalize();

    return 0;
}