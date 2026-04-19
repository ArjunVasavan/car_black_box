#include "black_box.h"
#include "clcd.h"

void view_dashboard() {
    unsigned char metrics[17] = " TIME   EV   SP ";
    clcd_print(metrics,LINE1(0));
}
