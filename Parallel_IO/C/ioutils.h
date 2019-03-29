void initarray(void *ptr, int nx, int ny);
void initpgrid(void *ptr, int nxproc, int nyproc);

void createfilename(char *filename, char *basename, int nx, int ny, int rank);

void ioread (char *filename, void *ptr, int ndouble);
void iowrite(char *filename, void *ptr, int ndouble);
