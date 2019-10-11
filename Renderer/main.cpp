#include <stdio.h>
#include "core/window.h"
#include "core/image.h"

int main(void) {
	Window *window;
	Image *image;

	window = window_create("Hello", 800, 600);
	image = image_load("test.tga"/*"demo.png"*/);

	while (!window_should_close(window)) {
		if (input_key_pressed(window, KEY_A)) {
			printf("Key \'A\' pressed\n");
		}
		if (input_button_pressed(window, BUTTON_L)) {
			printf("Left MouseButton pressed\n");
		}

		window_draw_image(window, image);
		input_poll_events();
	}

	image_release(image);
	window_destroy(window);

	return 0;
}