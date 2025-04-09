#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encode_nrzl(const char *data, FILE *out);
void encode_nrzi(const char *data, FILE *out);
void encode_manchester(const char *data, FILE *out);
void encode_diff_manchester(const char *data, FILE *out);
void encode_biphase(const char *data, FILE *out);

void encode_nrzl(const char *data, FILE *out){
    for(int i = 0; data[i] != '\0'; i++){
        if(data[i] == '0'){
            fprintf(out, "|-V");
        } else {
            fprintf(out, "|+V");
        }
    }
    fprintf(out, "|\n");
}

void encode_nrzi(const char *data, FILE *out){
    char last = '-';
    for(int i = 0; data[i]; i++){
        if(data[i] == '1'){
            last = (last == '+') ? '-' : '+';
        }
        fprintf(out, "|%cV", last);
    }
    fprintf(out, "|\n");
}

void encode_manchester(const char *data, FILE *out){
    for(int i = 0; data[i]; i++){
        if(data[i] == '0'){
            fprintf(out, "|+V|-V");
        } else {
            fprintf(out, "|-V|+V");
        }
    }
    fprintf(out, "|\n");
}

void encode_diff_manchester(const char *data, FILE *out){
    char last = '+';
    for(int i = 0; data[i]; i++){
        if(data[i] == '0'){
            last = (last == '+') ? '-' : '+';
            fprintf(out, "|%cV", last); // Start of bit
            last = (last == '+') ? '-' : '+'; // Mid-bit transition
            fprintf(out, "|%cV", last);
        }
        else{
            // no transition at beginning, only mid-bit transition
            fprintf(out, "|%cV", last);
            last = (last == '+') ? '-' : '+';
            fprintf(out, "|%cV", last);
        }
    }
    fprintf(out, "|\n");
}

void encode_biphase(const char *data, FILE *out){
    char last = '-';
    for(int i = 0; data[i]; i++){
        if(data[i] == '0'){
            fprintf(out, "|0V");
        } else {
            last = (last == '+') ? '-' : '+';
            fprintf(out, "|%cV", last);
        }
    }
    fprintf(out, "|\n");
}

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s INPUT.DATA CODE\n", argv[0]);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if(in == NULL){
        perror("Error opening input file");
        return 1;
    }

    char data[1024];
    fscanf(in, "%s", data);
    fclose(in);

    FILE *out = fopen("OUTPUT.SIGNAL", "w");
    if(!out){
        perror("Error opening output file");
        return 1;
    }

    int code = atoi(argv[2]);
    switch(code){
        case 1:
            encode_nrzl(data, out);
            break;
        case 2:
            encode_nrzi(data, out);
            break;
        case 3:
            encode_manchester(data, out);
            break;
        case 4:
            encode_diff_manchester(data, out);
            break;
        case 5:
            encode_biphase(data, out);
            break;
        default:
            fprintf(stderr, "Invalid coding scheme\n");
            fclose(out);
            return 1;
    }
    fclose(out);
    return 0;
}