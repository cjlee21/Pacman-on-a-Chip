#define __MAIN_C__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Define the ASCI type codes for non-ASCII symbols
#define CHERRY												0x50
#define NULL_CHAR											0x00

// Define Joystick directions
#define PAC_RIGHT											0b0001
#define PAC_LEFT											0b0010
#define PAC_UP												0b0100
#define PAC_DOWN											0b1000

// Define Ghost Directions
#define GHOST_RIGHT										0
#define GHOST_LEFT										1
#define GHOST_UP											2
#define GHOST_DOWN										3

// Define Movement Distance per tick
#define STEP_DIST											8

// Define the raw base address values for the i/o devices

//ROM_BASE 														0x00000000
#define AHB_RAM_BASE 									0x20000000
#define AHB_PacRAM_BASE 							0x30000000
#define AHB_GhostRAM_BASE 						0x40000000
#define AHB_JOYSTICK_BASE             0x50000000 // replace button module with joystick
#define AHB_CHARROM_BASE							0x60000000
#define AHB_VIDMEM_BASE								0x70000000
#define AHB_TIMER_BASE								0x80000000

// Define pointers with correct type for access to 32-bit i/o devices

volatile uint32_t* PACMAN_RAM = (volatile uint32_t*) AHB_PacRAM_BASE;
volatile uint32_t* GHOST_RAM = (volatile uint32_t*) AHB_GhostRAM_BASE;
volatile uint32_t* GENERAL_RAM = (volatile uint32_t*) AHB_RAM_BASE;
volatile uint32_t* CHAR_MEM = (volatile uint32_t*) AHB_CHARROM_BASE;
volatile uint32_t* VID_MEM = (volatile uint32_t*) AHB_VIDMEM_BASE;
volatile uint32_t* TIMER_REG = (volatile uint32_t*) AHB_TIMER_BASE;
volatile uint32_t* JOYSTICK_REGS = (volatile uint32_t*) AHB_JOYSTICK_BASE;

#define x_mask												0x3FFu //32 bit word with last 10 bits being 1

// Pacman Start
const uint16_t starting_x1 = 304;
const uint16_t starting_y1 = 272;

// Ghost Start
const uint16_t starting_x2 = 304;
const uint16_t starting_y2 = 192;

// Game Tracking
uint16_t pacman_x, pacman_y;
uint16_t ghost_x, ghost_y;
int score;
int ghost_dir;

//////////////////////////////////////////////////////////////////
// Functions provided to access i/o devices
//////////////////////////////////////////////////////////////////

uint32_t read_joystick(void) {
	
	return JOYSTICK_REGS[0];
	
};

uint32_t read_timer_count(void) {
	
	return TIMER_REG[1];
	
};

uint32_t read_timer_status(void) {
	
	return TIMER_REG[0];
	
};

void write_timer_count(uint32_t value) {
	
	TIMER_REG[1] = value;
	
};

uint32_t read_vidmem(uint32_t addr) {
	
	return VID_MEM[addr];
	
};


/*  int is_corridor(uint32_t xloc, uint32_t yloc) {
	
	uint32_t tile, index;
	int corridor = 1;
	for ( int a = -1; a < 2; a++ ) {
		for ( int b = -1; b < 2; b++ ) {
			index = ((yloc + a) / 16) * 40 + ((xloc + b) / 16);
			tile = read_vidmem(index);
			if ( !((tile == ' ' ) || (tile == NULL_CHAR ) || (tile == CHERRY)) ) {
				corridor = 0;
			};
		};
	};
	
	return corridor;
	
}; */ 

 
uint32_t is_corridor(uint32_t xloc, uint32_t yloc) {
	
	uint32_t tile, index;
	index = (yloc / 16) * 40 + ((xloc) / 16);
  tile = read_vidmem(index);
	return (tile == ' ' ) || (tile == NULL_CHAR ) || (tile == CHERRY);
	
};

uint32_t is_cherry(uint32_t xloc, uint32_t yloc) {
	
	uint32_t index, tile;
	index = ((pacman_y + 7) / 16) * 40 + ((pacman_x + 7) / 16);
	tile = read_vidmem(index);
	return tile;
	
};

void write_vidmem(uint32_t index, uint32_t value) {
	
	VID_MEM[index] = value;

};

void write_PacRAM(uint32_t index, uint32_t value) {
	
	PACMAN_RAM[index] = value;

};


void write_GhostRAM(uint32_t index, uint32_t value) {
	
	GHOST_RAM[index] = value;

};

