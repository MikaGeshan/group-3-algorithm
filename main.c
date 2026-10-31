/*
 * Toko Buku Literasi Nusantara Jaya - Menu 1 dan 6
 * Anggota kelompok: isi nama dan NIM anggota kelompok di sini.
 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 1000
#define MAX_HISTORY 1000
#define LINE_LENGTH 256
#define BOOK_FILE "databuku.txt"
#define HISTORY_FILE "history.txt"

typedef struct {
    char code[16];
    char name[101];
    char category[51];
    double price;
} Book;

typedef struct {
    char code[16];
    int quantity;
    double total;
} History;

Book books[MAX_BOOKS];
History historyRecords[MAX_HISTORY];
int bookCount = 0;
int historyCount = 0;

static int readLine(FILE *file, char *text, int capacity) {
    if (fgets(text, capacity, file) == NULL) {
        return 0;
    }

    if (strchr(text, '\n') == NULL) {
        int character = fgetc(file);
        if (character != '\n' && character != EOF) {
            while ((character = fgetc(file)) != '\n' && character != EOF) {
            }
            return -1;
        }
    }

    text[strcspn(text, "\r\n")] = '\0';
    return 1;
}

static int hasContent(const char *text) {
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) {
            return 1;
        }
        text++;
    }
    return 0;
}

static int getCodeNumber(const char *code) {
    if (strncmp(code, "BK", 2) != 0 || code[2] == '\0') {
        return 0;
    }
    for (const char *p = code + 2; *p != '\0'; p++) {
        if (!isdigit((unsigned char)*p)) {
            return 0;
        }
    }
    errno = 0;
    long number = strtol(code + 2, NULL, 10);
    return errno == 0 && number > 0 && number <= INT_MAX ? (int)number : 0;
}

int loadBooks(void) {
    FILE *file = fopen(BOOK_FILE, "r");
    if (file == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        perror(BOOK_FILE);
        return 0;
    }

    char line[LINE_LENGTH];
    int status;
    int valid = 1;
    while ((status = readLine(file, line, sizeof(line))) != 0) {
        Book book;
        int end = 0;
        if (status < 0 || bookCount == MAX_BOOKS ||
            sscanf(line, "%15[^|]|%100[^|]|%50[^|]|%lf%n",
                   book.code, book.name, book.category, &book.price, &end) != 4 ||
            hasContent(line + end) || getCodeNumber(book.code) == 0 ||
            !hasContent(book.name) || !hasContent(book.category) ||
            !isfinite(book.price) || book.price <= 0) {
            valid = 0;
            break;
        }
        for (int i = 0; i < bookCount; i++) {
            if (getCodeNumber(books[i].code) == getCodeNumber(book.code)) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            break;
        }
        books[bookCount++] = book;
    }
    if (ferror(file)) {
        valid = 0;
    }
    if (fclose(file) != 0) {
        valid = 0;
    }
    if (!valid) {
        fprintf(stderr, "Gagal memuat %s: periksa format atau kapasitas data.\n", BOOK_FILE);
    }
    return valid;
}

int loadHistory(void) {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        perror(HISTORY_FILE);
        return 0;
    }

    char line[LINE_LENGTH];
    int status;
    int valid = 1;
    while ((status = readLine(file, line, sizeof(line))) != 0) {
        History history;
        char quantityText[32];
        char *quantityEnd;
        int end = 0;
        if (status < 0 || historyCount == MAX_HISTORY ||
            sscanf(line, "%15[^|]|%31[^|]|%lf%n", history.code,
                   quantityText, &history.total, &end) != 3 ||
            hasContent(line + end) || getCodeNumber(history.code) == 0 ||
            !isfinite(history.total) || history.total < 0) {
            valid = 0;
            break;
        }
        errno = 0;
        long quantity = strtol(quantityText, &quantityEnd, 10);
        if (errno != 0 || quantityEnd == quantityText || hasContent(quantityEnd) ||
            quantity <= 0 || quantity > INT_MAX) {
            valid = 0;
            break;
        }
        history.quantity = (int)quantity;
        historyRecords[historyCount++] = history;
    }
    if (ferror(file)) {
        valid = 0;
    }
    if (fclose(file) != 0) {
        valid = 0;
    }
    if (!valid) {
        fprintf(stderr, "Gagal memuat %s: periksa format atau kapasitas data.\n", HISTORY_FILE);
    }
    return valid;
}

static int readText(const char *prompt, char *text, int capacity) {
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        int status = readLine(stdin, text, capacity);
        if (status == 0) {
            return 0;
        }
        if (status == 1 && hasContent(text) && strchr(text, '|') == NULL) {
            return 1;
        }
        printf("Input wajib diisi, maksimal %d karakter, dan tidak boleh mengandung '|'.\n",
               capacity - 1);
    }
}

void inputBook(void) {
    if (bookCount == MAX_BOOKS) {
        puts("Kapasitas data buku sudah penuh.");
        return;
    }

    int highestCodeNumber = 0;
    for (int i = 0; i < bookCount; i++) {
        int number = getCodeNumber(books[i].code);
        if (number > highestCodeNumber) {
            highestCodeNumber = number;
        }
    }
    if (highestCodeNumber == INT_MAX) {
        puts("Kode buku baru tidak dapat dibuat.");
        return;
    }

    Book book;
    puts("\n=== Input Data Buku Baru ===");
    if (!readText("Nama buku  : ", book.name, sizeof(book.name)) ||
        !readText("Jenis buku : ", book.category, sizeof(book.category))) {
        puts("\nInput buku dibatalkan.");
        return;
    }

    for (;;) {
        char line[LINE_LENGTH];
        char *end;
        printf("Harga buku : ");
        fflush(stdout);
        int status = readLine(stdin, line, sizeof(line));
        if (status == 0) {
            puts("\nInput buku dibatalkan.");
            return;
        }
        errno = 0;
        book.price = strtod(line, &end);
        if (status == 1 && errno == 0 && end != line && !hasContent(end) &&
            isfinite(book.price) && book.price > 0) {
            break;
        }
        puts("Harga harus berupa angka positif. Gunakan titik untuk desimal.");
    }

    snprintf(book.code, sizeof(book.code), "BK%03d", highestCodeNumber + 1);
    books[bookCount++] = book;
    printf("Buku berhasil ditambahkan. Kode buku: %s\n", book.code);
}

int saveData(void) {
    FILE *book = fopen(BOOK_FILE ".tmp", "w");
    if (book == NULL) {
        perror(BOOK_FILE ".tmp");
        return 0;
    }
    FILE *history = fopen(HISTORY_FILE ".tmp", "w");
    if (history == NULL) {
        perror(HISTORY_FILE ".tmp");
        fclose(book);
        remove(BOOK_FILE ".tmp");
        return 0;
    }

    int success = 1;
    for (int i = 0; i < bookCount; i++) {
        if (fprintf(book, "%s|%s|%s|%.17g\n", books[i].code,
                    books[i].name, books[i].category, books[i].price) < 0) {
            success = 0;
        }
    }
    for (int i = 0; i < historyCount; i++) {
        if (fprintf(history, "%s|%d|%.17g\n", historyRecords[i].code,
                    historyRecords[i].quantity, historyRecords[i].total) < 0) {
            success = 0;
        }
    }
    if (fclose(book) != 0) {
        success = 0;
    }
    if (fclose(history) != 0) {
        success = 0;
    }
    if (!success) {
        fputs("Gagal menulis file data.\n", stderr);
        remove(BOOK_FILE ".tmp");
        remove(HISTORY_FILE ".tmp");
        return 0;
    }
    if (rename(BOOK_FILE ".tmp", BOOK_FILE) != 0 ||
        rename(HISTORY_FILE ".tmp", HISTORY_FILE) != 0) {
        perror("Gagal mengganti file data");
        return 0;
    }
    return 1;
}

int main(void) {
    if (!loadBooks() || !loadHistory()) {
        return EXIT_FAILURE;
    }

    for (;;) {
        char choice[16];
        puts("1. Input Data Buku Baru");
        puts("6. Simpan dan Keluar");
        printf("Pilihan: ");
        fflush(stdout);
        int status = readLine(stdin, choice, sizeof(choice));
        if (status == 0) {
            puts("\nInput berakhir. Menyimpan data sebelum keluar.");
            return saveData() ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        if (status == 1 && strcmp(choice, "1") == 0) {
            inputBook();
        } else if (status == 1 && strcmp(choice, "6") == 0) {
            if (saveData()) {
                puts("Data berhasil disimpan. Terima kasih!");
                return EXIT_SUCCESS;
            }
            puts("Data gagal disimpan. Periksa file lalu pilih menu 6 lagi.");
        } else {
            puts("Pilihan tidak valid. Pilih menu 1 atau 6.");
        }
    }
}
