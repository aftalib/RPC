/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "tdm5.h"
#include <stdio.h>
#include <string.h>

void prompt(char *promptstr, char *buf, int maxsize) {
	printf("[%s] >> ", promptstr);
	fgets(buf, maxsize, stdin);
	buf[strcspn(buf, "\n")] = 0;
}

void listfiles(CLIENT *clnt) {
	liste nomFichiers;
	char *dirpath = malloc(MAXNOM);

	prompt("Dir", dirpath, MAXNOM);

	nomFichiers = *(ls_1(&dirpath, clnt));

	if(nomFichiers == (liste) 0xdeadbeef) {
		printf("Error while authenticating\n");
		return;
	}

	do {
		printf("%s\n", nomFichiers->name);
		nomFichiers = nomFichiers->next_cell;
	} while(nomFichiers->next_cell);
}

void readfile(CLIENT *clnt) {
	content_list contenu_fichier;
	filename nomFichier = malloc(MAXNOM);

	prompt("Filename", nomFichier, MAXNOM);
	
	contenu_fichier = *(readfile_1(&nomFichier, clnt));
	
	if(contenu_fichier == NULL) {
		printf("[-] File doesn't exist\n");
		return;
	}

	if(contenu_fichier == (content_list) 0xdeadbeef) {
		printf("Error while authenticating\n");
		return;
	}

	printf("Displaying content of file \"%s\"\n", nomFichier);

	do {
		printf("%s", contenu_fichier->content);
		contenu_fichier = contenu_fichier->next_cell;
	} while(contenu_fichier->next_cell);

	xdr_free((xdrproc_t) xdr_content_list, (char *) &contenu_fichier);
	xdr_free((xdrproc_t) xdr_filename, (char *) &nomFichier);

	printf("\n");
}

void writefile(CLIENT *clnt) {
	int res;
	params_write params;
	char *intstr = malloc(16);

	params.name = malloc(MAXNOM);
	params.data = malloc(MAXBUF);
	
	prompt("Filename", params.name, MAXNOM);
	prompt("Erase file", intstr, 16);
	prompt("File content", params.data, MAXBUF);

	params.ecraser = atoi(intstr);

	res = *(writefile_1(&params, clnt));

	if(res == -1) {
		perror("[-] writefile");
		return;
	}

	if(res == 0xdeadbeef) {
		printf("Error while authenticating\n");
		return;
	}

	free(params.name);
	params.name = NULL;

	free(params.data);
	params.data = NULL;

	xdr_free((xdrproc_t) xdr_params_write, (char *) &params);

	printf("File updated !\n");
}

int main (int argc, char *argv[]) {
	CLIENT *clnt;
	struct timeval delai;
	
	if(argc != 2) {
		printf("Usage: %s <action>\n", argv[0]);
		return 1;
	}

	clnt = clnt_create ("localhost", TDM5, VERSION, "udp");

	// Modification du timeout
	delai.tv_sec = 60;
	delai.tv_usec = 0;
	clnt_control(clnt, CLSET_TIMEOUT, (char *)&delai);
	
	// Sécurisation de l'accès RPC
	clnt->cl_auth = authunix_create_default();

	if(!strcmp(argv[1], "ls")) {
		listfiles(clnt);
	} else if(!strcmp(argv[1], "read")) {
		readfile(clnt);
	} else if(!strcmp(argv[1], "write")) {
		writefile(clnt);
	} else {
		printf("Unknown command\n");
	}

	exit (0);
}