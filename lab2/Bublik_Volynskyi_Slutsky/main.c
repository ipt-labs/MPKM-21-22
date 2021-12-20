#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define L 255
												//Quantiles for alpha equal to
float alph0 = 2.327,		//0.01
		  alph1 = 1.645,		//0.05
			alph2 = 1.282;		//0.1

void zeroize (unsigned char *arr, int len) {
	
	for (int i = 0; i < len; i++)
		arr[i] = 0;
}

void zeroize_int (int *arr, int len) {
	
	for (int i = 0; i < len; i++)
		arr[i] = 0;
}

unsigned char *librarian (int len, char *path) {
	
	unsigned char *seq = (unsigned char *) malloc(len * sizeof(char));
	FILE *file;
	file = fopen("randoutput.txt", "r");

	if (file != NULL) {
		fread(seq, 1, len, file);
	}
	else {
		puts("Can't read file.");
	}
	
	return seq;
}

void uniformity_test (unsigned char *seq, int len, float quantile, float alpha) {

	int bytes[256];
	int n_j = len / 256;
	float stat = 0,
			  hi_def = sqrt(2 * L) * quantile + L;
	unsigned num = 0;

	zeroize_int(bytes, 256);

	for (int i = 0; i < len; i++) {
		bytes[seq[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		stat += (n_j - bytes[i]) * (n_j - bytes[i]) / n_j;
	}

	printf("\t%.4f\t%.4f\t%.4f\t", alpha, hi_def, stat);
	
	if (stat <= hi_def) {
		puts("true");
	}
	else 
		puts("false");

	return;
}

void independency_test (unsigned char *seq, int len, float quantile, float alpha) {

	int bytes_i[256],
			bytes_j[256],
			pairs[256][256];			 
	float stat = 0,
			  hi_def = sqrt(2 * L * L) * quantile + L * L;
	
	for (int i = 0; i < 256; i++) {
		bytes_i[i] = 1;
		bytes_j[i] = 1;
		zeroize_int(pairs[i], 256);
	}
	for (int i = 0; i < len / 2; i++) {
		pairs[seq[2 * i]][seq[2 * i + 1]]++;
		bytes_i[seq[2 * i]]++;
		bytes_j[seq[2 * i + 1]]++;
	}
	
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			stat += pow(pairs[i][j], 2) / (bytes_i[i] * bytes_j[j]);
		}
	}
	
	stat = (stat - 1) * len / 2;

	printf("\t%.4f\t%.4f\t%.4f\t", alpha, hi_def, stat);
	
	if (stat <= hi_def) {
		puts("true");
	}
	else 
		puts("false");

	return;
}

void homogen_test (unsigned char *seq, int len, float quantile, float alpha) {
	
	int segm = 1000;
	int bytes[256], m = (int) len / segm;
	float stat = 0,
			  hi_def = sqrt(2 * L * L) * quantile + L * (segm - 1);
	
	zeroize_int(bytes, 256);
	for (int i = 0; i < len; i++) {
		bytes[seq[i]]++;
	}

	int *freq_by_segm[256];
	for (int i = 0; i < 256; i++) {
		freq_by_segm[i] = (int *) malloc(segm * sizeof(int));
		zeroize_int(freq_by_segm[i], segm);
	}

	for (int i = 0; i < len; i += m) {
		for (int j = 0; j < m; j++) {
			freq_by_segm[seq[i + j]][i / m]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < segm; j++) {
			if (bytes[i] == 0)
				continue;
			else {
				stat += pow(freq_by_segm[i][j], 2) / (m * bytes[i]);
			}
		}
	}

	stat = (stat - 1) * segm * m;
	printf("\t%.4f\t%.4f\t%.4f\t", alpha, hi_def, stat);
	
	if (stat <= hi_def) {
		puts("true");
	}
	else 
		puts("false");

	return;
}

void test_sequence (unsigned char *seq, char *seq_name, int len) {

	printf("\nGenerator: %s\n\n", seq_name);
	
	puts("Uniformity:");
	puts("\talpha\tCB\t\t\tAS");
	uniformity_test(seq, len, alph2, 0.1);
	uniformity_test(seq, len, alph1, 0.05);
	uniformity_test(seq, len, alph0, 0.01);
	
	puts("\nIndependency:");
	puts("\talpha\tCB\t\t\tAS");
	independency_test(seq, len, alph2, 0.1);
	independency_test(seq, len, alph1, 0.05);
	independency_test(seq, len, alph0, 0.01);
	
	puts("\nHomogenity:");
	puts("\talpha\tCB\t\t\tAS");
	homogen_test(seq, len, alph2, 0.1);
	homogen_test(seq, len, alph1, 0.05);
	homogen_test(seq, len, alph0, 0.01);

	return;
}

void seq_sample (unsigned char *seq, char *string, int len) {

	for (int i = 0; i < len; i++)
		for (int j = 0; j < 8; j++)
			printf("%d", (unsigned) (seq[i] >> j) % 2);
	
	puts("");
}

int main() {
	unsigned char *seq;
	size_t len = 1000000,
				len_sample = 100;

	seq = librarian(200000, "text.txt");
	test_sequence(seq, "PyCrypto (Random.new())", 200000);
}