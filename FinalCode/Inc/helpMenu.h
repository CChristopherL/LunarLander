#ifndef HELPMENU_H_
#define HELPMENU_H_

#include "math.h"

typedef struct {
	uint16_t *helpTitleImage;
	vector_t helpTitleImageSize;
	vector_t helpTitleImagePosition;

	uint16_t *controlsTitleImage;
	vector_t controlsTitleImageSize;
	vector_t controlsTitleImagePosition;

	uint16_t *leftArrowImage;
	vector_t leftArrowImageSize;
	vector_t leftArrowImagePosition;

	uint16_t *rightArrowImage;
	vector_t rightArrowImageSize;
	vector_t rightArrowImagePosition;

	uint16_t *upArrowImage;
	vector_t upArrowImageSize;
	vector_t upArrowImagePosition;

	uint16_t *downArrowImage;
	vector_t downArrowImageSize;
	vector_t downArrowImagePosition;

	uint16_t *button1Image;
	vector_t button1ImageSize;
	vector_t button1ImagePosition;

	uint16_t *button2Image;
	vector_t button2ImageSize;
	vector_t button2ImagePosition;

	uint16_t *joystickTextImage;
	vector_t joystickTextImageSize;
	vector_t joystickTextImagePosition;

	uint16_t *buttonsTextImage;
	vector_t buttonsTextImageSize;
	vector_t buttonsTextImagePosition;

	uint16_t *howToPlayTextImage;
	vector_t howToPlayTextImageSize;
	vector_t howToPlayTextImagePosition;

	uint16_t *objectivesTextImage;
	vector_t objectivesTextImageSize;
	vector_t objectivesTextImagePosition;

	uint16_t *landingPadTextImage;
	vector_t landingPadTextImageSize;
	vector_t landingPadTextImagePosition;

	uint16_t *backButtonImage;
	vector_t backButtonImageSize;
	vector_t backButtonImagePosition;

	uint8_t previousGameControllerState;
	uint8_t mainMenu;
} helpMenu_t;

void initHelpMenu(helpMenu_t *helpMenu);
void updateHelpMenu(helpMenu_t *helpMenu, uint8_t gameControllerState);

#endif /* HELPMENU_H_ */
