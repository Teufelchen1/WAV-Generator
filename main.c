#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { NONE, FULL, HALF, QUARTER };


char sGroupIDriff[4] = "RIFF";
unsigned int dwFileLength = 0;
char sRiffType[4] = "WAVE";

char sGroupIDfmt[4] = "fmt ";
unsigned int dwChunkSizefmt = 18;
unsigned short wFormatTag = 1;
unsigned short wChannels = 1;
unsigned int dwSamplesPerSec = 48000;
unsigned int dwAvgBytesPerSec = 48000;//sampleRate * blockAlign;
unsigned short wBlockAlign = 1; //wChannels * (dwBitsPerSample / 8) -> 1 * (8/8)
unsigned int dwBitsPerSample = 8;

char sGroupIDdata[4] = "data";
unsigned int dwChunkSizedata = 0;
//unsigned char data[8000];

void *knalloc(size_t size){
	void *ptr = malloc(size);
	if(ptr == NULL){
		printf("Memomry allocation failed! Out of memory?");
		exit(-1);
	}
	return ptr;
}

int main(void) {
	int data_len = 48000*20; // 10s sound
	unsigned char * data = knalloc(data_len);
	printf("Data length: %i\n", data_len);
	int LEN = (data_len + sizeof(dwChunkSizefmt) + sizeof(sGroupIDdata) + sizeof(dwBitsPerSample) + sizeof(wBlockAlign) + sizeof(dwAvgBytesPerSec) + 
			sizeof(dwSamplesPerSec) + sizeof(wChannels) + sizeof(wFormatTag) + sizeof(dwChunkSizedata) + sizeof(sGroupIDfmt) + sizeof(sRiffType) + sizeof(dwFileLength) + sizeof(sGroupIDriff));
	unsigned char * writeData = knalloc(LEN);
	printf("File length: %i\n", LEN);
	dwFileLength = LEN-8;
	dwChunkSizefmt = sizeof(wFormatTag) + sizeof(wChannels) + sizeof(dwSamplesPerSec) + sizeof(dwAvgBytesPerSec) + sizeof(wBlockAlign) + sizeof(dwBitsPerSample);
	dwChunkSizedata = data_len;
	printf("Format length: %i\n", dwChunkSizefmt);
	printf("WAV length: %i\n", dwFileLength);
	void *ptr = memcpy(writeData, &sGroupIDriff, 4);
	ptr = ptr + sizeof(sGroupIDriff);
	ptr = memcpy(ptr, &dwFileLength, sizeof(dwFileLength)) + sizeof(dwFileLength);
	ptr = memcpy(ptr, &sRiffType, sizeof(sRiffType)) + sizeof(sRiffType);
	ptr = memcpy(ptr, &sGroupIDfmt, sizeof(sGroupIDfmt)) + sizeof(sGroupIDfmt);
	ptr = memcpy(ptr, &dwChunkSizefmt, sizeof(dwChunkSizefmt)) + sizeof(dwChunkSizefmt);
	ptr = memcpy(ptr, &wFormatTag, sizeof(wFormatTag)) + sizeof(wFormatTag);
	ptr = memcpy(ptr, &wChannels, sizeof(wChannels)) + sizeof(wChannels);
	ptr = memcpy(ptr, &dwSamplesPerSec, sizeof(dwSamplesPerSec)) + sizeof(dwSamplesPerSec);
	ptr = memcpy(ptr, &dwAvgBytesPerSec, sizeof(dwAvgBytesPerSec)) + sizeof(dwAvgBytesPerSec);
	ptr = memcpy(ptr, &wBlockAlign, sizeof(wBlockAlign)) + sizeof(wBlockAlign);
	ptr = memcpy(ptr, &dwBitsPerSample, sizeof(dwBitsPerSample)) + sizeof(dwBitsPerSample);
	ptr = memcpy(ptr, &sGroupIDdata, sizeof(sGroupIDdata)) + sizeof(sGroupIDdata);
	ptr = memcpy(ptr, &dwChunkSizedata, sizeof(dwChunkSizedata)) + sizeof(dwChunkSizedata);
	memset(data, 0x00, data_len);
	int i = 0;
	int count = 0;
	int state = NONE;
	int p = 400;
	int sub = -2;
	unsigned char amplitude = 0xc0;
	int lambda = 2;
	for(i = 0; i < data_len; i++){
		if(p >= lambda){
			if(amplitude > 0x00){
				amplitude = 0x00;
			} else {
				amplitude = 0xc0;
			}

			p = 0;
		}
	//	printf("%i",count);
		if(count == 600){/*
			lambda += sub;
			if(lambda <= 0){
				lambda = 0;
				sub *= -1;
			}
			if(lambda >= 500){
				lambda = 500;
				sub *= -1;
			}*/
			count = 0;
		}
		count += 1;
		p += 1;
		data[i] = amplitude;
	}
	ptr = memcpy(ptr, data, data_len);

	FILE *fp;
	fp = fopen("test2.wav", "w");
	fwrite(writeData, 1, LEN, fp);
	fclose(fp);
	free(writeData);
	return 0;
}
