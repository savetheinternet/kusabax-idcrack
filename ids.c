/* 
 *  Copyright (c) 2011 Michael Save <savetheinternet@tinyboard.org>
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Read the README provided for more information.
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#include <openssl/md5.h>

char *
ultoip(unsigned long ul)
{
	static char ip[16];
	char t[16];
	char *tp;
	unsigned long tl;
	ip[0] = '\0';

	do {
		tp = t;
		tl = ul % 256;
		ul /= 256;
		if (ul)
			*t = '.', ++tp;
		sprintf(tp, "%lu", tl);
		strcat(t, ip);
		strcpy(ip, t);
	} while (ul);
	return ip;
}

int
main(int argc, char *argv[])
{	
	
	if(argc != 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: ids <database> <poster id>\n");
		fprintf(stderr, "   or: ids <database> --generate\n");
		exit(0);
	}
	
	char *database = argv[1];
	
	if(strcmp(argv[2], "--generate") == 0) {
		// generate rainbow table
		FILE *fp = fopen(database, "w");
		if(fp == NULL) {
			fprintf(stderr, "Error opening %s for writing!\n", database);
			return 1;
		}
		
		char *hash = malloc(7);
		
		unsigned long x;
		char *ip;
		for(x = 16843009; x < 4294967295 ; x++) {
			ip = ultoip(x);
			
			// create hash
			MD5_CTX c;
			uint8_t digest[MD5_DIGEST_LENGTH];
			
			MD5_Init(&c);
			MD5_Update(&c, ip, strlen(ip));
			MD5_Final(&digest[0], &c);
			
			for(int i = 0; i < 3; i++) {
				hash[i*2] = "0123456789abcdef"[(digest[i] / 16) % 16];
				hash[1+i*2] = "0123456789abcdef"[digest[i] % 16];
			}
			
			// append hash to rainbow table
			fwrite(hash, 1, 6, fp);
		}
	
		fclose(fp);	
	} else if(strlen(argv[1]) == 6) {
		char *hash = malloc(7);
		hash[6] = 0;
		
		// open rainbow table for reading
		FILE *fp = fopen(database, "r");
		if(fp == NULL) {
			fprintf(stderr, "Error opening %s for reading!\n", database);
			return 1;
		}
		
		char* search_hash = argv[2];
		
		// iterate through ipv4 address space
		unsigned long i = 16843009;
		while(!feof(fp)) {
			fread(hash, 1, 6, fp);
			// test hash
			if(strncmp(hash, search_hash, 6) == 0) {
				// yep
				printf("%s\n", ultoip(i));
			}
			i++;
		}

		fclose(fp);
	} else {
		fprintf(stderr, "Error: Poster ID must be 6 characters (hexadecimal)!");
		return 1;
	}
}

