# HOMEWORK 4 EXECUTION INSTRUCTIONS

- ## Problem 1
Use `gcc p1.c -o p1` to compile, 
`./p1 Tarea_4_w_years.csv <year | name> <filename output>` to execute.
This will create a file with the given filename of the ordered array of authors
in the original csv file.


- ## Problem 2
Use `gcc pgm_io.c wav_io.c p2.c -o p2` to compile, 
`./p2 <encryption-key> <file-input> <file-output>` to execute. The `file-input`
will be encrypted (with seed = `encryption-key`) using the Predator-Prey model,
and the result is sent to `file-output`. To decrpyt a file, run the same
program, with the same key, but now the encrypted file is the input file. Note
that one may use various keys to create a more complex encryption, that is, we
must know all `n` keys which are used in order to decrpyt the input file.

- ## Problem 3
Use `gcc p3.c pgm_io.c -lm -o p3` to compile, `./p3 <file-input>` to execute.
This will print the Shannon entropy (up to a certain floating point precision)
of the input file. The printed value tells us how much information a particular
pixel reveals. For `lena_ascii.pgm`, we get `7.444935`, which tells us that
almost all values in the range (0, 255) are possible in the image. The maximum
for an 8-bit image is an entropy of `8`.