void init_ghost_ram(void) {
	
	//row 1
	write_GhostRAM(1, 0x00000000);
	write_GhostRAM(2, 0x00000000);
	write_GhostRAM(3, 0x00000000);
	write_GhostRAM(4, 0x00000000);
	write_GhostRAM(5, 0x00000000);
	write_GhostRAM(6, 0x00000000);
	write_GhostRAM(7, 0x00000000);
	write_GhostRAM(8, 0x00000000);
	write_GhostRAM(9, 0x00000000);
	write_GhostRAM(10, 0x00000000);
	write_GhostRAM(11, 0x00000000);
	write_GhostRAM(12, 0x00000000);
	write_GhostRAM(13, 0x00000000);
	write_GhostRAM(14, 0x00000000);
	write_GhostRAM(15, 0x00000000);
	write_GhostRAM(16, 0x00000000);
	
	//row 2
	write_GhostRAM(17, 0x00000000);
	write_GhostRAM(18, 0x00000000);
	write_GhostRAM(19, 0x00000000);
	write_GhostRAM(20, 0x00000000);
	write_GhostRAM(21, 0x00000000);
	write_GhostRAM(22, 0x00000000);
	write_GhostRAM(23, 0x00000004);
	write_GhostRAM(24, 0x00000004);
	write_GhostRAM(25, 0x00000004);
	write_GhostRAM(26, 0x00000004);
	write_GhostRAM(27, 0x00000000);
	write_GhostRAM(28, 0x00000000);
	write_GhostRAM(29, 0x00000000);
	write_GhostRAM(30, 0x00000000);
	write_GhostRAM(31, 0x00000000);
	write_GhostRAM(32, 0x00000000);
	
	//row 3
	write_GhostRAM(33, 0x00000000);
	write_GhostRAM(34, 0x00000000);
	write_GhostRAM(35, 0x00000000);
	write_GhostRAM(36, 0x00000000);
	write_GhostRAM(37, 0x00000004);
	write_GhostRAM(38, 0x00000004);
	write_GhostRAM(39, 0x00000004);
	write_GhostRAM(40, 0x00000004);
	write_GhostRAM(41, 0x00000004);
	write_GhostRAM(42, 0x00000004);
	write_GhostRAM(43, 0x00000004);
	write_GhostRAM(44, 0x00000004);
	write_GhostRAM(45, 0x00000000);
	write_GhostRAM(46, 0x00000000);
	write_GhostRAM(47, 0x00000000);
	write_GhostRAM(48, 0x00000000);
	
	//row 4
	write_GhostRAM(49, 0x00000000);
	write_GhostRAM(50, 0x00000000);
	write_GhostRAM(51, 0x00000000);
	write_GhostRAM(52, 0x00000004);
	write_GhostRAM(53, 0x00000004);
	write_GhostRAM(54, 0x00000004);
	write_GhostRAM(55, 0x00000004);
	write_GhostRAM(56, 0x00000004);
	write_GhostRAM(57, 0x00000004);
	write_GhostRAM(58, 0x00000004);
	write_GhostRAM(59, 0x00000004);
	write_GhostRAM(60, 0x00000004);
	write_GhostRAM(61, 0x00000004);
	write_GhostRAM(62, 0x00000000);
	write_GhostRAM(63, 0x00000000);
	write_GhostRAM(64, 0x00000000);
	
	//row 5
	write_GhostRAM(65, 0x00000000);
	write_GhostRAM(66, 0x00000000);
	write_GhostRAM(67, 0x00000004);
	write_GhostRAM(68, 0x00000007);
	write_GhostRAM(69, 0x00000007);
	write_GhostRAM(70, 0x00000004);
	write_GhostRAM(71, 0x00000004);
	write_GhostRAM(72, 0x00000004);
	write_GhostRAM(73, 0x00000004);
	write_GhostRAM(74, 0x00000007);
	write_GhostRAM(75, 0x00000007);
	write_GhostRAM(76, 0x00000004);
	write_GhostRAM(77, 0x00000004);
	write_GhostRAM(78, 0x00000004);
	write_GhostRAM(79, 0x00000000);
	write_GhostRAM(80, 0x00000000);
	
	//row 6
	write_GhostRAM(81, 0x00000000);
	write_GhostRAM(82, 0x00000000);
	write_GhostRAM(83, 0x00000007);
	write_GhostRAM(84, 0x00000007);
	write_GhostRAM(85, 0x00000007);
	write_GhostRAM(86, 0x00000007);
	write_GhostRAM(87, 0x00000004);
	write_GhostRAM(88, 0x00000004);
	write_GhostRAM(89, 0x00000007);
	write_GhostRAM(90, 0x00000007);
	write_GhostRAM(91, 0x00000007);
	write_GhostRAM(92, 0x00000007);
	write_GhostRAM(93, 0x00000004);
	write_GhostRAM(94, 0x00000004);
	write_GhostRAM(95, 0x00000000);
	write_GhostRAM(96, 0x00000000);
	
	//row 7
	write_GhostRAM(97, 0x00000000);
	write_GhostRAM(98, 0x00000000);
	write_GhostRAM(99, 0x00000001);
	write_GhostRAM(100, 0x00000001);
	write_GhostRAM(101, 0x00000007);
	write_GhostRAM(102, 0x00000007);
	write_GhostRAM(103, 0x00000004);
	write_GhostRAM(104, 0x00000004);
	write_GhostRAM(105, 0x00000001);
	write_GhostRAM(106, 0x00000001);
	write_GhostRAM(107, 0x00000007);
	write_GhostRAM(108, 0x00000007);
	write_GhostRAM(109, 0x00000004);
	write_GhostRAM(110, 0x00000004);
	write_GhostRAM(111, 0x00000000);
	write_GhostRAM(112, 0x00000000);
	
	//row 8
	write_GhostRAM(113, 0x00000000);
	write_GhostRAM(114, 0x00000004);
	write_GhostRAM(115, 0x00000001);
	write_GhostRAM(116, 0x00000001);
	write_GhostRAM(117, 0x00000007);
	write_GhostRAM(118, 0x00000007);
	write_GhostRAM(119, 0x00000004);
	write_GhostRAM(120, 0x00000004);
	write_GhostRAM(121, 0x00000001);
	write_GhostRAM(122, 0x00000001);
	write_GhostRAM(123, 0x00000007);
	write_GhostRAM(124, 0x00000007);
	write_GhostRAM(125, 0x00000004);
	write_GhostRAM(126, 0x00000004);
	write_GhostRAM(127, 0x00000004);
	write_GhostRAM(128, 0x00000000);
	
	//row 9
	write_GhostRAM(129, 0x00000000);
	write_GhostRAM(130, 0x00000004);
	write_GhostRAM(131, 0x00000004);
	write_GhostRAM(132, 0x00000007);
	write_GhostRAM(133, 0x00000007);
	write_GhostRAM(134, 0x00000004);
	write_GhostRAM(135, 0x00000004);
	write_GhostRAM(136, 0x00000004);
	write_GhostRAM(137, 0x00000004);
	write_GhostRAM(138, 0x00000007);
	write_GhostRAM(139, 0x00000007);
	write_GhostRAM(140, 0x00000004);
	write_GhostRAM(141, 0x00000004);
	write_GhostRAM(142, 0x00000004);
	write_GhostRAM(143, 0x00000004);
	write_GhostRAM(144, 0x00000000);
	
	//row 10
	write_GhostRAM(145, 0x00000000);
	write_GhostRAM(146, 0x00000004);
	write_GhostRAM(147, 0x00000004);
	write_GhostRAM(148, 0x00000004);
	write_GhostRAM(149, 0x00000004);
	write_GhostRAM(150, 0x00000004);
	write_GhostRAM(151, 0x00000004);
	write_GhostRAM(152, 0x00000004);
	write_GhostRAM(153, 0x00000004);
	write_GhostRAM(154, 0x00000004);
	write_GhostRAM(155, 0x00000004);
	write_GhostRAM(156, 0x00000004);
	write_GhostRAM(157, 0x00000004);
	write_GhostRAM(158, 0x00000004);
	write_GhostRAM(159, 0x00000004);
	write_GhostRAM(160, 0x00000000);
	
	//row 11
	write_GhostRAM(161, 0x00000000);
	write_GhostRAM(162, 0x00000004);
	write_GhostRAM(163, 0x00000004);
	write_GhostRAM(164, 0x00000004);
	write_GhostRAM(165, 0x00000004);
	write_GhostRAM(166, 0x00000004);
	write_GhostRAM(167, 0x00000004);
	write_GhostRAM(168, 0x00000004);
	write_GhostRAM(169, 0x00000004);
	write_GhostRAM(170, 0x00000004);
	write_GhostRAM(171, 0x00000004);
	write_GhostRAM(172, 0x00000004);
	write_GhostRAM(173, 0x00000004);
	write_GhostRAM(174, 0x00000004);
	write_GhostRAM(175, 0x00000004);
	write_GhostRAM(176, 0x00000000);
	
	//row 12
	write_GhostRAM(177, 0x00000000);
	write_GhostRAM(178, 0x00000004);
	write_GhostRAM(179, 0x00000004);
	write_GhostRAM(180, 0x00000004);
	write_GhostRAM(181, 0x00000004);
	write_GhostRAM(182, 0x00000004);
	write_GhostRAM(183, 0x00000004);
	write_GhostRAM(184, 0x00000004);
	write_GhostRAM(185, 0x00000004);
	write_GhostRAM(186, 0x00000004);
	write_GhostRAM(187, 0x00000004);
	write_GhostRAM(188, 0x00000004);
	write_GhostRAM(189, 0x00000004);
	write_GhostRAM(190, 0x00000004);
	write_GhostRAM(191, 0x00000004);
	write_GhostRAM(192, 0x00000000);
	
	//row 13
	write_GhostRAM(193, 0x00000000);
	write_GhostRAM(194, 0x00000004);
	write_GhostRAM(195, 0x00000004);
	write_GhostRAM(196, 0x00000004);
	write_GhostRAM(197, 0x00000004);
	write_GhostRAM(198, 0x00000004);
	write_GhostRAM(199, 0x00000004);
	write_GhostRAM(200, 0x00000004);
	write_GhostRAM(201, 0x00000004);
	write_GhostRAM(202, 0x00000004);
	write_GhostRAM(203, 0x00000004);
	write_GhostRAM(204, 0x00000004);
	write_GhostRAM(205, 0x00000004);
	write_GhostRAM(206, 0x00000004);
	write_GhostRAM(207, 0x00000004);
	write_GhostRAM(208, 0x00000000);
	
	//row 14
	write_GhostRAM(209, 0x00000000);
	write_GhostRAM(210, 0x00000004);
	write_GhostRAM(211, 0x00000004);
	write_GhostRAM(212, 0x00000004);
	write_GhostRAM(213, 0x00000004);
	write_GhostRAM(214, 0x00000004);
	write_GhostRAM(215, 0x00000004);
	write_GhostRAM(216, 0x00000004);
	write_GhostRAM(217, 0x00000004);
	write_GhostRAM(218, 0x00000004);
	write_GhostRAM(219, 0x00000004);
	write_GhostRAM(220, 0x00000004);
	write_GhostRAM(221, 0x00000004);
	write_GhostRAM(222, 0x00000004);
	write_GhostRAM(223, 0x00000004);
	write_GhostRAM(224, 0x00000000);
	
	//row 15
	write_GhostRAM(225, 0x00000000);
	write_GhostRAM(226, 0x00000004);
	write_GhostRAM(227, 0x00000000);
	write_GhostRAM(228, 0x00000000);
	write_GhostRAM(229, 0x00000000);
	write_GhostRAM(230, 0x00000004);
	write_GhostRAM(231, 0x00000004);
	write_GhostRAM(232, 0x00000000);
	write_GhostRAM(233, 0x00000000);
	write_GhostRAM(234, 0x00000004);
	write_GhostRAM(235, 0x00000004);
	write_GhostRAM(236, 0x00000000);
	write_GhostRAM(237, 0x00000000);
	write_GhostRAM(238, 0x00000000);
	write_GhostRAM(239, 0x00000004);
	write_GhostRAM(240, 0x00000000);
	
	//row 16
	write_GhostRAM(241, 0x00000000);
	write_GhostRAM(242, 0x00000000);
	write_GhostRAM(243, 0x00000000);
	write_GhostRAM(244, 0x00000000);
	write_GhostRAM(245, 0x00000000);
	write_GhostRAM(246, 0x00000000);
	write_GhostRAM(247, 0x00000000);
	write_GhostRAM(248, 0x00000000);
	write_GhostRAM(249, 0x00000000);
	write_GhostRAM(250, 0x00000000);
	write_GhostRAM(251, 0x00000000);
	write_GhostRAM(252, 0x00000000);
	write_GhostRAM(253, 0x00000000);
	write_GhostRAM(254, 0x00000000);
	write_GhostRAM(255, 0x00000000);
	write_GhostRAM(256, 0x00000000);
	
};


