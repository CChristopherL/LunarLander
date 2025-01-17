//color included
//0x bg(4 bits), fg(4 bits), char(8 bits)
//white on black

uint16_t meteor1[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07DB,
		0x07DB,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07DB,
		0x07DB,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B1,0x07B1,0x07B1,0x07B1,0x07B1,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};


uint16_t meteor2[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B1,0x07DB,
		0x07DB,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07DB,
		0x07DB,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor3[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B1,0x07B0,0x07B0,0x07B1,0x07B1,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor4[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B0,0x07B1,0x07B1,0x07B1,0x07B0,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor5[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07B1,0x07B1,0x07B1,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor6[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor7[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B1,0x07B1,0x07B0,0x07B0,0x07B1,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B1,0x07B1,0x07B0,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

uint16_t meteor8[] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DC,0x07DC,0x07DC,0x0720,0x0720,0x0720,
		0x0720,0x07DC,0x07DB,0x07B0,0x07B1,0x07B1,0x07B1,0x07B0,0x07DB,0x07DC,0x0720,
		0x07DB,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07B1,0x07B0,0x07B0,0x07B0,0x07DB,
		0x07DB,0x07B0,0x07B1,0x07B1,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07DB,
		0x07DB,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B0,0x07B1,0x07B1,0x07DB,
		0x0720,0x07DF,0x07DB,0x07B1,0x07B1,0x07B1,0x07B0,0x07B0,0x07DB,0x07DF,0x0720,
		0x0720,0x0720,0x0720,0x07DF,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720};

//char string
char meteor1[] = "   ▄▄▄▄▄    ▄█▓▓▓░░█▄ █▓▓░░▓▓░░▓██▓▓▓▓▓▓▓▓▓██▓░░░▓▓░▓▓█▀█▓░▓▓▓█▀    ▀▀▀▀▀▀▀   ";

char meteor2[] = "   ▄▄▄▄▄    ▄█▓▓▓░░█▄ █▓▓▓░░▓▓▓░██░░▓▓▓▓▓░░██▓░▓▓▓▓▓░▓█▀█▓▓░▓▓█▀    ▀▀▀▀▀▀▀   ";

//color NOT included
//black on white
char meteor1[77] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,178,178,178,176,176,219,220,32,
		219,178,178,176,176,178,178,176,176,178,219,
		219,178,178,178,178,178,178,178,178,178,219,
		219,178,176,176,176,178,178,176,178,178,219,
		32,223,219,178,176,178,178,178,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};


//white on black
char meteor1[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,176,176,178,178,219,220,32,
		219,176,176,178,178,176,176,178,178,176,219,
		219,176,176,176,176,176,176,176,176,176,219,
		219,176,178,178,178,176,176,178,176,176,219,
		32,223,219,178,176,176,176,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};


char meteor2[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,176,176,176,178,219,220,32,
		219,176,176,176,178,178,176,176,176,178,219,
		219,178,178,176,176,176,176,176,178,178,219,
		219,176,178,176,176,176,176,176,178,176,219,
		32,223,219,176,176,178,176,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor3[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,178,176,176,176,176,219,220,32,
		219,176,178,178,176,176,178,178,176,176,219,
		219,176,176,178,176,176,176,176,176,176,219,
		219,176,176,176,176,176,176,178,178,176,219,
		32,223,219,178,176,176,178,178,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor4[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,178,178,178,176,219,220,32,
		219,176,176,176,176,176,176,176,176,176,219,
		219,176,178,176,176,176,176,178,178,176,219,
		219,176,176,176,178,176,176,176,176,176,219,
		32,223,219,176,178,178,176,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor5[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,176,176,176,178,219,220,32,
		219,176,176,178,176,176,178,178,178,176,219,
		219,176,176,176,176,176,176,176,176,176,219,
		219,176,178,178,176,176,178,178,176,176,219,
		32,223,219,178,178,176,176,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor6[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,176,176,178,176,219,220,32,
		219,176,176,178,178,176,176,176,178,176,219,
		219,176,178,178,176,176,176,176,178,178,219,
		219,176,176,176,176,176,176,176,176,178,219,
		32,223,219,176,176,178,178,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor7[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,178,178,176,176,178,219,220,32,
		219,176,178,178,176,176,176,176,176,176,219,
		219,176,176,176,176,176,176,178,176,176,219,
		219,176,176,178,178,176,176,178,178,176,219,
		32,223,219,176,176,176,176,178,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};

char meteor8[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,176,178,178,178,176,219,220,32,
		219,176,178,176,176,176,178,176,176,176,219,
		219,176,178,178,176,176,176,176,178,178,219,
		219,176,176,176,176,176,176,176,178,178,219,
		32,223,219,178,178,178,176,176,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};





char meteor1[] = {32,32,32,220,220,220,220,220,32,32,32,
		32,220,219,177,177,177,178,178,219,220,32,
		219,177,177,178,178,177,177,178,178,177,219,
		219,177,177,177,177,177,177,177,177,177,219,
		219,177,178,178,178,177,177,178,177,177,219,
		32,223,219,178,177,177,177,177,219,223,32,
		32,32,32,223,223,223,223,223,32,32,32};


