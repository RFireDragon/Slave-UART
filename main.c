#include <libpynq.h>
int main (void)
{
 // initialise all I/O
 pynq_init();
 switchbox_init();
 display_t display;
 display_init(&display);
 buttons_init();
 
 displayFillScreen(&display, RGB_BLACK);
 FontxFile fx16G[2];
 uint8_t buffer_fx16G[FontxGlyphBufSize];
  uint8_t fontWidth_fx16G, fontHeight_fx16G;
  // from which the executable is run -- see InitFontx
  InitFontx(fx16G, "../../fonts/ILGH16XB.FNT", "");
  GetFontx(fx16G, 0, buffer_fx16G, &fontWidth_fx16G, &fontHeight_fx16G);
  displaySetFontDirection(&display, TEXT_DIRECTION0);
 switchbox_set_pin(IO_AR13, SWB_UART0_RX);
 // initialize UART channel 0
 uart_init(UART0);
 // flush FIFOs of UART 0
 uart_reset_fifos (UART0);
 uint8_t listen[] = "Listening for message\n";
 printf("%s", listen);
 displayDrawString(&display, fx16G, 0, fontHeight_fx16G * 1, listen, RGB_WHITE);
 int x = 0;
 int y = fontHeight_fx16G * 2;

 while (1) {
    uint8_t c = uart_recv(UART0);
    printf("%c", c);
    // Negeer carriage return
    if (c == '\r') {
        continue;
    }

    // Ga bij een newline naar de volgende regel
    if (c == '\n') {
        x = 0;
        y += fontHeight_fx16G;
        continue;
    }
    if (c == '*') {
       displayDrawFillRect(&display, x, y, fontWidth_fx16G, fontHeight_fx16G, RGB_BLACK);
       x=0;
       y=fontHeight_fx16G * 2;
        continue;
    }

    uint8_t msg[] = {c, '\0'};

    displayDrawString(&display, fx16G, x, y, msg, RGB_WHITE);



    x += fontWidth_fx16G;
    if (x + fontWidth_fx16G >= 240) {
        x = 0;
        y += fontHeight_fx16G;
    }
 }
 
 display_destroy(&display);
 switchbox_destroy(); 
 buttons_destroy();
 // clean up after use
  pynq_destroy();
  return EXIT_SUCCESS;
}