void init_pacman_ram(void) {
	
	//row 1
	write_PacRAM(1, 0x00000000);
	write_PacRAM(2, 0x00000000);
	write_PacRAM(3, 0x00000000);
	write_PacRAM(4, 0x00000000);
	write_PacRAM(5, 0x00000000);
	write_PacRAM(6, 0x00000000);
	write_PacRAM(7, 0x00000000);
	write_PacRAM(8, 0x00000000);
	write_PacRAM(9, 0x00000000);
	write_PacRAM(10, 0x00000000);
	write_PacRAM(11, 0x00000000);
	write_PacRAM(12, 0x00000000);
	write_PacRAM(13, 0x00000000);
	write_PacRAM(14, 0x00000000);
	write_PacRAM(15, 0x00000000);
	write_PacRAM(16, 0x00000000);
	
	//row 2
	write_PacRAM(17, 0x00000000);
	write_PacRAM(18, 0x00000000);
	write_PacRAM(19, 0x00000000);
	write_PacRAM(20, 0x00000000);
	write_PacRAM(21, 0x00000000);
	write_PacRAM(21, 0x00000006);
	write_PacRAM(22, 0x00000006);
	write_PacRAM(23, 0x00000006);
	write_PacRAM(24, 0x00000006);
	write_PacRAM(25, 0x00000006);
	write_PacRAM(26, 0x00000006);
	write_PacRAM(27, 0x00000006);
	write_PacRAM(28, 0x00000006);
	write_PacRAM(29, 0x00000000);
	write_PacRAM(30, 0x00000000);
	write_PacRAM(31, 0x00000000);
	write_PacRAM(32, 0x00000000);
	
	//row 3
	write_PacRAM(33, 0x00000000);
	write_PacRAM(34, 0x00000000);
	write_PacRAM(35, 0x00000000);
	write_PacRAM(36, 0x00000006);
	write_PacRAM(37, 0x00000006);
	write_PacRAM(38, 0x00000006);
	write_PacRAM(39, 0x00000006);
	write_PacRAM(40, 0x00000006);
	write_PacRAM(41, 0x00000006);
	write_PacRAM(42, 0x00000006);
	write_PacRAM(43, 0x00000006);
	write_PacRAM(44, 0x00000006);
	write_PacRAM(45, 0x00000006);
	write_PacRAM(46, 0x00000006);
	write_PacRAM(47, 0x00000000);
	write_PacRAM(48, 0x00000000);
	
	//row 4
	write_PacRAM(49, 0x00000000);
	write_PacRAM(50, 0x00000000);
	write_PacRAM(51, 0x00000006);
	write_PacRAM(52, 0x00000006);
	write_PacRAM(53, 0x00000006);
	write_PacRAM(54, 0x00000006);
	write_PacRAM(55, 0x00000006);
	write_PacRAM(56, 0x00000006);
	write_PacRAM(57, 0x00000006);
	write_PacRAM(58, 0x00000006);
	write_PacRAM(59, 0x00000006);
	write_PacRAM(60, 0x00000006);
	write_PacRAM(61, 0x00000006);
	write_PacRAM(62, 0x00000006);
	write_PacRAM(63, 0x00000006);
	write_PacRAM(64, 0x00000000);
	
	//row 5
	write_PacRAM(65, 0x00000000);
	write_PacRAM(66, 0x00000000);
	write_PacRAM(67, 0x00000006);
	write_PacRAM(68, 0x00000006);
	write_PacRAM(69, 0x00000006);
	write_PacRAM(70, 0x00000006);
	write_PacRAM(71, 0x00000006);
	write_PacRAM(72, 0x00000006);
	write_PacRAM(73, 0x00000006);
	write_PacRAM(74, 0x00000006);
	write_PacRAM(75, 0x00000006);
	write_PacRAM(76, 0x00000006);
	write_PacRAM(77, 0x00000006);
	write_PacRAM(78, 0x00000006);
	write_PacRAM(79, 0x00000006);
	write_PacRAM(80, 0x00000000);
	
	//row 6
	write_PacRAM(81, 0x00000000);
	write_PacRAM(82, 0x00000006);
	write_PacRAM(83, 0x00000006);
	write_PacRAM(84, 0x00000006);
	write_PacRAM(85, 0x00000006);
	write_PacRAM(86, 0x00000006);
	write_PacRAM(87, 0x00000006);
	write_PacRAM(88, 0x00000006);
	write_PacRAM(89, 0x00000006);
	write_PacRAM(90, 0x00000006);
	write_PacRAM(91, 0x00000006);
	write_PacRAM(92, 0x00000006);
	write_PacRAM(93, 0x00000006);
	write_PacRAM(94, 0x00000000);
	write_PacRAM(95, 0x00000000);
	write_PacRAM(96, 0x00000000);
	
	//row 7
	write_PacRAM(97, 0x00000000);
	write_PacRAM(98, 0x00000006);
	write_PacRAM(99, 0x00000006);
	write_PacRAM(100, 0x00000006);
	write_PacRAM(101, 0x00000006);
	write_PacRAM(102, 0x00000006);
	write_PacRAM(103, 0x00000006);
	write_PacRAM(104, 0x00000006);
	write_PacRAM(105, 0x00000006);
	write_PacRAM(106, 0x00000006);
	write_PacRAM(107, 0x00000000);
	write_PacRAM(108, 0x00000000);
	write_PacRAM(109, 0x00000000);
	write_PacRAM(110, 0x00000000);
	write_PacRAM(111, 0x00000000);
	write_PacRAM(112, 0x00000000);
	
	//row 8
	write_PacRAM(113, 0x00000000);
	write_PacRAM(114, 0x00000006);
	write_PacRAM(115, 0x00000006);
	write_PacRAM(116, 0x00000006);
	write_PacRAM(117, 0x00000006);
	write_PacRAM(118, 0x00000006);
	write_PacRAM(119, 0x00000006);
	write_PacRAM(120, 0x00000000);
	write_PacRAM(121, 0x00000000);
	write_PacRAM(122, 0x00000000);
	write_PacRAM(123, 0x00000000);
	write_PacRAM(124, 0x00000000);
	write_PacRAM(125, 0x00000000);
	write_PacRAM(126, 0x00000000);
	write_PacRAM(127, 0x00000000);
	write_PacRAM(128, 0x00000000);
	
	//row 9
	write_PacRAM(129, 0x00000000);
	write_PacRAM(130, 0x00000006);
	write_PacRAM(131, 0x00000006);
	write_PacRAM(132, 0x00000006);
	write_PacRAM(133, 0x00000006);
	write_PacRAM(134, 0x00000006);
	write_PacRAM(135, 0x00000006);
	write_PacRAM(136, 0x00000006);
	write_PacRAM(137, 0x00000006);
	write_PacRAM(138, 0x00000006);
	write_PacRAM(139, 0x00000000);
	write_PacRAM(140, 0x00000000);
	write_PacRAM(141, 0x00000000);
	write_PacRAM(142, 0x00000000);
	write_PacRAM(143, 0x00000000);
	write_PacRAM(144, 0x00000000);
	
	//row 10
	write_PacRAM(145, 0x00000000);
	write_PacRAM(146, 0x00000006);
	write_PacRAM(147, 0x00000006);
	write_PacRAM(148, 0x00000006);
	write_PacRAM(149, 0x00000006);
	write_PacRAM(150, 0x00000006);
	write_PacRAM(151, 0x00000006);
	write_PacRAM(152, 0x00000006);
	write_PacRAM(153, 0x00000006);
	write_PacRAM(154, 0x00000006);
	write_PacRAM(155, 0x00000006);
	write_PacRAM(156, 0x00000006);
	write_PacRAM(157, 0x00000006);
	write_PacRAM(158, 0x00000000);
	write_PacRAM(159, 0x00000000);
	write_PacRAM(160, 0x00000000);
	
	//row 11
	write_PacRAM(161, 0x00000000);
	write_PacRAM(162, 0x00000006);
	write_PacRAM(163, 0x00000006);
	write_PacRAM(164, 0x00000006);
	write_PacRAM(165, 0x00000006);
	write_PacRAM(166, 0x00000006);
	write_PacRAM(167, 0x00000006);
	write_PacRAM(168, 0x00000006);
	write_PacRAM(169, 0x00000006);
	write_PacRAM(170, 0x00000006);
	write_PacRAM(171, 0x00000006);
	write_PacRAM(172, 0x00000006);
	write_PacRAM(173, 0x00000006);
	write_PacRAM(174, 0x00000006);
	write_PacRAM(175, 0x00000006);
	write_PacRAM(176, 0x00000000);
	
	//row 12
	write_PacRAM(177, 0x00000000);
	write_PacRAM(178, 0x00000000);
	write_PacRAM(179, 0x00000006);
	write_PacRAM(180, 0x00000006);
	write_PacRAM(181, 0x00000006);
	write_PacRAM(182, 0x00000006);
	write_PacRAM(183, 0x00000006);
	write_PacRAM(184, 0x00000006);
	write_PacRAM(185, 0x00000006);
	write_PacRAM(186, 0x00000006);
	write_PacRAM(187, 0x00000006);
	write_PacRAM(188, 0x00000006);
	write_PacRAM(189, 0x00000006);
	write_PacRAM(190, 0x00000006);
	write_PacRAM(191, 0x00000006);
	write_PacRAM(192, 0x00000000);
	
	//row 13
	write_PacRAM(193, 0x00000000);
	write_PacRAM(194, 0x00000000);
	write_PacRAM(195, 0x00000006);
	write_PacRAM(196, 0x00000006);
	write_PacRAM(197, 0x00000006);
	write_PacRAM(198, 0x00000006);
	write_PacRAM(199, 0x00000006);
	write_PacRAM(200, 0x00000006);
	write_PacRAM(201, 0x00000006);
	write_PacRAM(202, 0x00000006);
	write_PacRAM(203, 0x00000006);
	write_PacRAM(204, 0x00000006);
	write_PacRAM(205, 0x00000006);
	write_PacRAM(206, 0x00000006);
	write_PacRAM(207, 0x00000000);
	write_PacRAM(208, 0x00000000);
	
	//row 14
	write_PacRAM(209, 0x00000000);
	write_PacRAM(210, 0x00000000);
	write_PacRAM(211, 0x00000000);
	write_PacRAM(212, 0x00000000);
	write_PacRAM(213, 0x00000006);
	write_PacRAM(214, 0x00000006);
	write_PacRAM(215, 0x00000006);
	write_PacRAM(216, 0x00000006);
	write_PacRAM(217, 0x00000006);
	write_PacRAM(218, 0x00000006);
	write_PacRAM(219, 0x00000006);
	write_PacRAM(220, 0x00000006);
	write_PacRAM(221, 0x00000006);
	write_PacRAM(222, 0x00000006);
	write_PacRAM(223, 0x00000000);
	write_PacRAM(224, 0x00000000);
	
	//row 15
	write_PacRAM(225, 0x00000000);
	write_PacRAM(226, 0x00000000);
	write_PacRAM(227, 0x00000000);
	write_PacRAM(228, 0x00000000);
	write_PacRAM(229, 0x00000000);
	write_PacRAM(230, 0x00000006);
	write_PacRAM(231, 0x00000006);
	write_PacRAM(232, 0x00000006);
	write_PacRAM(233, 0x00000006);
	write_PacRAM(234, 0x00000006);
	write_PacRAM(235, 0x00000006);
	write_PacRAM(236, 0x00000006);
	write_PacRAM(237, 0x00000000);
	write_PacRAM(238, 0x00000000);
	write_PacRAM(239, 0x00000000);
	write_PacRAM(240, 0x00000000);
	
	//row 16
	write_PacRAM(241, 0x00000000);
	write_PacRAM(242, 0x00000000);
	write_PacRAM(243, 0x00000000);
	write_PacRAM(244, 0x00000000);
	write_PacRAM(245, 0x00000000);
	write_PacRAM(246, 0x00000000);
	write_PacRAM(247, 0x00000000);
	write_PacRAM(248, 0x00000000);
	write_PacRAM(249, 0x00000000);
	write_PacRAM(250, 0x00000000);
	write_PacRAM(251, 0x00000000);
	write_PacRAM(252, 0x00000000);
	write_PacRAM(253, 0x00000000);
	write_PacRAM(254, 0x00000000);
	write_PacRAM(255, 0x00000000);
	write_PacRAM(256, 0x00000000);
	
};


