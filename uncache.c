/* This program removes specified (or current) directory/file from Linux filesystem cache */
#define _GNU_SOURCE
#define _XOPEN_SOURCE 600

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fts.h>
#include <fcntl.h>

int main(int ac, char ** av) {
	FTS * tree;
	FTSENT * node;
	
	char **paths = malloc(ac*sizeof(char*));
	memcpy(paths,av+1,(ac-1)*sizeof(char*));
	paths[ac]=NULL;
	if (ac<2) paths[0]=getcwd(NULL,0);

	tree=fts_open(paths,FTS_LOGICAL|FTS_NOCHDIR,NULL);
	while((node=fts_read(tree))) {
		int fd;
		if (node->fts_info != FTS_F) continue;
		fd=open(node->fts_path,O_DIRECT);
		posix_fadvise(fd,0,0,POSIX_FADV_DONTNEED);
		close(fd);
	}
	fts_close(tree);
	if (ac<2) free(paths[0]);
	free(paths);
	return(0);
}

