/*
 * encoder.c
 *
 * A simple C program that reads a binary data string from a file and
 * encodes it into various line coding schemes:
 *   1. NRZ-L (Non-Return-to-Zero-Level)
 *   2. NRZ-I (Non-Return-to-Zero-Inverted)
 *   3. Bi-phase (Alternate Mark Inversion)
 *   4. Manchester (IEEE 802.3)
 *   5. Differential Manchester
 *
 * Usage:
 *   Run the following:
 *   "gcc -std=c11 -Wall -Wextra PROJECT.c -o ENCODE"
 *   "./ENCODE INPUT.DATA CODE"
 * where CODE is:
 *   1 => NRZ-L
 *   2 => NRZ-I
 *   3 => Bi-phase
 *   4 => Manchester
 *   5 => Differential Manchester
 *
 * The encoded waveform is written to OUTPUT.SIGNAL in a simple textual form:
 * e.g. "|+V|-V|+V|...|"
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /* Function prototypes for each encoding scheme */
 void encode_nrzl(const char *data, FILE *out);
 void encode_nrzi(const char *data, FILE *out);
 void encode_manchester(const char *data, FILE *out);
 void encode_diff_manchester(const char *data, FILE *out);
 void encode_biphase(const char *data, FILE *out);
 
 /*
  * NRZ-L: Non-Return-to-Zero-Level
  * '0' maps to a -V level, '1' maps to a +V level
  */
 void encode_nrzl(const char *data, FILE *out) {
     fprintf(out, "NRZ-L ENCODING SCHEME: ");
     for (int i = 0; data[i] != '\0'; i++) {
         if (data[i] == '0') {
             fprintf(out, "|-V");  // low level for 0
         } else {
             fprintf(out, "|+V");  // high level for 1
         }
     }
     fprintf(out, "|\n");         // close the waveform line
 }
 
 /*
  * NRZ-I: Non-Return-to-Zero Inverted
  * A '1' causes a transition (invert level) and '0' holds the level
  */
 void encode_nrzi(const char *data, FILE *out) {
     fprintf(out, "NRZ-I ENCODING SCHEME: ");
     char last = '-';              // start at negative level
     for (int i = 0; data[i]; i++) {
         if (data[i] == '1') {
             // invert the level on a '1'
             last = (last == '+') ? '-' : '+';
         }
         fprintf(out, "|%cV", last);
     }
     fprintf(out, "|\n");
 }
 
 /*
  * Manchester: Single-transition per bit
  * '0' is encoded as (+V then -V), '1' is (-V then +V)
  */
 void encode_manchester(const char *data, FILE *out) {
     fprintf(out, "MANCHESTER ENCODING SCHEME: ");
     for (int i = 0; data[i]; i++) {
         if (data[i] == '0') {
             fprintf(out, "|+V|-V");  // low-to-high mid-bit
         } else {
             fprintf(out, "|-V|+V");  // high-to-low mid-bit
         }
     }
     fprintf(out, "|\n");
 }
 
 /*
  * Differential Manchester: Transition at middle of each bit;
  * '0' has an additional transition at start of bit, '1' does not
  */
 void encode_diff_manchester(const char *data, FILE *out) {
     fprintf(out, "DIFFERENTIAL MANCHESTER ENCODING SCHEME: ");
     char last = '+';  // track current level, start positive
     for (int i = 0; data[i]; i++) {
         if (data[i] == '0') {
             // transition at start for '0'
             last = (last == '+') ? '-' : '+';
             fprintf(out, "|%cV", last);
             // always transition at mid-bit
             last = (last == '+') ? '-' : '+';
             fprintf(out, "|%cV", last);
         } else {
             // no start transition for '1', only mid-bit
             fprintf(out, "|%cV", last);
             last = (last == '+') ? '-' : '+';
             fprintf(out, "|%cV", last);
         }
     }
     fprintf(out, "|\n");
 }
 
 /*
  * Bi-phase (Alternate Mark Inversion):
  * '0' outputs a zero-level marker, '1' alternates polarity
  */
 void encode_biphase(const char *data, FILE *out) {
     fprintf(out, "B-AMI ENCODING SCHEME: ");
     char last = '-';  // track last polarity
     for (int i = 0; data[i]; i++) {
         if (data[i] == '0') {
             fprintf(out, "|0V");  // zero marker for 0
         } else {
             // invert on each '1'
             last = (last == '+') ? '-' : '+';
             fprintf(out, "|%cV", last);
         }
     }
     fprintf(out, "|\n");
 }
 
 int main(int argc, char *argv[]) {
     /* Check for correct number of arguments */
     if (argc != 3) {
         fprintf(stderr, "Usage: %s INPUT.DATA CODE\n", argv[0]);
         return EXIT_FAILURE;
     }
 
     /* Open input file containing bit string */
     FILE *in = fopen(argv[1], "r");
     if (in == NULL) {
         perror("Error opening input file");
         return EXIT_FAILURE;
     }
 
     /* Read the binary data up to 1023 characters */
     char data[1024];
     if (fscanf(in, "%1023s", data) != 1) {
         fprintf(stderr, "Failed to read data from %s\n", argv[1]);
         fclose(in);
         return EXIT_FAILURE;
     }
     fclose(in);
 
     /* Open output file for writing the encoded waveform */
     FILE *out = fopen("OUTPUT.SIGNAL", "w");
     if (out == NULL) {
         perror("Error opening output file");
         return EXIT_FAILURE;
     }
 
     /* Dispatch to the chosen coding scheme */
     int code = atoi(argv[2]);
     switch (code) {
         case 1:
             encode_nrzl(data, out);
             break;
         case 2:
             encode_nrzi(data, out);
             break;
         case 3:
             encode_biphase(data, out);
             break;
         case 4:
             encode_manchester(data, out);
             break;
         case 5:
             encode_diff_manchester(data, out);
             break;
         default:
             fprintf(stderr, "Invalid coding scheme: %d\n", code);
             fclose(out);
             return EXIT_FAILURE;
     }
 
     fclose(out);
     return EXIT_SUCCESS;
 }
 