void init_vidmem(void) {
	
    write_vidmem(0, 0x00);
    write_vidmem(1, 0x00);
    write_vidmem(2, 0x00);
    write_vidmem(3, 0x00);
    write_vidmem(4, 0x00);
    write_vidmem(5, 0x00);
    write_vidmem(6, 0x00);
    write_vidmem(7, 0x00);
    write_vidmem(8, 0x00);
    write_vidmem(9, 0x00);
    write_vidmem(10, 0x00);
    write_vidmem(11, 0x00);
    write_vidmem(12, 0x00);
    write_vidmem(13, 0x00);
    write_vidmem(14, 0x00);
    write_vidmem(15, 0x00);
    write_vidmem(16, 0x00);
    write_vidmem(17, 0x00);
    write_vidmem(18, 0x00);
    write_vidmem(19, 0x00);
    write_vidmem(20, 0x00);
    write_vidmem(21, 0x00);
    write_vidmem(22, 0x00);
    write_vidmem(23, 0x00);
    write_vidmem(24, 0x00);
    write_vidmem(25, 0x00);
    write_vidmem(26, 0x00);
    write_vidmem(27, 0x00);
    write_vidmem(28, 0x00);
    write_vidmem(29, 0x00);
    write_vidmem(30, 0x00);
    write_vidmem(31, 0x00);
    write_vidmem(32, 0x00);
    write_vidmem(33, 0x00);
    write_vidmem(34, 0x00);
    write_vidmem(35, 0x00);
    write_vidmem(36, 0x00);
    write_vidmem(37, 0x00);
    write_vidmem(38, 0x00);
    write_vidmem(39, 0x00);
    write_vidmem(40, 0x00);
    write_vidmem(41, 0x00);
    write_vidmem(42, 0x00);
    write_vidmem(43, 0x00);
    write_vidmem(44, 0x00);
    write_vidmem(45, 0x00);
    write_vidmem(46, 0x00);
    write_vidmem(47, 0x00);
    write_vidmem(48, 0x00);
    write_vidmem(49, 0x00);
    write_vidmem(50, 0x00);
    write_vidmem(51, 0x00);
    write_vidmem(52, 0x00);
    write_vidmem(53, 0x00);
    write_vidmem(54, 0x00);
    write_vidmem(55, 0x00);
    write_vidmem(56, 0x00);
    write_vidmem(57, 0x00);
    write_vidmem(58, 0x00);
    write_vidmem(59, 0x00);
    write_vidmem(60, 0x00);
    write_vidmem(61, 0x00);
    write_vidmem(62, 0x00);
    write_vidmem(63, 0x00);
    write_vidmem(64, 0x00);
    write_vidmem(65, 0x00);
    write_vidmem(66, 0x00);
    write_vidmem(67, 0x00);
    write_vidmem(68, 0x00);
    write_vidmem(69, 0x00);
    write_vidmem(70, 0x00);
    write_vidmem(71, 0x00);
    write_vidmem(72, 0x00);
    write_vidmem(73, 0x00);
    write_vidmem(74, 0x00);
    write_vidmem(75, 0x00);
    write_vidmem(76, 0x00);
    write_vidmem(77, 0x00);
    write_vidmem(78, 0x00);
    write_vidmem(79, 0x00);
    write_vidmem(80, 0x00);
    write_vidmem(81, 0x00);
    write_vidmem(82, 0x00);
    write_vidmem(83, 0x00);
    write_vidmem(84, 0x00);
    write_vidmem(85, 0x00);
    write_vidmem(86, 0x00);
    write_vidmem(87, 0x00);
    write_vidmem(88, 0x00);
    write_vidmem(89, 0x00);
    write_vidmem(90, 0x00);
    write_vidmem(91, 0x00);
    write_vidmem(92, 0x00);
    write_vidmem(93, 0x00);
    write_vidmem(94, 0x00);
    write_vidmem(95, 0x00);
    write_vidmem(96, 0x00);
    write_vidmem(97, 0x00);
    write_vidmem(98, 0x00);
    write_vidmem(99, 0x00);
    write_vidmem(100, 0x00);
    write_vidmem(101, 0x00);
    write_vidmem(102, 0x00);
    write_vidmem(103, 0x00);
    write_vidmem(104, 0x00);
    write_vidmem(105, 0x00);
    write_vidmem(106, 0x00);
    write_vidmem(107, 0x00);
    write_vidmem(108, 0x00);
    write_vidmem(109, 0x00);
    write_vidmem(110, 0x00);
    write_vidmem(111, 0x00);
    write_vidmem(112, 0x00);
    write_vidmem(113, 0x00);
    write_vidmem(114, 0x00);
    write_vidmem(115, 0x00);
    write_vidmem(116, 0x00);
    write_vidmem(117, 0x00);
    write_vidmem(118, 0x00);
    write_vidmem(119, 0x00);
    write_vidmem(120, 0x00);
    write_vidmem(121, 0x00);
    write_vidmem(122, 0x00);
    write_vidmem(123, 0x00);
    write_vidmem(124, 0x00);
    write_vidmem(125, 0x00);
    write_vidmem(126, 0x00);
    write_vidmem(127, 0x00);
    write_vidmem(128, 0x00);
    write_vidmem(129, 0x00);
    write_vidmem(130, 0x00);
    write_vidmem(131, 0x00);
    write_vidmem(132, 0x00);
    write_vidmem(133, 0x00);
    write_vidmem(134, 0x00);
    write_vidmem(135, 0x00);
    write_vidmem(136, 0x00);
    write_vidmem(137, 0x00);
    write_vidmem(138, 0x00);
    write_vidmem(139, 0x00);
    write_vidmem(140, 0x00);
    write_vidmem(141, 0x00);
    write_vidmem(142, 0x00);
    write_vidmem(143, 0x00);
    write_vidmem(144, 0x00);
    write_vidmem(145, 0x00);
    write_vidmem(146, 0x00);
    write_vidmem(147, 0x00);
    write_vidmem(148, 0x00);
    write_vidmem(149, 0x00);
    write_vidmem(150, 0x00);
    write_vidmem(151, 0x00);
    write_vidmem(152, 0x00);
    write_vidmem(153, 0x00);
    write_vidmem(154, 0x00);
    write_vidmem(155, 0x00);
    write_vidmem(156, 0x00);
    write_vidmem(157, 0x00);
    write_vidmem(158, 0x00);
    write_vidmem(159, 0x00);
    write_vidmem(160, 0x00);
    write_vidmem(161, 0x00);
    write_vidmem(162, 0x00);
    write_vidmem(163, 0x00);
    write_vidmem(164, 0x00);
    write_vidmem(165, 0x00);
    write_vidmem(166, 0x00);
    write_vidmem(167, 0x01);
    write_vidmem(168, 0x02);
    write_vidmem(169, 0x02);
    write_vidmem(170, 0x02);
    write_vidmem(171, 0x02);
    write_vidmem(172, 0x02);
    write_vidmem(173, 0x02);
    write_vidmem(174, 0x02);
    write_vidmem(175, 0x02);
    write_vidmem(176, 0x02);
    write_vidmem(177, 0x02);
    write_vidmem(178, 0x02);
    write_vidmem(179, 0x0A);
    write_vidmem(180, 0x02);
    write_vidmem(181, 0x02);
    write_vidmem(182, 0x02);
    write_vidmem(183, 0x02);
    write_vidmem(184, 0x02);
    write_vidmem(185, 0x02);
    write_vidmem(186, 0x02);
    write_vidmem(187, 0x02);
    write_vidmem(188, 0x02);
    write_vidmem(189, 0x02);
    write_vidmem(190, 0x02);
    write_vidmem(191, 0x03);
    write_vidmem(192, 0x00);
    write_vidmem(193, 0x00);
    write_vidmem(194, 0x00);
    write_vidmem(195, 0x00);
    write_vidmem(196, 0x00);
    write_vidmem(197, 0x00);
    write_vidmem(198, 0x00);
    write_vidmem(199, 0x00);
    write_vidmem(200, 0x00);
    write_vidmem(201, 0x00);
    write_vidmem(202, 0x00);
    write_vidmem(203, 0x00);
    write_vidmem(204, 0x00);
    write_vidmem(205, 0x00);
    write_vidmem(206, 0x00);
    write_vidmem(207, 0x04);
    write_vidmem(208, 0x50);
    write_vidmem(209, 0x50);
    write_vidmem(210, 0x50);
    write_vidmem(211, 0x50);
    write_vidmem(212, 0x50);
    write_vidmem(213, 0x50);
    write_vidmem(214, 0x50);
    write_vidmem(215, 0x50);
    write_vidmem(216, 0x50);
    write_vidmem(217, 0x50);
    write_vidmem(218, 0x50);
    write_vidmem(219, 0x15);
    write_vidmem(220, 0x50);
    write_vidmem(221, 0x50);
    write_vidmem(222, 0x50);
    write_vidmem(223, 0x50);
    write_vidmem(224, 0x50);
    write_vidmem(225, 0x50);
    write_vidmem(226, 0x50);
    write_vidmem(227, 0x50);
    write_vidmem(228, 0x50);
    write_vidmem(229, 0x50);
    write_vidmem(230, 0x50);
    write_vidmem(231, 0x08);
    write_vidmem(232, 0x00);
    write_vidmem(233, 0x00);
    write_vidmem(234, 0x00);
    write_vidmem(235, 0x00);
    write_vidmem(236, 0x00);
    write_vidmem(237, 0x00);
    write_vidmem(238, 0x00);
    write_vidmem(239, 0x00);
    write_vidmem(240, 0x00);
    write_vidmem(241, 0x00);
    write_vidmem(242, 0x00);
    write_vidmem(243, 0x00);
    write_vidmem(244, 0x00);
    write_vidmem(245, 0x00);
    write_vidmem(246, 0x00);
    write_vidmem(247, 0x04);
    write_vidmem(248, 0x50);
    write_vidmem(249, 0x0B);
    write_vidmem(250, 0x0C);
    write_vidmem(251, 0x0D);
    write_vidmem(252, 0x50);
    write_vidmem(253, 0x0B);
    write_vidmem(254, 0x0C);
    write_vidmem(255, 0x0C);
    write_vidmem(256, 0x0C);
    write_vidmem(257, 0x0D);
    write_vidmem(258, 0x50);
    write_vidmem(259, 0x15);
    write_vidmem(260, 0x50);
    write_vidmem(261, 0x0B);
    write_vidmem(262, 0x0C);
    write_vidmem(263, 0x0C);
    write_vidmem(264, 0x0C);
    write_vidmem(265, 0x0D);
    write_vidmem(266, 0x50);
    write_vidmem(267, 0x0B);
    write_vidmem(268, 0x0C);
    write_vidmem(269, 0x0D);
    write_vidmem(270, 0x50);
    write_vidmem(271, 0x08);
    write_vidmem(272, 0x00);
    write_vidmem(273, 0x00);
    write_vidmem(274, 0x00);
    write_vidmem(275, 0x00);
    write_vidmem(276, 0x00);
    write_vidmem(277, 0x00);
    write_vidmem(278, 0x00);
    write_vidmem(279, 0x00);
    write_vidmem(280, 0x00);
    write_vidmem(281, 0x00);
    write_vidmem(282, 0x00);
    write_vidmem(283, 0x00);
    write_vidmem(284, 0x00);
    write_vidmem(285, 0x00);
    write_vidmem(286, 0x00);
    write_vidmem(287, 0x04);
    write_vidmem(288, 0x50);
    write_vidmem(289, 0x20);
    write_vidmem(290, 0x10);
    write_vidmem(291, 0x0F);
    write_vidmem(292, 0x50);
    write_vidmem(293, 0x20);
    write_vidmem(294, 0x10);
    write_vidmem(295, 0x10);
    write_vidmem(296, 0x10);
    write_vidmem(297, 0x0F);
    write_vidmem(298, 0x50);
    write_vidmem(299, 0x16);
    write_vidmem(300, 0x50);
    write_vidmem(301, 0x20);
    write_vidmem(302, 0x10);
    write_vidmem(303, 0x10);
    write_vidmem(304, 0x10);
    write_vidmem(305, 0x0F);
    write_vidmem(306, 0x50);
    write_vidmem(307, 0x20);
    write_vidmem(308, 0x10);
    write_vidmem(309, 0x0F);
    write_vidmem(310, 0x50);
    write_vidmem(311, 0x08);
    write_vidmem(312, 0x00);
    write_vidmem(313, 0x00);
    write_vidmem(314, 0x00);
    write_vidmem(315, 0x00);
    write_vidmem(316, 0x00);
    write_vidmem(317, 0x00);
    write_vidmem(318, 0x00);
    write_vidmem(319, 0x00);
    write_vidmem(320, 0x00);
    write_vidmem(321, 0x00);
    write_vidmem(322, 0x00);
    write_vidmem(323, 0x00);
    write_vidmem(324, 0x00);
    write_vidmem(325, 0x00);
    write_vidmem(326, 0x00);
    write_vidmem(327, 0x04);
    write_vidmem(328, 0x50);
    write_vidmem(329, 0x50);
    write_vidmem(330, 0x50);
    write_vidmem(331, 0x50);
    write_vidmem(332, 0x50);
    write_vidmem(333, 0x50);
    write_vidmem(334, 0x50);
    write_vidmem(335, 0x50);
    write_vidmem(336, 0x50);
    write_vidmem(337, 0x50);
    write_vidmem(338, 0x50);
    write_vidmem(339, 0x50);
    write_vidmem(340, 0x50);
    write_vidmem(341, 0x50);
    write_vidmem(342, 0x50);
    write_vidmem(343, 0x50);
    write_vidmem(344, 0x50);
    write_vidmem(345, 0x50);
    write_vidmem(346, 0x50);
    write_vidmem(347, 0x50);
    write_vidmem(348, 0x50);
    write_vidmem(349, 0x50);
    write_vidmem(350, 0x50);
    write_vidmem(351, 0x08);
    write_vidmem(352, 0x00);
    write_vidmem(353, 0x00);
    write_vidmem(354, 0x00);
    write_vidmem(355, 0x00);
    write_vidmem(356, 0x00);
    write_vidmem(357, 0x00);
    write_vidmem(358, 0x00);
    write_vidmem(359, 0x00);
    write_vidmem(360, 0x00);
    write_vidmem(361, 0x00);
    write_vidmem(362, 0x00);
    write_vidmem(363, 0x00);
    write_vidmem(364, 0x00);
    write_vidmem(365, 0x00);
    write_vidmem(366, 0x00);
    write_vidmem(367, 0x04);
    write_vidmem(368, 0x50);
    write_vidmem(369, 0x12);
    write_vidmem(370, 0x1B);
    write_vidmem(371, 0x13);
    write_vidmem(372, 0x50);
    write_vidmem(373, 0x14);
    write_vidmem(374, 0x50);
    write_vidmem(375, 0x12);
    write_vidmem(376, 0x1B);
    write_vidmem(377, 0x1B);
    write_vidmem(378, 0x1B);
    write_vidmem(379, 0x19);
    write_vidmem(380, 0x1B);
    write_vidmem(381, 0x1B);
    write_vidmem(382, 0x1B);
    write_vidmem(383, 0x13);
    write_vidmem(384, 0x50);
    write_vidmem(385, 0x14);
    write_vidmem(386, 0x50);
    write_vidmem(387, 0x12);
    write_vidmem(388, 0x1B);
    write_vidmem(389, 0x13);
    write_vidmem(390, 0x50);
    write_vidmem(391, 0x08);
    write_vidmem(392, 0x00);
    write_vidmem(393, 0x00);
    write_vidmem(394, 0x00);
    write_vidmem(395, 0x00);
    write_vidmem(396, 0x00);
    write_vidmem(397, 0x00);
    write_vidmem(398, 0x00);
    write_vidmem(399, 0x00);
    write_vidmem(400, 0x00);
    write_vidmem(401, 0x00);
    write_vidmem(402, 0x00);
    write_vidmem(403, 0x00);
    write_vidmem(404, 0x00);
    write_vidmem(405, 0x00);
    write_vidmem(406, 0x00);
    write_vidmem(407, 0x04);
    write_vidmem(408, 0x50);
    write_vidmem(409, 0x50);
    write_vidmem(410, 0x50);
    write_vidmem(411, 0x50);
    write_vidmem(412, 0x50);
    write_vidmem(413, 0x15);
    write_vidmem(414, 0x50);
    write_vidmem(415, 0x50);
    write_vidmem(416, 0x50);
    write_vidmem(417, 0x50);
    write_vidmem(418, 0x50);
    write_vidmem(419, 0x15);
    write_vidmem(420, 0x50);
    write_vidmem(421, 0x50);
    write_vidmem(422, 0x50);
    write_vidmem(423, 0x50);
    write_vidmem(424, 0x50);
    write_vidmem(425, 0x15);
    write_vidmem(426, 0x50);
    write_vidmem(427, 0x50);
    write_vidmem(428, 0x50);
    write_vidmem(429, 0x50);
    write_vidmem(430, 0x50);
    write_vidmem(431, 0x08);
    write_vidmem(432, 0x00);
    write_vidmem(433, 0x00);
    write_vidmem(434, 0x00);
    write_vidmem(435, 0x00);
    write_vidmem(436, 0x00);
    write_vidmem(437, 0x00);
    write_vidmem(438, 0x00);
    write_vidmem(439, 0x00);
    write_vidmem(440, 0x00);
    write_vidmem(441, 0x00);
    write_vidmem(442, 0x00);
    write_vidmem(443, 0x00);
    write_vidmem(444, 0x00);
    write_vidmem(445, 0x00);
    write_vidmem(446, 0x00);
    write_vidmem(447, 0x06);
    write_vidmem(448, 0x07);
    write_vidmem(449, 0x07);
    write_vidmem(450, 0x07);
    write_vidmem(451, 0x21);
    write_vidmem(452, 0x50);
    write_vidmem(453, 0x17);
    write_vidmem(454, 0x1B);
    write_vidmem(455, 0x1B);
    write_vidmem(456, 0x1B);
    write_vidmem(457, 0x13);
    write_vidmem(458, 0x50);
    write_vidmem(459, 0x16);
    write_vidmem(460, 0x50);
    write_vidmem(461, 0x12);
    write_vidmem(462, 0x1B);
    write_vidmem(463, 0x1B);
    write_vidmem(464, 0x1B);
    write_vidmem(465, 0x18);
    write_vidmem(466, 0x50);
    write_vidmem(467, 0x23);
    write_vidmem(468, 0x07);
    write_vidmem(469, 0x07);
    write_vidmem(470, 0x07);
    write_vidmem(471, 0x05);
    write_vidmem(472, 0x00);
    write_vidmem(473, 0x00);
    write_vidmem(474, 0x00);
    write_vidmem(475, 0x00);
    write_vidmem(476, 0x00);
    write_vidmem(477, 0x00);
    write_vidmem(478, 0x00);
    write_vidmem(479, 0x00);
    write_vidmem(480, 0x00);
    write_vidmem(481, 0x00);
    write_vidmem(482, 0x00);
    write_vidmem(483, 0x00);
    write_vidmem(484, 0x00);
    write_vidmem(485, 0x00);
    write_vidmem(486, 0x00);
    write_vidmem(487, 0x00);
    write_vidmem(488, 0x00);
    write_vidmem(489, 0x00);
    write_vidmem(490, 0x00);
    write_vidmem(491, 0x04);
    write_vidmem(492, 0x50);
    write_vidmem(493, 0x15);
    write_vidmem(494, 0x50);
    write_vidmem(495, 0x50);
    write_vidmem(496, 0x50);
    write_vidmem(497, 0x50);
    write_vidmem(498, 0x50);
    write_vidmem(499, 0x50);
    write_vidmem(500, 0x50);
    write_vidmem(501, 0x50);
    write_vidmem(502, 0x50);
    write_vidmem(503, 0x50);
    write_vidmem(504, 0x50);
    write_vidmem(505, 0x15);
    write_vidmem(506, 0x50);
    write_vidmem(507, 0x08);
    write_vidmem(508, 0x00);
    write_vidmem(509, 0x00);
    write_vidmem(510, 0x00);
    write_vidmem(511, 0x00);
    write_vidmem(512, 0x00);
    write_vidmem(513, 0x00);
    write_vidmem(514, 0x00);
    write_vidmem(515, 0x00);
    write_vidmem(516, 0x00);
    write_vidmem(517, 0x00);
    write_vidmem(518, 0x00);
    write_vidmem(519, 0x00);
    write_vidmem(520, 0x00);
    write_vidmem(521, 0x00);
    write_vidmem(522, 0x00);
    write_vidmem(523, 0x00);
    write_vidmem(524, 0x00);
    write_vidmem(525, 0x00);
    write_vidmem(526, 0x02);
    write_vidmem(527, 0x02);
    write_vidmem(528, 0x02);
    write_vidmem(529, 0x02);
    write_vidmem(530, 0x02);
    write_vidmem(531, 0x22);
    write_vidmem(532, 0x50);
    write_vidmem(533, 0x16);
    write_vidmem(534, 0x50);
    write_vidmem(535, 0x0B);
    write_vidmem(536, 0x0C);
    write_vidmem(537, 0x0C);
    write_vidmem(538, 0x0C);
    write_vidmem(539, 0x51);
    write_vidmem(540, 0x0C);
    write_vidmem(541, 0x0C);
    write_vidmem(542, 0x0C);
    write_vidmem(543, 0x0D);
    write_vidmem(544, 0x50);
    write_vidmem(545, 0x16);
    write_vidmem(546, 0x50);
    write_vidmem(547, 0x24);
    write_vidmem(548, 0x02);
    write_vidmem(549, 0x02);
    write_vidmem(550, 0x02);
    write_vidmem(551, 0x02);
    write_vidmem(552, 0x02);
    write_vidmem(553, 0x00);
    write_vidmem(554, 0x00);
    write_vidmem(555, 0x00);
    write_vidmem(556, 0x00);
    write_vidmem(557, 0x00);
    write_vidmem(558, 0x00);
    write_vidmem(559, 0x00);
    write_vidmem(560, 0x00);
    write_vidmem(561, 0x00);
    write_vidmem(562, 0x00);
    write_vidmem(563, 0x00);
    write_vidmem(564, 0x00);
    write_vidmem(565, 0x00);
    write_vidmem(566, 0x50);
    write_vidmem(567, 0x50);
    write_vidmem(568, 0x50);
    write_vidmem(569, 0x50);
    write_vidmem(570, 0x50);
    write_vidmem(571, 0x50);
    write_vidmem(572, 0x50);
    write_vidmem(573, 0x50);
    write_vidmem(574, 0x50);
    write_vidmem(575, 0x11);
    write_vidmem(576, 0x00);
    write_vidmem(577, 0x00);
    write_vidmem(578, 0x00);
    write_vidmem(579, 0x00);
    write_vidmem(580, 0x00);
    write_vidmem(581, 0x00);
    write_vidmem(582, 0x00);
    write_vidmem(583, 0x0E);
    write_vidmem(584, 0x50);
    write_vidmem(585, 0x50);
    write_vidmem(586, 0x50);
    write_vidmem(587, 0x50);
    write_vidmem(588, 0x50);
    write_vidmem(589, 0x50);
    write_vidmem(590, 0x50);
    write_vidmem(591, 0x50);
    write_vidmem(592, 0x50);
    write_vidmem(593, 0x00);
    write_vidmem(594, 0x00);
    write_vidmem(595, 0x00);
    write_vidmem(596, 0x00);
    write_vidmem(597, 0x00);
    write_vidmem(598, 0x00);
    write_vidmem(599, 0x00);
    write_vidmem(600, 0x00);
    write_vidmem(601, 0x00);
    write_vidmem(602, 0x00);
    write_vidmem(603, 0x00);
    write_vidmem(604, 0x00);
    write_vidmem(605, 0x00);
    write_vidmem(606, 0x07);
    write_vidmem(607, 0x07);
    write_vidmem(608, 0x07);
    write_vidmem(609, 0x07);
    write_vidmem(610, 0x07);
    write_vidmem(611, 0x21);
    write_vidmem(612, 0x50);
    write_vidmem(613, 0x14);
    write_vidmem(614, 0x50);
    write_vidmem(615, 0x11);
    write_vidmem(616, 0x00);
    write_vidmem(617, 0x00);
    write_vidmem(618, 0x00);
    write_vidmem(619, 0x00);
    write_vidmem(620, 0x00);
    write_vidmem(621, 0x00);
    write_vidmem(622, 0x00);
    write_vidmem(623, 0x0E);
    write_vidmem(624, 0x50);
    write_vidmem(625, 0x14);
    write_vidmem(626, 0x50);
    write_vidmem(627, 0x23);
    write_vidmem(628, 0x07);
    write_vidmem(629, 0x07);
    write_vidmem(630, 0x07);
    write_vidmem(631, 0x07);
    write_vidmem(632, 0x07);
    write_vidmem(633, 0x00);
    write_vidmem(634, 0x00);
    write_vidmem(635, 0x00);
    write_vidmem(636, 0x00);
    write_vidmem(637, 0x00);
    write_vidmem(638, 0x00);
    write_vidmem(639, 0x00);
    write_vidmem(640, 0x00);
    write_vidmem(641, 0x00);
    write_vidmem(642, 0x00);
    write_vidmem(643, 0x00);
    write_vidmem(644, 0x00);
    write_vidmem(645, 0x00);
    write_vidmem(646, 0x00);
    write_vidmem(647, 0x00);
    write_vidmem(648, 0x00);
    write_vidmem(649, 0x00);
    write_vidmem(650, 0x00);
    write_vidmem(651, 0x04);
    write_vidmem(652, 0x50);
    write_vidmem(653, 0x15);
    write_vidmem(654, 0x50);
    write_vidmem(655, 0x20);
    write_vidmem(656, 0x10);
    write_vidmem(657, 0x10);
    write_vidmem(658, 0x10);
    write_vidmem(659, 0x10);
    write_vidmem(660, 0x10);
    write_vidmem(661, 0x10);
    write_vidmem(662, 0x10);
    write_vidmem(663, 0x0F);
    write_vidmem(664, 0x50);
    write_vidmem(665, 0x15);
    write_vidmem(666, 0x50);
    write_vidmem(667, 0x08);
    write_vidmem(668, 0x00);
    write_vidmem(669, 0x00);
    write_vidmem(670, 0x00);
    write_vidmem(671, 0x00);
    write_vidmem(672, 0x00);
    write_vidmem(673, 0x00);
    write_vidmem(674, 0x00);
    write_vidmem(675, 0x00);
    write_vidmem(676, 0x00);
    write_vidmem(677, 0x00);
    write_vidmem(678, 0x00);
    write_vidmem(679, 0x00);
    write_vidmem(680, 0x00);
    write_vidmem(681, 0x00);
    write_vidmem(682, 0x00);
    write_vidmem(683, 0x00);
    write_vidmem(684, 0x00);
    write_vidmem(685, 0x00);
    write_vidmem(686, 0x00);
    write_vidmem(687, 0x00);
    write_vidmem(688, 0x00);
    write_vidmem(689, 0x00);
    write_vidmem(690, 0x00);
    write_vidmem(691, 0x04);
    write_vidmem(692, 0x50);
    write_vidmem(693, 0x15);
    write_vidmem(694, 0x50);
    write_vidmem(695, 0x50);
    write_vidmem(696, 0x50);
    write_vidmem(697, 0x50);
    write_vidmem(698, 0x50);
    write_vidmem(699, 0x50);
    write_vidmem(700, 0x50);
    write_vidmem(701, 0x50);
    write_vidmem(702, 0x50);
    write_vidmem(703, 0x50);
    write_vidmem(704, 0x50);
    write_vidmem(705, 0x15);
    write_vidmem(706, 0x50);
    write_vidmem(707, 0x08);
    write_vidmem(708, 0x00);
    write_vidmem(709, 0x00);
    write_vidmem(710, 0x00);
    write_vidmem(711, 0x00);
    write_vidmem(712, 0x00);
    write_vidmem(713, 0x00);
    write_vidmem(714, 0x00);
    write_vidmem(715, 0x00);
    write_vidmem(716, 0x00);
    write_vidmem(717, 0x00);
    write_vidmem(718, 0x00);
    write_vidmem(719, 0x00);
    write_vidmem(720, 0x00);
    write_vidmem(721, 0x00);
    write_vidmem(722, 0x00);
    write_vidmem(723, 0x00);
    write_vidmem(724, 0x00);
    write_vidmem(725, 0x00);
    write_vidmem(726, 0x00);
    write_vidmem(727, 0x01);
    write_vidmem(728, 0x02);
    write_vidmem(729, 0x02);
    write_vidmem(730, 0x02);
    write_vidmem(731, 0x22);
    write_vidmem(732, 0x50);
    write_vidmem(733, 0x16);
    write_vidmem(734, 0x50);
    write_vidmem(735, 0x12);
    write_vidmem(736, 0x1B);
    write_vidmem(737, 0x1B);
    write_vidmem(738, 0x1B);
    write_vidmem(739, 0x19);
    write_vidmem(740, 0x1B);
    write_vidmem(741, 0x1B);
    write_vidmem(742, 0x1B);
    write_vidmem(743, 0x13);
    write_vidmem(744, 0x50);
    write_vidmem(745, 0x16);
    write_vidmem(746, 0x50);
    write_vidmem(747, 0x24);
    write_vidmem(748, 0x02);
    write_vidmem(749, 0x02);
    write_vidmem(750, 0x02);
    write_vidmem(751, 0x03);
    write_vidmem(752, 0x00);
    write_vidmem(753, 0x00);
    write_vidmem(754, 0x00);
    write_vidmem(755, 0x00);
    write_vidmem(756, 0x00);
    write_vidmem(757, 0x00);
    write_vidmem(758, 0x00);
    write_vidmem(759, 0x00);
    write_vidmem(760, 0x00);
    write_vidmem(761, 0x00);
    write_vidmem(762, 0x00);
    write_vidmem(763, 0x00);
    write_vidmem(764, 0x00);
    write_vidmem(765, 0x00);
    write_vidmem(766, 0x00);
    write_vidmem(767, 0x04);
    write_vidmem(768, 0x50);
    write_vidmem(769, 0x50);
    write_vidmem(770, 0x50);
    write_vidmem(771, 0x50);
    write_vidmem(772, 0x50);
    write_vidmem(773, 0x50);
    write_vidmem(774, 0x50);
    write_vidmem(775, 0x50);
    write_vidmem(776, 0x50);
    write_vidmem(777, 0x50);
    write_vidmem(778, 0x50);
    write_vidmem(779, 0x15);
    write_vidmem(780, 0x50);
    write_vidmem(781, 0x50);
    write_vidmem(782, 0x50);
    write_vidmem(783, 0x50);
    write_vidmem(784, 0x50);
    write_vidmem(785, 0x50);
    write_vidmem(786, 0x50);
    write_vidmem(787, 0x50);
    write_vidmem(788, 0x50);
    write_vidmem(789, 0x50);
    write_vidmem(790, 0x50);
    write_vidmem(791, 0x08);
    write_vidmem(792, 0x00);
    write_vidmem(793, 0x00);
    write_vidmem(794, 0x00);
    write_vidmem(795, 0x00);
    write_vidmem(796, 0x00);
    write_vidmem(797, 0x00);
    write_vidmem(798, 0x00);
    write_vidmem(799, 0x00);
    write_vidmem(800, 0x00);
    write_vidmem(801, 0x00);
    write_vidmem(802, 0x00);
    write_vidmem(803, 0x00);
    write_vidmem(804, 0x00);
    write_vidmem(805, 0x00);
    write_vidmem(806, 0x00);
    write_vidmem(807, 0x04);
    write_vidmem(808, 0x50);
    write_vidmem(809, 0x12);
    write_vidmem(810, 0x1B);
    write_vidmem(811, 0x1E);
    write_vidmem(812, 0x50);
    write_vidmem(813, 0x12);
    write_vidmem(814, 0x1B);
    write_vidmem(815, 0x1B);
    write_vidmem(816, 0x1B);
    write_vidmem(817, 0x13);
    write_vidmem(818, 0x50);
    write_vidmem(819, 0x16);
    write_vidmem(820, 0x50);
    write_vidmem(821, 0x12);
    write_vidmem(822, 0x1B);
    write_vidmem(823, 0x1B);
    write_vidmem(824, 0x1B);
    write_vidmem(825, 0x13);
    write_vidmem(826, 0x50);
    write_vidmem(827, 0x1F);
    write_vidmem(828, 0x1B);
    write_vidmem(829, 0x13);
    write_vidmem(830, 0x50);
    write_vidmem(831, 0x08);
    write_vidmem(832, 0x00);
    write_vidmem(833, 0x00);
    write_vidmem(834, 0x00);
    write_vidmem(835, 0x00);
    write_vidmem(836, 0x00);
    write_vidmem(837, 0x00);
    write_vidmem(838, 0x00);
    write_vidmem(839, 0x00);
    write_vidmem(840, 0x00);
    write_vidmem(841, 0x00);
    write_vidmem(842, 0x00);
    write_vidmem(843, 0x00);
    write_vidmem(844, 0x00);
    write_vidmem(845, 0x00);
    write_vidmem(846, 0x00);
    write_vidmem(847, 0x04);
    write_vidmem(848, 0x50);
    write_vidmem(849, 0x50);
    write_vidmem(850, 0x50);
    write_vidmem(851, 0x15);
    write_vidmem(852, 0x50);
    write_vidmem(853, 0x50);
    write_vidmem(854, 0x50);
    write_vidmem(855, 0x50);
    write_vidmem(856, 0x50);
    write_vidmem(857, 0x50);
    write_vidmem(858, 0x50);
    write_vidmem(859, 0x50);
    write_vidmem(860, 0x50);
    write_vidmem(861, 0x50);
    write_vidmem(862, 0x50);
    write_vidmem(863, 0x50);
    write_vidmem(864, 0x50);
    write_vidmem(865, 0x50);
    write_vidmem(866, 0x50);
    write_vidmem(867, 0x15);
    write_vidmem(868, 0x50);
    write_vidmem(869, 0x50);
    write_vidmem(870, 0x50);
    write_vidmem(871, 0x08);
    write_vidmem(872, 0x00);
    write_vidmem(873, 0x00);
    write_vidmem(874, 0x00);
    write_vidmem(875, 0x00);
    write_vidmem(876, 0x00);
    write_vidmem(877, 0x00);
    write_vidmem(878, 0x00);
    write_vidmem(879, 0x00);
    write_vidmem(880, 0x00);
    write_vidmem(881, 0x00);
    write_vidmem(882, 0x00);
    write_vidmem(883, 0x00);
    write_vidmem(884, 0x00);
    write_vidmem(885, 0x00);
    write_vidmem(886, 0x00);
    write_vidmem(887, 0x1D);
    write_vidmem(888, 0x1B);
    write_vidmem(889, 0x13);
    write_vidmem(890, 0x50);
    write_vidmem(891, 0x16);
    write_vidmem(892, 0x50);
    write_vidmem(893, 0x14);
    write_vidmem(894, 0x50);
    write_vidmem(895, 0x12);
    write_vidmem(896, 0x1B);
    write_vidmem(897, 0x1B);
    write_vidmem(898, 0x1B);
    write_vidmem(899, 0x19);
    write_vidmem(900, 0x1B);
    write_vidmem(901, 0x1B);
    write_vidmem(902, 0x1B);
    write_vidmem(903, 0x13);
    write_vidmem(904, 0x50);
    write_vidmem(905, 0x14);
    write_vidmem(906, 0x50);
    write_vidmem(907, 0x16);
    write_vidmem(908, 0x50);
    write_vidmem(909, 0x12);
    write_vidmem(910, 0x1B);
    write_vidmem(911, 0x1C);
    write_vidmem(912, 0x00);
    write_vidmem(913, 0x00);
    write_vidmem(914, 0x00);
    write_vidmem(915, 0x00);
    write_vidmem(916, 0x00);
    write_vidmem(917, 0x00);
    write_vidmem(918, 0x00);
    write_vidmem(919, 0x00);
    write_vidmem(920, 0x00);
    write_vidmem(921, 0x00);
    write_vidmem(922, 0x00);
    write_vidmem(923, 0x00);
    write_vidmem(924, 0x00);
    write_vidmem(925, 0x00);
    write_vidmem(926, 0x00);
    write_vidmem(927, 0x04);
    write_vidmem(928, 0x50);
    write_vidmem(929, 0x50);
    write_vidmem(930, 0x50);
    write_vidmem(931, 0x50);
    write_vidmem(932, 0x50);
    write_vidmem(933, 0x15);
    write_vidmem(934, 0x50);
    write_vidmem(935, 0x50);
    write_vidmem(936, 0x50);
    write_vidmem(937, 0x50);
    write_vidmem(938, 0x50);
    write_vidmem(939, 0x15);
    write_vidmem(940, 0x50);
    write_vidmem(941, 0x50);
    write_vidmem(942, 0x50);
    write_vidmem(943, 0x50);
    write_vidmem(944, 0x50);
    write_vidmem(945, 0x15);
    write_vidmem(946, 0x50);
    write_vidmem(947, 0x50);
    write_vidmem(948, 0x50);
    write_vidmem(949, 0x50);
    write_vidmem(950, 0x50);
    write_vidmem(951, 0x08);
    write_vidmem(952, 0x00);
    write_vidmem(953, 0x00);
    write_vidmem(954, 0x00);
    write_vidmem(955, 0x00);
    write_vidmem(956, 0x00);
    write_vidmem(957, 0x00);
    write_vidmem(958, 0x00);
    write_vidmem(959, 0x00);
    write_vidmem(960, 0x00);
    write_vidmem(961, 0x00);
    write_vidmem(962, 0x00);
    write_vidmem(963, 0x00);
    write_vidmem(964, 0x00);
    write_vidmem(965, 0x00);
    write_vidmem(966, 0x00);
    write_vidmem(967, 0x04);
    write_vidmem(968, 0x50);
    write_vidmem(969, 0x12);
    write_vidmem(970, 0x1B);
    write_vidmem(971, 0x1B);
    write_vidmem(972, 0x1B);
    write_vidmem(973, 0x1A);
    write_vidmem(974, 0x1B);
    write_vidmem(975, 0x1B);
    write_vidmem(976, 0x1B);
    write_vidmem(977, 0x13);
    write_vidmem(978, 0x50);
    write_vidmem(979, 0x16);
    write_vidmem(980, 0x50);
    write_vidmem(981, 0x12);
    write_vidmem(982, 0x1B);
    write_vidmem(983, 0x1B);
    write_vidmem(984, 0x1B);
    write_vidmem(985, 0x1A);
    write_vidmem(986, 0x1B);
    write_vidmem(987, 0x1B);
    write_vidmem(988, 0x1B);
    write_vidmem(989, 0x13);
    write_vidmem(990, 0x50);
    write_vidmem(991, 0x08);
    write_vidmem(992, 0x00);
    write_vidmem(993, 0x00);
    write_vidmem(994, 0x00);
    write_vidmem(995, 0x00);
    write_vidmem(996, 0x00);
    write_vidmem(997, 0x00);
    write_vidmem(998, 0x00);
    write_vidmem(999, 0x00);
    write_vidmem(1000, 0x00);
    write_vidmem(1001, 0x00);
    write_vidmem(1002, 0x00);
    write_vidmem(1003, 0x00);
    write_vidmem(1004, 0x00);
    write_vidmem(1005, 0x00);
    write_vidmem(1006, 0x00);
    write_vidmem(1007, 0x04);
    write_vidmem(1008, 0x50);
    write_vidmem(1009, 0x50);
    write_vidmem(1010, 0x50);
    write_vidmem(1011, 0x50);
    write_vidmem(1012, 0x50);
    write_vidmem(1013, 0x50);
    write_vidmem(1014, 0x50);
    write_vidmem(1015, 0x50);
    write_vidmem(1016, 0x50);
    write_vidmem(1017, 0x50);
    write_vidmem(1018, 0x50);
    write_vidmem(1019, 0x50);
    write_vidmem(1020, 0x50);
    write_vidmem(1021, 0x50);
    write_vidmem(1022, 0x50);
    write_vidmem(1023, 0x50);
    write_vidmem(1024, 0x50);
    write_vidmem(1025, 0x50);
    write_vidmem(1026, 0x50);
    write_vidmem(1027, 0x50);
    write_vidmem(1028, 0x50);
    write_vidmem(1029, 0x50);
    write_vidmem(1030, 0x50);
    write_vidmem(1031, 0x08);
    write_vidmem(1032, 0x00);
    write_vidmem(1033, 0x00);
    write_vidmem(1034, 0x00);
    write_vidmem(1035, 0x00);
    write_vidmem(1036, 0x00);
    write_vidmem(1037, 0x00);
    write_vidmem(1038, 0x00);
    write_vidmem(1039, 0x00);
    write_vidmem(1040, 0x00);
    write_vidmem(1041, 0x00);
    write_vidmem(1042, 0x00);
    write_vidmem(1043, 0x00);
    write_vidmem(1044, 0x00);
    write_vidmem(1045, 0x00);
    write_vidmem(1046, 0x00);
    write_vidmem(1047, 0x06);
    write_vidmem(1048, 0x07);
    write_vidmem(1049, 0x07);
    write_vidmem(1050, 0x07);
    write_vidmem(1051, 0x07);
    write_vidmem(1052, 0x07);
    write_vidmem(1053, 0x07);
    write_vidmem(1054, 0x07);
    write_vidmem(1055, 0x07);
    write_vidmem(1056, 0x07);
    write_vidmem(1057, 0x07);
    write_vidmem(1058, 0x07);
    write_vidmem(1059, 0x07);
    write_vidmem(1060, 0x07);
    write_vidmem(1061, 0x07);
    write_vidmem(1062, 0x07);
    write_vidmem(1063, 0x07);
    write_vidmem(1064, 0x07);
    write_vidmem(1065, 0x07);
    write_vidmem(1066, 0x07);
    write_vidmem(1067, 0x07);
    write_vidmem(1068, 0x07);
    write_vidmem(1069, 0x07);
    write_vidmem(1070, 0x07);
    write_vidmem(1071, 0x05);
    write_vidmem(1072, 0x00);
    write_vidmem(1073, 0x00);
    write_vidmem(1074, 0x00);
    write_vidmem(1075, 0x00);
    write_vidmem(1076, 0x00);
    write_vidmem(1077, 0x00);
    write_vidmem(1078, 0x00);
    write_vidmem(1079, 0x00);
    write_vidmem(1080, 0x00);
    write_vidmem(1081, 0x00);
    write_vidmem(1082, 0x00);
    write_vidmem(1083, 0x00);
    write_vidmem(1084, 0x00);
    write_vidmem(1085, 0x00);
    write_vidmem(1086, 0x00);
    write_vidmem(1087, 0x00);
    write_vidmem(1088, 0x00);
    write_vidmem(1089, 0x00);
    write_vidmem(1090, 0x00);
    write_vidmem(1091, 0x00);
    write_vidmem(1092, 0x00);
    write_vidmem(1093, 0x00);
    write_vidmem(1094, 0x00);
    write_vidmem(1095, 0x00);
    write_vidmem(1096, 0x00);
    write_vidmem(1097, 0x00);
    write_vidmem(1098, 0x00);
    write_vidmem(1099, 0x00);
    write_vidmem(1100, 0x00);
    write_vidmem(1101, 0x00);
    write_vidmem(1102, 0x00);
    write_vidmem(1103, 0x00);
    write_vidmem(1104, 0x00);
    write_vidmem(1105, 0x00);
    write_vidmem(1106, 0x00);
    write_vidmem(1107, 0x00);
    write_vidmem(1108, 0x00);
    write_vidmem(1109, 0x00);
    write_vidmem(1110, 0x00);
    write_vidmem(1111, 0x00);
    write_vidmem(1112, 0x00);
    write_vidmem(1113, 0x00);
    write_vidmem(1114, 0x00);
    write_vidmem(1115, 0x00);
    write_vidmem(1116, 0x00);
    write_vidmem(1117, 0x00);
    write_vidmem(1118, 0x00);
    write_vidmem(1119, 0x00);
    write_vidmem(1120, 0x00);
    write_vidmem(1121, 0x00);
    write_vidmem(1122, 0x00);
    write_vidmem(1123, 0x00);
    write_vidmem(1124, 0x00);
    write_vidmem(1125, 0x00);
    write_vidmem(1126, 0x00);
    write_vidmem(1127, 0x00);
    write_vidmem(1128, 0x00);
    write_vidmem(1129, 0x00);
    write_vidmem(1130, 0x00);
    write_vidmem(1131, 0x00);
    write_vidmem(1132, 0x00);
    write_vidmem(1133, 0x00);
    write_vidmem(1134, 0x00);
    write_vidmem(1135, 0x00);
    write_vidmem(1136, 0x00);
    write_vidmem(1137, 0x00);
    write_vidmem(1138, 0x00);
    write_vidmem(1139, 0x00);
    write_vidmem(1140, 0x00);
    write_vidmem(1141, 0x00);
    write_vidmem(1142, 0x00);
    write_vidmem(1143, 0x00);
    write_vidmem(1144, 0x00);
    write_vidmem(1145, 0x00);
    write_vidmem(1146, 0x00);
    write_vidmem(1147, 0x00);
    write_vidmem(1148, 0x00);
    write_vidmem(1149, 0x00);
    write_vidmem(1150, 0x00);
    write_vidmem(1151, 0x00);
    write_vidmem(1152, 0x00);
    write_vidmem(1153, 0x00);
    write_vidmem(1154, 0x00);
    write_vidmem(1155, 0x00);
    write_vidmem(1156, 0x00);
    write_vidmem(1157, 0x00);
    write_vidmem(1158, 0x00);
    write_vidmem(1159, 0x00);
    write_vidmem(1160, 0x00);
    write_vidmem(1161, 0x00);
    write_vidmem(1162, 0x00);
    write_vidmem(1163, 0x00);
    write_vidmem(1164, 0x00);
    write_vidmem(1165, 0x00);
    write_vidmem(1166, 0x00);
    write_vidmem(1167, 0x00);
    write_vidmem(1168, 0x00);
    write_vidmem(1169, 0x00);
    write_vidmem(1170, 0x00);
    write_vidmem(1171, 0x00);
    write_vidmem(1172, 0x00);
    write_vidmem(1173, 0x00);
    write_vidmem(1174, 0x00);
    write_vidmem(1175, 0x00);
    write_vidmem(1176, 0x00);
    write_vidmem(1177, 0x00);
    write_vidmem(1178, 0x00);
    write_vidmem(1179, 0x00);
    write_vidmem(1180, 0x00);
    write_vidmem(1181, 0x00);
    write_vidmem(1182, 0x00);
    write_vidmem(1183, 0x00);
    write_vidmem(1184, 0x00);
    write_vidmem(1185, 0x00);
    write_vidmem(1186, 0x00);
    write_vidmem(1187, 0x00);
    write_vidmem(1188, 0x00);
    write_vidmem(1189, 0x00);
    write_vidmem(1190, 0x00);
    write_vidmem(1191, 0x00);
    write_vidmem(1192, 0x00);
    write_vidmem(1193, 0x00);
    write_vidmem(1194, 0x00);
    write_vidmem(1195, 0x00);
    write_vidmem(1196, 0x00);
    write_vidmem(1197, 0x00);
    write_vidmem(1198, 0x00);
    write_vidmem(1199, 0x00);
		
};

