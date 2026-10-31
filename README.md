# group-3-algorithm

C practicum for Toko Buku Literasi Nusantara Jaya. This contribution implements:

- **Menu 1:** enter a book's name, category, and positive price. The program assigns
  a code such as `BK001` and adds the book to the array.
- **Menu 6:** save the book and transaction arrays, then exit. If saving fails,
  the program reports the error and allows another attempt.

Open the project in CLion and run the `team_assignment_lab` target. Only choices
`1` and `6` are available in this contribution.

Data files are loaded from and saved to the program's **working directory**,
which may be the build directory in CLion. Missing files start with empty arrays
and are created when saving. Existing malformed files stop startup so they are
not overwritten with incomplete data.

The assignment's file formats are used without header rows:

```text
databuku.txt: KodeBuku|NamaBuku|JenisBuku|Harga
history.txt:  KodeBuku|JumlahTerjual|TotalHarga
```

Example records:

```text
BK001|Logika dan Algoritma|Pendidikan|75000
BK001|2|150000
```

The first example belongs in `databuku.txt`; the second belongs in `history.txt`.
Use a dot for decimal prices. Names and categories may contain spaces but cannot
contain `|`, the file separator. Limits are 1,000 books, 1,000 history records,
100 characters per book name, and 50 per category.

For team integration, `daftarBuku`, `jumlahBuku`, `daftarHistory`, and
`jumlahHistory` hold the shared data. Startup calls `muatDataBuku()` and
`muatHistory()`; menu 1 calls `inputDataBuku()`; menu 6 calls `simpanData()`.
New codes use the highest existing code number plus one to avoid duplicates
after deletions. Menus 2–5, 7, and 8 can be added to the main loop by teammates.

Fill in the group members' names and NIMs in the comment at the top of `main.c`.
