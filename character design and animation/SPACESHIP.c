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




	static const uint16_t landerImage[8][12] = {
		{0x3820,0x38DB,0x3820,0x3820,
		0x38DB,0x3820,0x38DB,0x3820,
		0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x3820,0x38DC,
		0x38DC,0x38DF,0x38DB,0x3820,
		0x3820,0x38DF,0x3820,0x3820},

		{0x38DC,0x38DC,0x3820,0x3820,
		0x38DC,0x38DC,0x38DF,0x38DF,
		0x3820,0x3820,0x3820,0x3820},

		{0x3820,0x38DC,0x3820,0x3820,
		0x38DF,0x38DC,0x38DB,0x3820,
		0x3820,0x3820,0x3820,0x38DF},

		{0x3820,0x3820,0x3820,0x3820,
		0x3820,0x38DB,0x3820,0x38DB,
		0x3820,0x3820,0x38DB,0x3820},

		{0x3820,0x3820,0x38DC,0x3820,
		0x3820,0x38DB,0x38DC,0x38DF,
		0x38DF,0x3820,0x3820,0x3820},

		{0x3820,0x3820,0x38DC,0x38DC,
		0x38DF,0x38DF,0x38DC,0x38DC,
		0x3820,0x3820,0x3820,0x3820},

		{0x38DC,0x3820,0x3820,0x3820,
		0x3820,0x38DB,0x38DF,0x38DC,
		0x3820,0x3820,0x38DF,0x3820}
	};


//magenta stribe

static const uint16_t landerImage[8][12] = {
		{0x0020,0x38DB,0x0020,0x0020,
		0x2FDF,0x0020,0x2FDF,0x0020,
		0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x0020,0x38DC,
		0x38DC,0x28DF,0x2FDC,0x0020,
		0x0020,0x38DF,0x0020,0x0020},

		{0x38DC,0x28DC,0x0020,0x0020,
		0x38DC,0x28DC,0x38DF,0x38DF,
		0x0020,0x0020,0x0020,0x0020},

		{0x0020,0x38DC,0x0020,0x0020,
		0x38DF,0x28DC,0x2FDF,0x0020,
		0x0020,0x0020,0x0020,0x38DF},

		{0x0020,0x0020,0x0020,0x0020,
		0x0020,0x2FDC,0x0020,0x2FDC,
		0x0020,0x0020,0x38DB,0x0020},

		{0x0020,0x0020,0x38DC,0x0020,
		0x0020,0x327F,0x28DC,0x38DF,
		0x38DF,0x0020,0x0020,0x0020},

		{0x0020,0x0020,0x28DC,0x38DC,
		0x38DF,0x38DF,0x28DC,0x38DC,
		0x0020,0x0020,0x0020,0x0020},

		{0x38DC,0x0020,0x0020,0x0020,
		0x0020,0x2FDC,0x28DF,0x38DC,
		0x0020,0x0020,0x38DF,0x0020}
	};





static const uint16_t landerImage2[8][20] = {
		{
		0x0020,0x0020,0x38DC,0x0020,0x0020,
		0x0020,0x28DC,0x38DF,0x28DC,0x0020,
		0x0020,0x2FDC,0x0020,0x2FDC,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x0020,0x38DC,
		0x0020,0x38DC,0x28DF,0x2FDC,0x0020,
		0x28DF,0x28DC,0x38DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x0020,0x0020,
		0x28DF,0x38DF,0x28DF,0x38DC,0x38DC,
		0x28DF,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x28DC,0x0020,0x0020,0x0020,
		0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x28DF,0x38DF,0x38DC,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x28DC,0x0020,0x28DC,0x0020,
		0x0020,0x2FDC,0x0020,0x2FDC,0x0020,
		0x0020,0x0020,0x38DB,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x28DC,0x0020,
		0x0020,0x28DC,0x38DF,0x38DC,0x28DF,
		0x38DC,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x0020,0x0020,
		0x38DC,0x38DC,0x28DF,0x38DF,0x28DF,
		0x0020,0x0020,0x28DF,0x38DF,0x28DF,
		0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x38DC,0x0020,0x0020,0x0020,0x0020,
		0x0020,0x2FDC,0x28DF,0x38DC,0x0020,
		0x0020,0x0020,0x38DF,0x28DC,0x28DF,
		0x0020,0x0020,0x0020,0x0020,0x0020
		}	
};

static const uint16_t LanderImage3[8][24] = {
		{
		0x0020,0x0020,0x0020,0x38DB,0x0020,0x0020,
		0x0020,0x0020,0x2FDF,0x0020,0x2FDF,0x0020,
		0x0020,0x0020,0x2FDF,0x0020,0x2FDF,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x28DC,0x38DC,0x38DF,
		0x0020,0x28DC,0x38DF,0x38DC,0x2FDF,0x0020,
		0x38DF,0x38DC,0x28DF,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x38DF,0x28DF,0x38DF,0x28DF,0x38DC,0x38DC,
		0x38DF,0x28DF,0x38DF,0x28DF,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x38DC,0x38DF,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x0020,0x28DF,0x38DF,0x38DC
		},
		{
		0x0020,0x2FDC,0x0020,0x2FDC,0x0020,0x0020,
		0x0020,0x2FDC,0x0020,0x2FDC,0x0020,0x0020,
		0x0020,0x0020,0x38DB,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x28DC,0x38DF,0x38DC,
		0x0020,0x28DC,0x38DF,0x38DC,0x28DF,0x0020,
		0x38DC,0x38DF,0x28DF,0x0020,0x0020,0x0020,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
		0x38DC,0x38DC,0x28DF,0x38DF,0x28DF,0x38DF,
		0x0020,0x0020,0x28DF,0x38DF,0x28DF,0x38DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		},
		{
		0x38DF,0x38DC,0x28DC,0x0020,0x0020,0x0020,
		0x0020,0x28DF,0x38DC,0x38DF,0x28DC,0x0020,
		0x0020,0x0020,0x0020,0x28DF,0x38DC,0x38DF,
		0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
		}
};