//////////////////////////////////////////////////////////////////
// Other Functions
//////////////////////////////////////////////////////////////////

uint32_t pack_spritedata(uint16_t x, uint16_t y) {
	
	return ((uint32_t)y << 10) | (x_mask & x);
	
};

int reverse_direction(int dir) {
	
	if ( dir == GHOST_UP ) {
		return GHOST_DOWN;
	};
	
	if ( dir == GHOST_DOWN ) {
		return GHOST_UP;
	};
	
	if ( dir == GHOST_RIGHT ) {
		return GHOST_LEFT;
	};
	
	if ( dir == GHOST_LEFT ) {
		return GHOST_RIGHT;
	};
	
	return -1;
	
};

int find_new_x(int dir) {
	if ( dir == GHOST_RIGHT ){
		return ghost_x + STEP_DIST;
	}
	if ( dir == GHOST_LEFT ) {
		return ghost_x - STEP_DIST;
	}
	else
		return ghost_x;
};
	
int find_new_y(int dir) {
	if ( dir == GHOST_DOWN ){
		return ghost_y + STEP_DIST;
	}
	else if ( dir == GHOST_UP ) {
		return ghost_y - STEP_DIST;
	}
	else
		return ghost_y;
};


//////////////////////////////////////////////////////////////////
// Main Function
//////////////////////////////////////////////////////////////////

