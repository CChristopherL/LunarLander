//color included

//0x bg(4 bits), fg(4 bits), char(8 bits)


//spaceship pointing up

//32, 32, 219, 32, 32, 220, 223, 32, 223,220,

//dimensions 5x2
uint16_t pointing_up [] = {0x3820,0x3820,0x38DB,0x3820,0x3820,0x38DC,0x38DF,0x3820,0x38DF,0x38DC};


//spaceship pointing right - UP

//32, 32, 32, 220, 220, 223, 219, 32, 32, 223, 32, 32,


//dimensions 4x3
uint16_t pointing_right_UP [] = {0x3820,0x3820,0x3820,0x38DC,0x38DC,0x38DF,0x38DB,0x3820,0x3820,0x38DF,0x3820,0x3820};


// spaceship pointing left - UP

//220, 32, 32, 32, 32, 219, 223, 220, 32, 32, 223, 32,

//dimensions 4x3
uint16_t pointing_left_UP [] = {0x38DC,0x3820,0x3820,0x3820,0x3820,0x38DB,0x38DF,0x38DC,0x3820,0x3820,0x38DF,0x3820};


//spaceship pointing right

//223, 220, 32, 32, 32, 32, 220, 223, 223, 223, 223, 32, 32, 32, 32,
//dimensions 5x3
uint16_t pointing_right [] = {0x38DF,0x38DC,0x3820,0x3820,0x3820,0x3820,0x38DC,0x38DF,0x38DF,0x38DF,0x38DF,0x3820,0x3820,0x3820,0x3820};



//spaceship pointing right - down

//32, 220, 32, 32, 223, 220, 219, 32, 32, 32, 32, 223,
//dimensions 4x3
uint16_t pointing_right_down [] = {0x3820,0x38DC,0x3820,0x3820,0x38DF,0x38DC,0x38DB,0x3820,0x3820,0x3820,0x3820,0x38DF};



//spaceship pointing - down

//223, 220, 32, 220, 223, 32, 32, 219, 32, 32,
//dimensions 5x2
uint16_t pointing_down [] = {0x38DF,0x38DC,0x3820,0x38DC,0x38DF,0x3820,0x3820,0x38DB,0x3820,0x3820};



//spaceship pointing left - down

//32, 32, 220, 32, 32, 219, 220, 32, 223, 32, 32, 32,
//dimensions 4x3
uint16_t pointing_left_down [] = {0x3820,0x3820,0x38DC,0x3820,0x3820,0x38DB,0x38DC,0x3820,0x38DF,0x3820,0x3820,0x3820};


//spaceship pointing left

//32, 32, 32, 220, 223, 223, 223, 220, 32, 32, 32, 32, 32, 223,
//dimensions 5x3
uint16_t pointing_left [] = {0x3820,0x3820,0x3820,0x38DC,0x38DF,0x38DF,0x38DF,0x38DC,0x3820,0x3820,0x3820,0x3820,0x3820,0x38DF};


		bg sort    fg hvid
32 = 0000 0111 00100000 binør i hexa 0x3820

219 = 0000 0111 11011011 binær, i hexa 0x38DB

220 = 0000 0111 11011100 binær, i hexa 0x38DC

223 = 0000 0111 11011111 binær, i hexa 0x38DF

