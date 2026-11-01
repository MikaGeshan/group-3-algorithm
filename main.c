#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sortBooks(void);

#define MAX_BOOKS 1000
#define MAX_HISTORY 1000
#define LINE_LENGTH 256
#define BOOK_FILE "databuku.txt"
#define HISTORY_FILE "history.txt"

typedef struct
{
    char code[16];
    char name[101];
    char category[51];
    double price;
} Book;

typedef struct
{
    char code[16];
    int quantity;
    double total;
} History;

Book books[MAX_BOOKS];
History historyRecords[MAX_HISTORY];
int bookCount = 0;
int historyCount = 0;

static int readLine(FILE *file, char *text, int capacity)
{
    if (fgets(text, capacity, file) == NULL)
    {
        return 0;
    }

    if (strchr(text, '\n') == NULL)
    {
        int character = fgetc(file);
        if (character != '\n' && character != EOF)
        {
            while ((character = fgetc(file)) != '\n' && character != EOF)
            {
            }
            return -1;
        }
    }

    text[strcspn(text, "\r\n")] = '\0';
    return 1;
}

static int hasContent(const char *text)
{
    while (*text != '\0')
    {
        if (!isspace((unsigned char)*text))
        {
            return 1;
        }
        text++;
    }
    return 0;
}

static int getCodeNumber(const char *code)
{
    if (strncmp(code, "BK", 2) != 0 || code[2] == '\0')
    {
        return 0;
    }
    for (const char *p = code + 2; *p != '\0'; p++)
    {
        if (!isdigit((unsigned char)*p))
        {
            return 0;
        }
    }
    errno = 0;
    long number = strtol(code + 2, NULL, 10);
    return errno == 0 && number > 0 && number <= INT_MAX ? (int)number : 0;
}

int loadBooks(void)
{
    FILE *file = fopen(BOOK_FILE, "r");
    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return 1;
        }
        perror(BOOK_FILE);
        return 0;
    }

    char line[LINE_LENGTH];
    int status;
    int valid = 1;
    while ((status = readLine(file, line, sizeof(line))) != 0)
    {
        Book book;
        int end = 0;
        if (status < 0 || bookCount == MAX_BOOKS ||
            sscanf(line, "%15[^|]|%100[^|]|%50[^|]|%lf%n",
                   book.code, book.name, book.category, &book.price, &end) != 4 ||
            hasContent(line + end) || getCodeNumber(book.code) == 0 ||
            !hasContent(book.name) || !hasContent(book.category) ||
            !isfinite(book.price) || book.price <= 0)
        {
            valid = 0;
            break;
        }
        for (int i = 0; i < bookCount; i++)
        {
            if (getCodeNumber(books[i].code) == getCodeNumber(book.code))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            break;
        }
        books[bookCount++] = book;
    }
    if (ferror(file))
    {
        valid = 0;
    }
    if (fclose(file) != 0)
    {
        valid = 0;
    }
    if (!valid)
    {
        fprintf(stderr, "Gagal memuat %s: periksa format atau kapasitas data.\n", BOOK_FILE);
    }
    return valid;
}

int loadHistory(void)
{
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return 1;
        }
        perror(HISTORY_FILE);
        return 0;
    }

    char line[LINE_LENGTH];
    int status;
    int valid = 1;
    while ((status = readLine(file, line, sizeof(line))) != 0)
    {
        History history;
        char quantityText[32];
        char *quantityEnd;
        int end = 0;
        if (status < 0 || historyCount == MAX_HISTORY ||
            sscanf(line, "%15[^|]|%31[^|]|%lf%n", history.code,
                   quantityText, &history.total, &end) != 3 ||
            hasContent(line + end) || getCodeNumber(history.code) == 0 ||
            !isfinite(history.total) || history.total < 0)
        {
            valid = 0;
            break;
        }
        errno = 0;
        long quantity = strtol(quantityText, &quantityEnd, 10);
        if (errno != 0 || quantityEnd == quantityText || hasContent(quantityEnd) ||
            quantity <= 0 || quantity > INT_MAX)
        {
            valid = 0;
            break;
        }
        history.quantity = (int)quantity;
        historyRecords[historyCount++] = history;
    }
    if (ferror(file))
    {
        valid = 0;
    }
    if (fclose(file) != 0)
    {
        valid = 0;
    }
    if (!valid)
    {
        fprintf(stderr, "Gagal memuat %s: periksa format atau kapasitas data.\n", HISTORY_FILE);
    }
    return valid;
}