int main(void) {
			
	// Initialisation
	
	pacman_x = starting_x1;
	pacman_y = starting_y1;
	
	ghost_x = starting_x2;
	ghost_y = starting_y2;
	
	ghost_dir = GHOST_LEFT;
	
	write_GhostRAM(0, ((uint32_t)starting_y2 << 10) | (x_mask & starting_x2));
	write_PacRAM(0, ((uint32_t)starting_y1 << 10) | (x_mask & starting_x1));
	
	init_ghost_ram();
	init_pacman_ram();
	init_vidmem();
	
//	enable_button_interrupt();

	uint8_t joystick;

	while (1) {

		write_timer_count(80);
		
		while ( read_timer_status() );
		
	////////////////////////////////////////////////////////////////
	// PACMAN
	////////////////////////////////////////////////////////////////
		
		joystick = read_joystick();
				
		// Joystick Movement UP DOWN LEFT RIGHT
		// Pacman origin in top left
		// Extend Right and Down Distance and also try the other is_corridor.
		if ( joystick & PAC_RIGHT ) { // right
			if ( is_corridor((pacman_x + 8), pacman_y) & is_corridor((pacman_x + 8), (pacman_y + 15)) ) {
				pacman_x += STEP_DIST; 
			};
		};
		
		if ( (joystick & PAC_LEFT) ) {	// left
			if ( is_corridor((pacman_x - 8), pacman_y) & is_corridor((pacman_x - 8), (pacman_y + 15)) ) {
				pacman_x -= STEP_DIST; 
			};
		};
		
		if ( (joystick & PAC_UP) ) { // up
			if ( is_corridor(pacman_x, (pacman_y - 8)) & is_corridor((pacman_x + 15), (pacman_y - 8)) ) {
				pacman_y -= STEP_DIST; 
			};
		};
		
		if ( (joystick & PAC_DOWN) ) { // down
			if ( is_corridor(pacman_x, (pacman_y +  8)) & is_corridor((pacman_x + 15), (pacman_y + 8)) ) {
				pacman_y += STEP_DIST; 
			};
		};
			
		// side paths
		// if ( pacman_y >= 224 && pacman_y <= 240 ) {
			// if ( pacman_x <= 96 ) {
				// pacman_x = 528;
			// }
			// else
				// pacman_x = 96;
		// };
		
		write_PacRAM(0, pack_spritedata(pacman_x, pacman_y));
		
		// +7 so it is when the centre of pacman enters the tile
		// doesn't return tiles upon reset
		uint32_t index, tile;
		
		index = ((pacman_y + 7) / 16) * 40 + ((pacman_x + 7) / 16);
		tile = read_vidmem(index);
		
		if ( tile == CHERRY ) {
			score += 1;
			write_vidmem(index, NULL_CHAR);
			
			// Display score in 3 digits
			write_vidmem(55, '0' + (score % 10));
			write_vidmem(54, '0' + ((score / 10) % 10));
			write_vidmem(53, '0' + ((score / 100) % 10));
			
		};
		
	////////////////////////////////////////////////////////////////
	// GHOST
	////////////////////////////////////////////////////////////////	
		
		int best_dir = -1;
		int best_dist = 9999999;
		
		for (int dir = 0; dir < 4; dir++ ) {
		
			if ( dir == reverse_direction(ghost_dir) ) {	// cannot turn around
				continue;
			};
			
			int new_x, new_y;
			new_x = find_new_x(dir);
			new_y = find_new_y(dir);
			
			
			if ( !(is_corridor(new_x, new_y)) ){		// cannot be a wall
				continue;
			};
			
			int dist = abs(new_x - pacman_x) + abs(new_y - pacman_y);
			
			// smaller distance wins, preferring current direction
			if ( (dist < best_dist) || (dist == best_dist && dir == ghost_dir) ) {
				best_dist = dist;
				best_dir = dir;
			};
			
			// if we haven't chosen a direction at this point
			if ( best_dir == -1 ){ 									
				int reverse = reverse_direction(ghost_dir);			// turn around
				new_x = find_new_x(reverse);
				new_y = find_new_y(reverse);
				if ( is_corridor(new_x, new_y) ) {
					best_dir = reverse;
				}
				else {
					best_dir = ghost_dir;
				};	
			};	
		};
		
		ghost_dir = best_dir;

		
		// Update Ghost Position based on Direction
		if ( ghost_dir == GHOST_UP ) {
			ghost_y -= 8;
		};
		if ( ghost_dir == GHOST_DOWN ) {
			ghost_y += 8;
		};
		if ( ghost_dir == GHOST_RIGHT ) {
			ghost_x += 8;
		};
		if ( ghost_dir == GHOST_LEFT ) {
			ghost_x -= 8;
		};
		
		write_GhostRAM(0, pack_spritedata(ghost_x, ghost_y));
		
		// Ghost catches Pacman
		
		///////////////////////////////////////////////////////////////
		// IMPROVE EDGE DETECTION
		// FIX SIDEWAYS TUNNELS OR REMOVE
		// ADD GAME OVER
		// ADD GHOST + PACMAN COLLISION
		// EXTRACT
		///////////////////////////////////////////////////////////////
	};
	
	
};
