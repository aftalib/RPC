const MAXNOM = 255;
const MAXBUF = 1024;
typedef string filename<MAXNOM>;
typedef string contentbuf<MAXBUF>;

typedef struct cellule *liste;
struct cellule {
	filename name;
	liste next_cell;
};

typedef struct content_cell *content_list;
struct content_cell {
	contentbuf content;
	content_list next_cell;
};

typedef struct params_write *params_write;
struct params_write {
	filename name;
	int ecraser;
	contentbuf data;
};

program TDM5 {
	version VERSION {
		liste LS(filename) = 1;
		content_list READFILE(filename) = 2;
		int WRITEFILE(params_write) = 3;
	} = 1;
} = 0x20000001;