/******************************** INCLUDE FILES *******************************/
#include <stdio.h>

/******************************** LOCAL DEFINES *******************************/
#define UNUSED(x) (void)(x)

/******************************** TYPEDEFS ************************************/

/**
 * Print event
 */
typedef enum _print_events_t {
    PRINT_NUMBER = 0, /**< Print the number */
    PRINT_A,  /**< Print A */
    PRINT_B, /**< Print B */
    PRINT_AB, /**< Print AB */
    PRINT_EVENTS, /**< Number of events */
} print_events_t;

typedef void (*print_handler_t) (int);

/****************************** PRINT HANDLERS ********************************/
static void print_number(int number)
{
    fprintf(stdout, "%d", number);
}

static void print_a(int number)
{
    UNUSED(number);
    fprintf(stdout, "A");
}

static void print_b(int number)
{
    UNUSED(number);
    fprintf(stdout, "B");
}

static void print_ab(int number)
{
    UNUSED(number);
    fprintf(stdout, "AB");
}

/******************************** LOCAL DATA **********************************/
static print_handler_t print_handlers[PRINT_EVENTS] = {
    print_number,
    print_a,
    print_b,
    print_ab
};

/***************************** LOCAL FUNCTIONS ********************************/
int fizzbuzz(int n, int end) {
    int a_print = 0b0;
    int b_print = 0b0;
    print_events_t print_event = PRINT_NUMBER;

    /**
     * Encode the print event using 2 bits
     * 0b00 - print the number
     * 0b01 - print A
     * 0b10 - print B
     * 0b11 - print AB
     */
    a_print = (n % 3 == 0);
    b_print = (n % 5 == 0);
    print_event = a_print | b_print << 1;

    print_handlers[print_event](n);

    /**
     * The recursion will be running until the first condition becomes false.
     */
    n < end && fizzbuzz(n + 1, end);

    return 1;
}

int main() {
    fizzbuzz(1, 100);

    fprintf(stdout, "\n");

    return 0;
}