static int readText(const char *prompt, char *text, int capacity)
{
    for (;;)
    {
        printf("%s", prompt);
        fflush(stdout);
        int status = readLine(stdin, text, capacity);
        if (status == 0)
        {
            return 0;
        }
        if (status == 1 && hasContent(text) && strchr(text, '|') == NULL)
        {
            return 1;
        }
        printf("Input wajib diisi, maksimal %d karakter, dan tidak boleh mengandung '|'.\n",
               capacity - 1);
    }
}

// Menu 2 : View History Transaksi Penjualan
<<<<<<< HEAD
void viewHistoryPenjualan(void) {
    if (historyCount == 0) {
=======
void viewHistoryPenjualan(void)
{
    if (historyCount == 0)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        puts("Tidak ada histori transaksi.");
        return;
    }
    printf("\n=== Histori Transaski Penjualan ===\n");
    printf("%-5s %-10s %-18s %s\n", "No", "Kode Buku", "Jumlah Terjual", "Total Harga");
<<<<<<< HEAD
    for (int i = 0; i < historyCount; i++) {
=======
    for (int i = 0; i < historyCount; i++)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        printf("%-5d %-10s %-18d %.2f\n",
               i + 1,
               historyRecords[i].code,
               historyRecords[i].quantity,
               historyRecords[i].total);
    }
}

// Menampilkan seluruh data buku beserta nomor index.
void viewBooks(void)
{
    if (bookCount == 0)
    {
        puts("Tidak ada data buku.");
        return;
    }

    printf("\n=== Daftar Buku ===\n");
    printf("%-5s %-10s %-25s %-20s %s\n",
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga");

    for (int i = 0; i < bookCount; i++)
    {
        printf("%-5d %-10s %-25s %-20s %.2f\n",
               i + 1, books[i].code, books[i].name,
               books[i].category, books[i].price);
    }
}

// Menghapus data buku berdasarkan nomor index.
void deleteBook(void)
{
    if (bookCount == 0)
    {
        puts("Tidak ada data buku untuk dihapus.");
        return;
    }

    viewBooks();

    int index;
    printf("\nMasukkan index buku yang ingin dihapus: ");

    char line[LINE_LENGTH];
    char *end;

    if (readLine(stdin, line, sizeof(line)) != 1)
    {
        puts("Input tidak valid.");
        return;
    }

    long number = strtol(line, &end, 10);

    if (end == line || hasContent(end) ||
        number < 1 || number > bookCount)
    {
        puts("Index tidak valid.");
        return;
    }

    index = (int)number;

    for (int i = index - 1; i < bookCount - 1; i++)
    {
        books[i] = books[i + 1];
    }

    bookCount--;

    puts("Data Successfully delete..");
}

// menampilkan data riwayat transaksi penjualan
<<<<<<< HEAD
void viewHistory(void) {
    // validasi jika riwayat kosong
    if (historyCount == 0) {
=======
void viewHistory(void)
{
    // validasi jika riwayat kosong
    if (historyCount == 0)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        puts("Tidak ada data histori transaksi.");
        return;
    }

    printf("\n=== Daftar Histori Transaksi ===\n");
    printf("%-5s %-10s %-15s %s\n", "No", "Kode Buku", "Jumlah Terjual", "Total Harga");

    // looping tampilkan isi riwayat
<<<<<<< HEAD
    for (int i = 0; i < historyCount; i++) {
=======
    for (int i = 0; i < historyCount; i++)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        printf("%-5d %-10s %-15d %.2f\n",
               i + 1, historyRecords[i].code,
               historyRecords[i].quantity, historyRecords[i].total);
    }
}

// menghapus data riwayat transaksi tertentu
<<<<<<< HEAD
void deleteHistory(void) {
    // validasi jika riwayat kosong
    if (historyCount == 0) {
=======
void deleteHistory(void)
{
    // validasi jika riwayat kosong
    if (historyCount == 0)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        puts("Tidak ada data histori transaksi untuk dihapus.");
        return;
    }

    // tampilkan daftar histori dulu
    viewHistory();

    int index;
    printf("\nMasukkan index history yang ingin dihapus: ");

    char line[LINE_LENGTH];
    char *end;

    // baca input user
<<<<<<< HEAD
    if (readLine(stdin, line, sizeof(line)) != 1) {
=======
    if (readLine(stdin, line, sizeof(line)) != 1)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        puts("Input tidak valid.");
        return;
    }

    long number = strtol(line, &end, 10);

    // validasi batas nomor index
    if (end == line || hasContent(end) ||
<<<<<<< HEAD
        number < 1 || number > historyCount) {
=======
        number < 1 || number > historyCount)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        puts("Index tidak valid.");
        return;
    }

    index = (int)number;

    // geser data ke kiri untuk menghapus
<<<<<<< HEAD
    for (int i = index - 1; i < historyCount - 1; i++) {
=======
    for (int i = index - 1; i < historyCount - 1; i++)
    {
>>>>>>> 1f2e902 (update input transaksi penjualan)
        historyRecords[i] = historyRecords[i + 1];
    }

    // kurangi jumlah data riwayat
    historyCount--;

    puts("Data Successfully delete..");
}
// Mengurutkan buku berdasarkan nama secara ascending menggunakan Bubble Sort.

// Mengurutkan buku berdasarkan nama secara ascending menggunakan Bubble Sort.
void bubbleSortByName(void)
{
    Book sortedBooks[MAX_BOOKS];
    Book temp;

    if (bookCount == 0)
    {
        puts("Tidak ada data buku untuk diurutkan.");
        return;
    }

    // Menyalin data buku agar urutan data asli tidak berubah.
    for (int i = 0; i < bookCount; i++)
    {
        sortedBooks[i] = books[i];
    }

    // Proses pengurutan menggunakan Bubble Sort.
    for (int i = 0; i < bookCount - 1; i++)
    {
        for (int j = 0; j < bookCount - i - 1; j++)
        {
            if (strcmp(sortedBooks[j].name, sortedBooks[j + 1].name) > 0)
            {
                temp = sortedBooks[j];
                sortedBooks[j] = sortedBooks[j + 1];
                sortedBooks[j + 1] = temp;
            }
        }
    }

    printf("\n=== Buku Berdasarkan Nama (A-Z) ===\n");
    printf("%-5s %-10s %-25s %-20s %s\n",
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga");

    for (int i = 0; i < bookCount; i++)
    {
        printf("%-5d %-10s %-25s %-20s %.2f\n",
               i + 1, sortedBooks[i].code, sortedBooks[i].name,
               sortedBooks[i].category, sortedBooks[i].price);
    }
}

// Mengurutkan buku berdasarkan harga secara descending menggunakan Selection Sort.
void selectionSortByPrice(void)
{
    Book sortedBooks[MAX_BOOKS];
    Book temp;
    int maxIndex;

    if (bookCount == 0)
    {
        puts("Tidak ada data buku untuk diurutkan.");
        return;
    }

    // Menyalin data buku agar urutan data asli tidak berubah.
    for (int i = 0; i < bookCount; i++)
    {
        sortedBooks[i] = books[i];
    }

    // Proses pengurutan menggunakan Selection Sort.
    for (int i = 0; i < bookCount - 1; i++)
    {
        maxIndex = i;

        for (int j = i + 1; j < bookCount; j++)
        {
            if (sortedBooks[j].price > sortedBooks[maxIndex].price)
            {
                maxIndex = j;
            }
        }

        if (maxIndex != i)
        {
            temp = sortedBooks[i];
            sortedBooks[i] = sortedBooks[maxIndex];
            sortedBooks[maxIndex] = temp;
        }
    }

    printf("\n=== Buku Berdasarkan Harga (Terbesar - Terkecil) ===\n");
    printf("%-5s %-10s %-25s %-20s %s\n",
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga");

    for (int i = 0; i < bookCount; i++)
    {
        printf("%-5d %-10s %-25s %-20s %.2f\n",
               i + 1, sortedBooks[i].code, sortedBooks[i].name,
               sortedBooks[i].category, sortedBooks[i].price);
    }
}

void sortBooks(void)
{
    char choice[16];

    for (;;)
    {
        puts("\n=== Menu Sorting Buku ===");
        puts("1. Urutkan Nama Buku (A-Z)");
        puts("2. Urutkan Harga (Terbesar - Terkecil)");
        puts("3. Kembali");
        printf("Pilihan: ");
        fflush(stdout);

        int status = readLine(stdin, choice, sizeof(choice));

        if (status == 0)
        {
            return;
        }

        if (status == 1 && strcmp(choice, "1") == 0)
        {
            bubbleSortByName();
        }
        else if (status == 1 && strcmp(choice, "2") == 0)
        {
            selectionSortByPrice();
        }
        else if (status == 1 && strcmp(choice, "3") == 0)
        {
            return;
        }
        else
        {
            puts("Pilihan tidak valid.");
        }
    }
}

void inputBook(void)
{
    if (bookCount == MAX_BOOKS)
    {
        puts("Kapasitas data buku sudah penuh.");
        return;
    }

    int highestCodeNumber = 0;
    for (int i = 0; i < bookCount; i++)
    {
        int number = getCodeNumber(books[i].code);
        if (number > highestCodeNumber)
        {
            highestCodeNumber = number;
        }
    }
    if (highestCodeNumber == INT_MAX)
    {
        puts("Kode buku baru tidak dapat dibuat.");
        return;
    }

    Book book;
    puts("\n=== Input Data Buku Baru ===");
    if (!readText("Nama buku  : ", book.name, sizeof(book.name)) ||
        !readText("Jenis buku : ", book.category, sizeof(book.category)))
    {
        puts("\nInput buku dibatalkan.");
        return;
    }

    for (;;)
    {
        char line[LINE_LENGTH];
        char *end;
        printf("Harga buku : ");
        fflush(stdout);
        int status = readLine(stdin, line, sizeof(line));
        if (status == 0)
        {
            puts("\nInput buku dibatalkan.");
            return;
        }
        errno = 0;
        book.price = strtod(line, &end);
        if (status == 1 && errno == 0 && end != line && !hasContent(end) &&
            isfinite(book.price) && book.price > 0)
        {
            break;
        }
        puts("Harga harus berupa angka positif. Gunakan titik untuk desimal.");
    }

    snprintf(book.code, sizeof(book.code), "BK%03d", highestCodeNumber + 1);
    books[bookCount++] = book;
    printf("Buku berhasil ditambahkan. Kode buku: %s\n", book.code);
}

void inputTransaction(void)
{
    if (bookCount == 0)
    {
        puts("Tidak ada data buku yang tersedia untuk transaksi.");
        return;
    }
    if (historyCount >= MAX_HISTORY)
    {
        puts("Kapasitas history transaksi sudah penuh.");
        return;
    }
    char codeInput[16];
    puts("\n=== Input Transaksi Penjualan ===");
    if (!readText("Masukkan kode buku (contoh BK001): ", codeInput, sizeof(codeInput)))
    {
        puts("Transaksi dibatalkan.");
        return;
    }
    int targetIdx = -1;
    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(books[i].code, codeInput) == 0)
        {
            targetIdx = i;
            break;
        }
    }
    if (targetIdx == -1)
    {
        printf("Kode buku '%s' tidak ditemukan.\n", codeInput);
        return;
    }
    int quantity = 0;
    for (;;)
    {
        char line[LINE_LENGTH];
        char *end;
        printf("Masukkan jumlah terjual : ");
        fflush(stdout);
        int status = readLine(stdin, line, sizeof(line));
        if (status == 0)
        {
            puts("Transaksi dibatalkan.");
            return;
        }
        errno = 0;
        long qtyNum = strtol(line, &end, 10);
        if (status == 1 && errno == 0 && end != line && !hasContent(end) &&
            qtyNum > 0 && qtyNum <= INT_MAX)
        {
            quantity = (int)qtyNum;
            break;
        }
        puts("Jumlah terjual harus berupa angka bulat positif.");
    }

    double total = quantity * books[targetIdx].price;
    if (!isfinite(total) || total > 1e308)
    {
        puts("Total harga melebihi batas maksimal yang diizinkan.");
        return; 
    }

    History newHistory;
    strcpy(newHistory.code, books[targetIdx].code);
    newHistory.quantity = quantity;
    newHistory.total = total;

    historyRecords[historyCount++] = newHistory;

    FILE *historyFile = fopen(HISTORY_FILE, "a");
    if (historyFile != NULL)
    {
        fprintf(historyFile, "%s|%d|%.17g\n", newHistory.code, newHistory.quantity, newHistory.total);
        fclose(historyFile);
    }

    printf("Transaksi berhasil dicatat.\n");
    printf("Total Harga: %.2f\n", total);
}

int saveData(void)
{
    FILE *book = fopen(BOOK_FILE ".tmp", "w");
    if (book == NULL)
    {
        perror(BOOK_FILE ".tmp");
        return 0;
    }
    FILE *history = fopen(HISTORY_FILE ".tmp", "w");
    if (history == NULL)
    {
        perror(HISTORY_FILE ".tmp");
        fclose(book);
        remove(BOOK_FILE ".tmp");
        return 0;
    }

    int success = 1;
    for (int i = 0; i < bookCount; i++)
    {
        if (fprintf(book, "%s|%s|%s|%.17g\n", books[i].code,
                    books[i].name, books[i].category, books[i].price) < 0)
        {
            success = 0;
        }
    }
    for (int i = 0; i < historyCount; i++)
    {
        if (fprintf(history, "%s|%d|%.17g\n", historyRecords[i].code,
                    historyRecords[i].quantity, historyRecords[i].total) < 0)
        {
            success = 0;
        }
    }
    if (fclose(book) != 0)
    {
        success = 0;
    }
    if (fclose(history) != 0)
    {
        success = 0;
    }
    if (!success)
    {
        fputs("Gagal menulis file data.\n", stderr);
        remove(BOOK_FILE ".tmp");
        remove(HISTORY_FILE ".tmp");
        return 0;
    }
    if (rename(BOOK_FILE ".tmp", BOOK_FILE) != 0 ||
        rename(HISTORY_FILE ".tmp", HISTORY_FILE) != 0)
    {
        perror("Gagal mengganti file data");
        return 0;
    }
    return 1;
}

int main(void)
{
    if (!loadBooks() || !loadHistory())
    {
        return EXIT_FAILURE;
    }

    for (;;)
    {
        char choice[16];

        puts("1. Input Data Buku Baru");
        puts("2. View History Transaksi Penjualan");
        puts("3. View Buku");
        puts("4. Delete History");
        puts("5. Delete Buku");
        puts("7. Input Transaksi Penjualan");
        puts("8. Sort Buku");
        puts("6. Simpan dan Keluar");

        printf("Pilihan: ");
        fflush(stdout);

        int status = readLine(stdin, choice, sizeof(choice));
        if (status == 0)
        {
            puts("\nInput berakhir. Menyimpan data sebelum keluar.");
            return saveData() ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        if (status == 1 && strcmp(choice, "1") == 0)
        {
            inputBook();
        } else if (status == 1 && strcmp(choice, "2") == 0) {
            viewHistoryPenjualan();
        }else if (status == 1 && strcmp(choice, "3") == 0) {
            viewBooks();
        } else if (status == 1 && strcmp(choice, "4") == 0) {
            deleteHistory();
        }
        else if (status == 1 && strcmp(choice, "2") == 0)
        {
            viewHistoryPenjualan();
        }
        else if (status == 1 && strcmp(choice, "3") == 0)
        {
            viewBooks();
        }
        else if (status == 1 && strcmp(choice, "4") == 0)
        {
            deleteHistory();
        }
        else if (status == 1 && strcmp(choice, "5") == 0)
        {
            deleteBook();
        }
        else if (status == 1 && strcmp(choice, "7") == 0)
        {
            inputTransaction();
        }
        else if (status == 1 && strcmp(choice, "8") == 0)
        {
            sortBooks();
        }
        else if (status == 1 && strcmp(choice, "6") == 0)
        {
            if (saveData())
            {
                puts("Data berhasil disimpan. Terima kasih!");
                return EXIT_SUCCESS;
            }
            puts("Data gagal disimpan. Periksa file lalu pilih menu 6 lagi.");
<<<<<<< HEAD
        } else {
           puts("Pilihan tidak valid. Pilih menu 1, 2, 3, 4, 5, 7, 8, atau 6.");
=======
        }
        else
        {
            puts("Pilihan tidak valid. Pilih menu 1, 2, 3, 4, 5, 7, 8, atau 6.");
>>>>>>> 1f2e902 (update input transaksi penjualan)
        }
    }
}
