//color included

//0x bg(4 bits), fg(4 bits), char(8 bits)


//spaceship pointing up

//32, 32, 219, 32, 32, 220, 223, 32, 223,220,

//dimensions 5x2
uint16_t pointing_up [] = {0x0720,0x0720,0x07DB,0x0720,0x0720,0x07DC,0x07DF,0x0720,0x07DF,0x07DC};


//spaceship pointing right - UP

//32, 32, 32, 220, 220, 223, 219, 32, 32, 223, 32, 32,


//dimensions 4x3
uint16_t pointing_right_UP [] = {0x0720,0x0720,0x0720,0x07DC,0x07DC,0x07DF,0x07DB,0x0720,0x0720,0x07DF,0x0720,0x0720};


// spaceship pointing left - UP

//220, 32, 32, 32, 32, 219, 223, 220, 32, 32, 223, 32,

//dimensions 4x3
uint16_t pointing_left_UP [] = {0x07DC,0x0720,0x0720,0x0720,0x0720,0x07DB,0x07DF,0x07DC,0x0720,0x0720,0x07DF,0x0720};


//spaceship pointing right

//223, 220, 32, 32, 32, 32, 220, 223, 223, 223, 223, 32, 32, 32, 32,
//dimensions 5x3
uint16_t pointing_right [] = {0x07DF,0x07DC,0x0720,0x0720,0x0720,0x0720,0x07DC,0x07DF,0x07DF,0x07DF,0x07DF,0x0720,0x0720,0x0720,0x0720};



//spaceship pointing right - down

//32, 220, 32, 32, 223, 220, 219, 32, 32, 32, 32, 223,
//dimensions 4x3
uint16_t pointing_right_down [] = {0x0720,0x07DC,0x0720,0x0720,0x07DF,0x07DC,0x07DB,0x0720,0x0720,0x0720,0x0720,0x07DF};



//spaceship pointing - down

//223, 220, 32, 220, 223, 32, 32, 219, 32, 32,
//dimensions 5x2
uint16_t pointing_down [] = {0x07DF,0x07DC,0x0720,0x07DC,0x07DF,0x0720,0x0720,0x07DB,0x0720,0x0720};



//spaceship pointing left - down

//32, 32, 220, 32, 32, 219, 220, 32, 223, 32, 32, 32,
//dimensions 4x3
uint16_t pointing_left_down [] = {0x0720,0x0720,0x07DC,0x0720,0x0720,0x07DB,0x07DC,0x0720,0x07DF,0x0720,0x0720,0x0720};


//spaceship pointing left

//32, 32, 32, 220, 223, 223, 223, 220, 32, 32, 32, 32, 32, 223,
//dimensions 5x3
uint16_t pointing_left [] = {0x0720,0x0720,0x0720,0x07DC,0x07DF,0x07DF,0x07DF,0x07DC,0x0720,0x0720,0x0720,0x0720,0x0720,0x07DF};


		bg sort    fg hvid
32 = 0000 0111 00100000 binør i hexa 0x0720

219 = 0000 0111 11011011 binær, i hexa 0x07DB

220 = 0000 0111 11011100 binær, i hexa 0x07DC

223 = 0000 0111 11011111 binær, i hexa 0x07DF

