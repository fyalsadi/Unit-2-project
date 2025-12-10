// ELEC2645 Unit 2 Project - Engineering Waveform Tool
// With safe user input handling and extra maths/engineering tools

#include <stdio.h>   // printf, FILE, fgets
#include <stdlib.h>  // exit, strtod, atoi
#include <math.h>    // sqrt, fabs, sin, M_PI
#include <string.h>  // strlen, strchr, strcspn

// ---------- Constants ----------
#define MAX_SAMPLES 1000
#define INPUT_BUF   100   // for safe input lines

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------- Global Data ----------
double samples[MAX_SAMPLES];
int sampleCount = 0;
int dataAvailable = 0;   // 0 = no data, 1 = data ready

char signalType[40];     // description of the signal

// ---------- Function Prototypes ----------

// menu system
void main_menu(void);
int  get_user_input(void);
void select_menu_item(int input);
void print_main_menu(void);
void go_back_to_main(void);

// waveform menu items
void menu_item_1(void);   // Enter samples manually
void menu_item_3(void);   // Show basic info (min, max, peak-to-peak)
void menu_item_4(void);   // DC offset and RMS
void menu_item_5(void);   // Crest factor
void menu_item_6(void);   // Zero-crossings
void menu_item_7(void);   // Save report to file
void menu_item_8(void);   // Classify signal type
void menu_item_9(void);   // Smooth waveform (moving average)
void menu_item_10(void);  // Detect peaks
void menu_item_12(void);  // Generate test signal (sine wave)
void menu_item_18(void);  // Signal frequency analyser

// maths/engineering menu items
void menu_item_13(void);  // Basic Calculator
void menu_item_14(void);  // Ohm's Law Calculator
void menu_item_15(void);  // Resistor Calculator (series/parallel)
void menu_item_16(void);  // Quadratic Equation Solver
void menu_item_17(void);  // Unit Converter with context
void menu_item_19(void);  // Sensor data converter
void menu_item_20(void);  // Filter design helper
void menu_item_21(void);  // AI-style assistant

// helper functions (analysis)
double find_min(const double a[], int n);
double find_max(const double a[], int n);
double calc_average(const double a[], int n);
double calc_rms(const double a[], int n);
int    count_zero_crossings(const double a[], int n);
int    detect_peaks(const double a[], int n);
void   smooth_moving_average(double a[], int n, int windowSize);
void   generate_sine_wave(double a[], int n, double amplitude, int periods);

// helper functions (safe input)
void flush_input_buffer(void);
int  is_integer_string(const char *s);
int  safe_get_int(const char *prompt, int *out_value);
int  safe_get_double(const char *prompt, double *out_value);
int  safe_get_filename(const char *prompt, char *buf, size_t buf_sz);

// ========== Main Function ==========
int main(void)
{
    main_menu();
    return 0;
}

// ========== Menu Functions ==========

void main_menu(void)
{
    print_main_menu();
    int input = get_user_input();
    select_menu_item(input);
}

int get_user_input(void)
{
    int input;
    if (safe_get_int("Select item: ", &input) != 0) {
        printf("Input error. Exiting.\n");
        exit(1);
    }
    return input;
}

void select_menu_item(int input)
{
    switch (input) {
        case 1:  menu_item_1();  break;
        case 3:  menu_item_3();  break;
        case 4:  menu_item_4();  break;
        case 5:  menu_item_5();  break;
        case 6:  menu_item_6();  break;
        case 7:  menu_item_7();  break;
        case 8:  menu_item_8();  break;
        case 9:  menu_item_9();  break;
        case 10: menu_item_10(); break;

        case 12: menu_item_12(); break;
        case 13: menu_item_13(); break;
        case 14: menu_item_14(); break;
        case 15: menu_item_15(); break;
        case 16: menu_item_16(); break;
        case 17: menu_item_17(); break;
        case 18: menu_item_18(); break;
        case 19: menu_item_19(); break;
        case 20: menu_item_20(); break;
        case 21: menu_item_21(); break;
        case 0:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid selection. Exiting...\n");
            exit(1);
    }
}

void print_main_menu(void)
{
    printf("\n---------- Engineering Waveform & Maths Tool ----------\n");
    printf("\t1.  Enter waveform samples manually\n");
    printf("\t3.  Show basic info (min, max, peak-to-peak)\n");
    printf("\t4.  Calculate DC offset and RMS\n");
    printf("\t5.  Calculate crest factor (peak / RMS)\n");
    printf("\t6.  Count zero-crossings\n");
    printf("\t7.  Save analysis report to file\n");
    printf("\t8.  Classify signal type\n");
    printf("\t9.  Smooth waveform (moving average)\n");
    printf("\t10. Detect peaks\n");
    printf("\t12. Generate test signal (sine wave)\n");
    printf("\t13. Basic calculator\n");
    printf("\t14. Ohm's law calculator (V = I * R)\n");
    printf("\t15. Resistor calculator (series & parallel)\n");
    printf("\t16. Quadratic equation solver\n");
    printf("\t17. Unit converter with context\n");
    printf("\t18. Signal frequency analyser\n");
    printf("\t19. Sensor data converter (ADC -> units)\n");
    printf("\t20. Filter design helper (RC)\n");
    printf("\t21. AI-style assistant (help & explanations)\n");
    printf("\t0.  Exit\n");
    printf("--------------------------------------------------------\n");
}

void go_back_to_main(void)
{
    char input;
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (scanf(" %c", &input) != 1) {
            flush_input_buffer();
            continue;
        }
    } while (input != 'b' && input != 'B');

    flush_input_buffer(); // clean up any extra chars
    main_menu();
}

// ========== Menu Item Functions ==========
//
// 1–12 & 18: waveform-related
// 13–17 & 19–21: maths/engineering tools

// 1) Enter samples manually
void menu_item_1(void)
{
    int i, n;

    printf("\n>> Menu 1: Enter waveform samples\n");

    if (safe_get_int("How many samples (1–1000)? ", &n) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (n < 1 || n > MAX_SAMPLES) {
        printf("Invalid number of samples.\n");
        sampleCount = 0;
        dataAvailable = 0;
        go_back_to_main();
        return;
    }

    sampleCount = n;

    for (i = 0; i < sampleCount; i++) {
        char prompt[40];
        snprintf(prompt, sizeof(prompt), "Sample %d: ", i + 1);
        if (safe_get_double(prompt, &samples[i]) != 0) {
            printf("Input error while reading sample.\n");
            sampleCount = i;
            dataAvailable = (sampleCount > 0);
            go_back_to_main();
            return;
        }
    }

    dataAvailable = 1;
    printf("Samples stored.\n");
    go_back_to_main();
}

// 3) Show basic info
void menu_item_3(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    double min = find_min(samples, sampleCount);
    double max = find_max(samples, sampleCount);
    double p2p = max - min;

    printf("\n>> Menu 3: Basic information\n");
    printf("Number of samples : %d\n", sampleCount);
    printf("Minimum value     : %.4f\n", min);
    printf("Maximum value     : %.4f\n", max);
    printf("Peak-to-peak      : %.4f\n", p2p);

    go_back_to_main();
}

// 4) DC offset and RMS
void menu_item_4(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    double dc  = calc_average(samples, sampleCount);
    double rms = calc_rms(samples, sampleCount);

    printf("\n>> Menu 4: DC offset and RMS\n");
    printf("DC offset (average): %.4f\n", dc);
    printf("RMS value          : %.4f\n", rms);

    go_back_to_main();
}

// 5) Crest factor
void menu_item_5(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    double maxAbs = fabs(find_max(samples, sampleCount));
    double minAbs = fabs(find_min(samples, sampleCount));
    double peak   = (maxAbs > minAbs) ? maxAbs : minAbs;
    double rms    = calc_rms(samples, sampleCount);

    printf("\n>> Menu 5: Crest factor\n");

    if (rms == 0.0) {
        printf("RMS is zero, crest factor undefined.\n");
    } else {
        double crest = peak / rms;
        printf("Peak value  : %.4f\n", peak);
        printf("RMS value   : %.4f\n", rms);
        printf("Crest factor: %.4f\n", crest);
    }

    go_back_to_main();
}

// 6) Zero-crossings
void menu_item_6(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    int zeroCount = count_zero_crossings(samples, sampleCount);

    printf("\n>> Menu 6: Zero-crossings\n");
    printf("Number of zero-crossings: %d\n", zeroCount);

    go_back_to_main();
}

// 7) Save report to file
void menu_item_7(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    char filename[100];
    FILE *fp;

    printf("\n>> Menu 7: Save report to file\n");

    if (safe_get_filename("Enter filename for report (e.g. report.txt): ",
                          filename, sizeof(filename)) != 0) {
        printf("Filename error.\n");
        go_back_to_main();
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        go_back_to_main();
        return;
    }

    double min  = find_min(samples, sampleCount);
    double max  = find_max(samples, sampleCount);
    double p2p  = max - min;
    double dc   = calc_average(samples, sampleCount);
    double rms  = calc_rms(samples, sampleCount);
    double maxA = fabs(max);
    double minA = fabs(min);
    double peak = (maxA > minA) ? maxA : minA;
    double crest = (rms != 0.0) ? peak / rms : 0.0;
    int zeroCount = count_zero_crossings(samples, sampleCount);

    fprintf(fp, "Waveform Analysis Report\n");
    fprintf(fp, "========================\n");
    fprintf(fp, "Number of samples : %d\n", sampleCount);
    fprintf(fp, "Minimum value     : %.4f\n", min);
    fprintf(fp, "Maximum value     : %.4f\n", max);
    fprintf(fp, "Peak-to-peak      : %.4f\n", p2p);
    fprintf(fp, "DC offset         : %.4f\n", dc);
    fprintf(fp, "RMS value         : %.4f\n", rms);
    fprintf(fp, "Peak value        : %.4f\n", peak);
    fprintf(fp, "Crest factor      : %.4f\n", crest);
    fprintf(fp, "Zero-crossings    : %d\n", zeroCount);

    fclose(fp);

    printf("Report saved to %s\n", filename);
    go_back_to_main();
}

// 8) Classify signal type
void menu_item_8(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    double dc   = calc_average(samples, sampleCount);
    double rms  = calc_rms(samples, sampleCount);
    int zeroCount = count_zero_crossings(samples, sampleCount);

    if (rms == 0.0) {
        snprintf(signalType, sizeof(signalType), "Flat / zero signal");
    } else if (fabs(dc) > 0.8 * rms && zeroCount == 0) {
        snprintf(signalType, sizeof(signalType), "Mostly DC signal");
    } else if (zeroCount > sampleCount / 10) {
        snprintf(signalType, sizeof(signalType), "AC / oscillating signal");
    } else {
        snprintf(signalType, sizeof(signalType), "Mixed / noisy signal");
    }

    printf("\n>> Menu 8: Signal classification\n");
    printf("DC offset      : %.4f\n", dc);
    printf("RMS value      : %.4f\n", rms);
    printf("Zero-crossings : %d\n", zeroCount);
    printf("Signal type    : %s\n", signalType);

    go_back_to_main();
}

// 9) Smooth waveform (moving average)
void menu_item_9(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    int window;
    printf("\n>> Menu 9: Smooth waveform (moving average)\n");

    if (safe_get_int("Enter window size (odd, e.g. 3 or 5): ", &window) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (window < 1 || window > sampleCount || window % 2 == 0) {
        printf("Invalid window size.\n");
        go_back_to_main();
        return;
    }

    smooth_moving_average(samples, sampleCount, window);
    printf("Waveform smoothed in-place.\n");

    go_back_to_main();
}

// 10) Detect peaks
void menu_item_10(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    printf("\n>> Menu 10: Peak detection\n");

    int peakCount = detect_peaks(samples, sampleCount);

    printf("Total peaks detected: %d\n", peakCount);
    if (peakCount == 0) {
        printf("No clear peaks found.\n");
    }

    go_back_to_main();
}

// 12) Generate test signal (sine wave)
void menu_item_12(void)
{
    int n;
    double amplitude;
    int periods;

    printf("\n>> Menu 12: Generate sine test signal\n");

    if (safe_get_int("Number of samples (1–1000): ", &n) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (n < 1 || n > MAX_SAMPLES) {
        printf("Invalid number of samples.\n");
        go_back_to_main();
        return;
    }

    if (safe_get_double("Amplitude (e.g. 1.0): ", &amplitude) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (safe_get_int("Number of periods over the samples (e.g. 1, 2, 3): ",
                     &periods) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    generate_sine_wave(samples, n, amplitude, periods);

    sampleCount = n;
    dataAvailable = 1;

    printf("Sine wave generated.\n");

    go_back_to_main();
}

// 13) Basic Calculator
void menu_item_13(void)
{
    double a, b, result;
    int op;

    printf("\n>> Menu 13: Basic Calculator\n");
    if (safe_get_double("Enter first number: ", &a) != 0 ||
        safe_get_double("Enter second number: ", &b) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    printf("Choose operation:\n");
    printf(" 1) Add (a + b)\n");
    printf(" 2) Subtract (a - b)\n");
    printf(" 3) Multiply (a * b)\n");
    printf(" 4) Divide (a / b)\n");

    if (safe_get_int("Enter choice (1–4): ", &op) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    switch (op) {
        case 1:
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
        case 2:
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
        case 3:
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
        case 4:
            if (b == 0.0) {
                printf("Error: division by zero.\n");
            } else {
                result = a / b;
                printf("Result: %.6f\n", result);
            }
            break;
        default:
            printf("Invalid operation.\n");
            break;
    }

    go_back_to_main();
}

// 14) Ohm's Law Calculator (V = I * R)
void menu_item_14(void)
{
    int choice;
    double V, I, R;

    printf("\n>> Menu 14: Ohm's Law Calculator (V = I * R)\n");
    printf("What do you want to calculate?\n");
    printf(" 1) Voltage (V)\n");
    printf(" 2) Current (I)\n");
    printf(" 3) Resistance (R)\n");

    if (safe_get_int("Enter choice (1–3): ", &choice) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    switch (choice) {
        case 1:
            if (safe_get_double("Enter current I (A): ", &I) != 0 ||
                safe_get_double("Enter resistance R (ohms): ", &R) != 0) {
                printf("Input error.\n");
                break;
            }
            V = I * R;
            printf("Voltage V = %.6f V\n", V);
            break;

        case 2:
            if (safe_get_double("Enter voltage V (V): ", &V) != 0 ||
                safe_get_double("Enter resistance R (ohms): ", &R) != 0) {
                printf("Input error.\n");
                break;
            }
            if (R == 0.0) {
                printf("Error: resistance cannot be zero.\n");
            } else {
                I = V / R;
                printf("Current I = %.6f A\n", I);
            }
            break;

        case 3:
            if (safe_get_double("Enter voltage V (V): ", &V) != 0 ||
                safe_get_double("Enter current I (A): ", &I) != 0) {
                printf("Input error.\n");
                break;
            }
            if (I == 0.0) {
                printf("Error: current cannot be zero.\n");
            } else {
                R = V / I;
                printf("Resistance R = %.6f ohms\n", R);
            }
            break;

        default:
            printf("Invalid choice.\n");
            break;
    }

    go_back_to_main();
}

// 15) Resistor Calculator (series & parallel)
void menu_item_15(void)
{
    int n, mode;
    double R[20];
    double R_total = 0.0;

    printf("\n>> Menu 15: Resistor Calculator (series & parallel)\n");

    if (safe_get_int("How many resistors (1–20)? ", &n) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (n < 1 || n > 20) {
        printf("Invalid number of resistors.\n");
        go_back_to_main();
        return;
    }

    for (int i = 0; i < n; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "R%d (ohms): ", i + 1);
        if (safe_get_double(prompt, &R[i]) != 0) {
            printf("Input error.\n");
            go_back_to_main();
            return;
        }
        if (R[i] < 0.0) {
            printf("Warning: negative resistance entered.\n");
        }
    }

    printf("Choose configuration:\n");
    printf(" 1) Series\n");
    printf(" 2) Parallel\n");

    if (safe_get_int("Enter choice (1–2): ", &mode) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (mode == 1) {
        R_total = 0.0;
        for (int i = 0; i < n; i++) {
            R_total += R[i];
        }
        printf("Total series resistance: %.6f ohms\n", R_total);
    } else if (mode == 2) {
        double inv_sum = 0.0;
        int valid = 1;
        for (int i = 0; i < n; i++) {
            if (R[i] == 0.0) {
                printf("Error: resistor value 0 in parallel -> infinite current.\n");
                valid = 0;
                break;
            }
            inv_sum += 1.0 / R[i];
        }
        if (valid && inv_sum != 0.0) {
            R_total = 1.0 / inv_sum;
            printf("Total parallel resistance: %.6f ohms\n", R_total);
        } else if (valid) {
            printf("Invalid parallel configuration.\n");
        }
    } else {
        printf("Invalid mode.\n");
    }

    go_back_to_main();
}

// 16) Quadratic Equation Solver
void menu_item_16(void)
{
    double a, b, c;

    printf("\n>> Menu 16: Quadratic Equation Solver (ax^2 + bx + c = 0)\n");

    if (safe_get_double("Enter a: ", &a) != 0 ||
        safe_get_double("Enter b: ", &b) != 0 ||
        safe_get_double("Enter c: ", &c) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (a == 0.0) {
        if (b == 0.0) {
            printf("No solution (a = 0, b = 0).\n");
        } else {
            double x = -c / b;
            printf("Linear equation. Single root: x = %.6f\n", x);
        }
        go_back_to_main();
        return;
    }

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0.0) {
        double sqrtD = sqrt(discriminant);
        double x1 = (-b + sqrtD) / (2.0 * a);
        double x2 = (-b - sqrtD) / (2.0 * a);
        printf("Two distinct real roots:\n");
        printf(" x1 = %.6f\n", x1);
        printf(" x2 = %.6f\n", x2);
    } else if (discriminant == 0.0) {
        double x = -b / (2.0 * a);
        printf("One repeated real root:\n");
        printf(" x = %.6f\n", x);
    } else {
        double sqrtD = sqrt(-discriminant);
        double realPart = -b / (2.0 * a);
        double imagPart = sqrtD / (2.0 * a);
        printf("Two complex roots:\n");
        printf(" x1 = %.6f + %.6fi\n", realPart, imagPart);
        printf(" x2 = %.6f - %.6fi\n", realPart, imagPart);
    }

    go_back_to_main();
}

// 17) Unit Converter with context
void menu_item_17(void)
{
    int category;
    int running = 1;

    printf("\n>> Menu 17: Unit Converter with Context\n");

    while (running) {
        printf("\nChoose quantity to convert:\n");
        printf(" 1) Voltage (V <-> mV)\n");
        printf(" 2) Resistance (ohm <-> kilo-ohm)\n");
        printf(" 3) Frequency (Hz <-> kHz)\n");
        printf(" 4) Length (m <-> mm)\n");
        printf(" 0) Return to main menu\n");

        if (safe_get_int("Enter choice (0–4): ", &category) != 0) {
            printf("Input error.\n");
            continue;
        }

        if (category == 0) {
            running = 0;
            break;
        }

        double value;
        int direction;

        switch (category) {
            case 1:
                printf("\nVoltage conversion (1 V = 1000 mV because 'milli' = 1/1000):\n");
                printf(" 1) V -> mV\n");
                printf(" 2) mV -> V\n");
                if (safe_get_int("Direction (1–2): ", &direction) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (safe_get_double("Enter value: ", &value) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (direction == 1) {
                    printf("Result: %.6f V = %.6f mV\n", value, value * 1000.0);
                } else if (direction == 2) {
                    printf("Result: %.6f mV = %.6f V\n", value, value / 1000.0);
                } else {
                    printf("Invalid direction.\n");
                }
                break;

            case 2:
                printf("\nResistance conversion (1 k-ohm = 1000 ohms):\n");
                printf(" 1) ohm -> kilo-ohm\n");
                printf(" 2) kilo-ohm -> ohm\n");
                if (safe_get_int("Direction (1–2): ", &direction) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (safe_get_double("Enter value: ", &value) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (direction == 1) {
                    printf("Result: %.6f ohm = %.6f k-ohm\n", value, value / 1000.0);
                } else if (direction == 2) {
                    printf("Result: %.6f k-ohm = %.6f ohm\n", value, value * 1000.0);
                } else {
                    printf("Invalid direction.\n");
                }
                break;

            case 3:
                printf("\nFrequency conversion (1 kHz = 1000 Hz):\n");
                printf(" 1) Hz -> kHz\n");
                printf(" 2) kHz -> Hz\n");
                if (safe_get_int("Direction (1–2): ", &direction) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (safe_get_double("Enter value: ", &value) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (direction == 1) {
                    printf("Result: %.6f Hz = %.6f kHz\n", value, value / 1000.0);
                } else if (direction == 2) {
                    printf("Result: %.6f kHz = %.6f Hz\n", value, value * 1000.0);
                } else {
                    printf("Invalid direction.\n");
                }
                break;

            case 4:
                printf("\nLength conversion (1 m = 1000 mm):\n");
                printf(" 1) m -> mm\n");
                printf(" 2) mm -> m\n");
                if (safe_get_int("Direction (1–2): ", &direction) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (safe_get_double("Enter value: ", &value) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (direction == 1) {
                    printf("Result: %.6f m = %.6f mm\n", value, value * 1000.0);
                } else if (direction == 2) {
                    printf("Result: %.6f mm = %.6f m\n", value, value / 1000.0);
                } else {
                    printf("Invalid direction.\n");
                }
                break;

            default:
                printf("Please enter a number from 0 to 4.\n");
                break;
        }
    }

    go_back_to_main();
}

// 18) Signal frequency analyser
void menu_item_18(void)
{
    if (!dataAvailable) {
        printf("\nNo waveform data available. Please enter samples first.\n");
        go_back_to_main();
        return;
    }

    double fs;
    if (safe_get_double("\nEnter sampling frequency fs (Hz): ", &fs) != 0) {
        printf("Input error.\n");
        go_back_to_main();
        return;
    }

    if (fs <= 0.0) {
        printf("Sampling frequency must be positive.\n");
        go_back_to_main();
        return;
    }

    int zeroCount = count_zero_crossings(samples, sampleCount);

    printf("\n>> Menu 18: Signal frequency analyser\n");
    printf("Zero-crossings counted: %d\n", zeroCount);

    if (zeroCount < 2 || sampleCount < 2) {
        printf("Not enough crossings to estimate frequency.\n");
    } else {
        double totalTime = (double)(sampleCount - 1) / fs;
        double cycles = (double)zeroCount / 2.0;
        double freq = cycles / totalTime;
        double period = 1.0 / freq;

        double rms = calc_rms(samples, sampleCount);
        double min = find_min(samples, sampleCount);
        double max = find_max(samples, sampleCount);
        double p2p = max - min;

        printf("Estimated cycles : %.2f\n", cycles);
        printf("Total time       : %.6f s\n", totalTime);
        printf("Estimated freq   : %.3f Hz\n", freq);
        printf("Estimated period : %.6f s\n", period);
        printf("RMS value        : %.4f\n", rms);
        printf("Peak-to-peak     : %.4f\n", p2p);
    }

    go_back_to_main();
}

// 19) Sensor data converter
void menu_item_19(void)
{
    int running = 1;
    printf("\n>> Menu 19: Sensor Data Converter (ADC -> real units)\n");

    while (running) {
        int adc;
        double vref;
        int sensorChoice;

        printf("\nEnter ADC reading (0–4095) or -1 to return to main menu: ");
        if (safe_get_int("", &adc) != 0) {
            printf("Input error.\n");
            continue;
        }
        if (adc == -1) {
            break;
        }
        if (adc < 0 || adc > 4095) {
            printf("ADC reading must be between 0 and 4095.\n");
            continue;
        }

        if (safe_get_double("Enter ADC reference voltage Vref (e.g. 3.3): ", &vref) != 0) {
            printf("Input error.\n");
            continue;
        }
        if (vref <= 0.0) {
            printf("Vref must be positive.\n");
            continue;
        }

        double voltage = (double)adc * vref / 4095.0;
        printf("ADC voltage = %.6f V\n", voltage);

        printf("\nChoose sensor mapping:\n");
        printf(" 1) Temperature sensor (LM35: 10 mV/°C)\n");
        printf(" 2) Pressure sensor (0–5 V -> 0–100 kPa)\n");
        printf(" 3) Accelerometer (0 g at 1.65 V, 0.3 V per g)\n");
        printf(" 0) Return to main menu\n");

        if (safe_get_int("Enter choice (0–3): ", &sensorChoice) != 0) {
            printf("Input error.\n");
            continue;
        }

        if (sensorChoice == 0) {
            running = 0;
            break;
        }

        switch (sensorChoice) {
            case 1: {
                double tempC = voltage / 0.01; // 10 mV/°C = 0.01 V/°C
                printf("Approx temperature: %.2f °C (LM35 model)\n", tempC);
                break;
            }
            case 2: {
                double pressure = (voltage / 5.0) * 100.0;
                printf("Approx pressure: %.2f kPa (0–5 V -> 0–100 kPa model)\n", pressure);
                break;
            }
            case 3: {
                double accel = (voltage - 1.65) / 0.3;
                printf("Approx acceleration: %.2f g (1.65 V at 0 g, 0.3 V/g model)\n", accel);
                break;
            }
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    go_back_to_main();
}

// 20) Filter design helper (RC)
void menu_item_20(void)
{
    int running = 1;

    printf("\n>> Menu 20: Filter Design Helper (RC)\n");

    while (running) {
        int type, mode;
        double R, C, fc;

        printf("\nChoose filter type:\n");
        printf(" 1) Low-pass RC\n");
        printf(" 2) High-pass RC\n");
        printf(" 0) Return to main menu\n");

        if (safe_get_int("Enter choice (0–2): ", &type) != 0) {
            printf("Input error.\n");
            continue;
        }
        if (type == 0) {
            break;
        }
        if (type != 1 && type != 2) {
            printf("Invalid type.\n");
            continue;
        }

        printf("\nWe use fc = 1 / (2*pi*R*C)\n");
        printf("What do you want to calculate?\n");
        printf(" 1) fc given R and C\n");
        printf(" 2) C given R and fc\n");
        printf(" 3) R given C and fc\n");

        if (safe_get_int("Enter choice (1–3): ", &mode) != 0) {
            printf("Input error.\n");
            continue;
        }

        switch (mode) {
            case 1:
                if (safe_get_double("Enter R (ohms): ", &R) != 0 ||
                    safe_get_double("Enter C (farads): ", &C) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (R <= 0.0 || C <= 0.0) {
                    printf("R and C must be positive.\n");
                    break;
                }
                fc = 1.0 / (2.0 * M_PI * R * C);
                printf("Cut-off frequency fc = %.3f Hz\n", fc);
                break;

            case 2:
                if (safe_get_double("Enter R (ohms): ", &R) != 0 ||
                    safe_get_double("Enter fc (Hz): ", &fc) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (R <= 0.0 || fc <= 0.0) {
                    printf("R and fc must be positive.\n");
                    break;
                }
                C = 1.0 / (2.0 * M_PI * R * fc);
                printf("Required capacitance C = %.9f F\n", C);
                break;

            case 3:
                if (safe_get_double("Enter C (farads): ", &C) != 0 ||
                    safe_get_double("Enter fc (Hz): ", &fc) != 0) {
                    printf("Input error.\n");
                    break;
                }
                if (C <= 0.0 || fc <= 0.0) {
                    printf("C and fc must be positive.\n");
                    break;
                }
                R = 1.0 / (2.0 * M_PI * C * fc);
                printf("Required resistance R = %.3f ohms\n", R);
                break;

            default:
                printf("Invalid mode.\n");
                break;
        }
    }

    go_back_to_main();
}

// 21) AI-style assistant (simple help system)
void menu_item_21(void)
{
    int running = 1;
    int choice;

    printf("\n>> Menu 21: AI-style assistant (concept help)\n");

    while (running) {
        printf("\nChoose a topic:\n");
        printf(" 1) What is RMS?\n");
        printf(" 2) Series vs parallel resistors\n");
        printf(" 3) What is crest factor?\n");
        printf(" 4) What is a zero-crossing?\n");
        printf(" 5) Reminder of Ohm's law\n");
        printf(" 0) Return to main menu\n");

        if (safe_get_int("Enter choice (0–5): ", &choice) != 0) {
            printf("Input error.\n");
            continue;
        }

        switch (choice) {
            case 0:
                running = 0;
                break;

            case 1:
                printf("\nRMS (Root Mean Square) is a kind of average for AC signals.\n");
                printf("You square each sample, take the mean, then take the square root.\n");
                printf("For a pure sine with peak A, RMS = A / sqrt(2).\n");
                break;

            case 2:
                printf("\nSeries resistors: R_total = R1 + R2 + ... (same current flows).\n");
                printf("Parallel resistors: 1/R_total = 1/R1 + 1/R2 + ... (same voltage).\n");
                printf("Parallel always gives a smaller equivalent resistance than any branch.\n");
                break;

            case 3:
                printf("\nCrest factor = Peak value / RMS value.\n");
                printf("It tells you how 'spiky' a waveform is.\n");
                printf("For a pure sine wave, crest factor is about 1.414.\n");
                break;

            case 4:
                printf("\nA zero-crossing is when the signal changes sign (positive to negative or vice versa).\n");
                printf("Counting zero-crossings gives an idea of signal frequency for periodic waves.\n");
                break;

            case 5:
                printf("\nOhm's law links voltage, current and resistance: V = I * R.\n");
                printf("Increase R with constant V -> current goes down.\n");
                printf("Increase V with constant R -> current goes up.\n");
                break;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    go_back_to_main();
}

// ========== Helper Functions (analysis) ==========

double find_min(const double a[], int n)
{
    int i;
    double min = a[0];
    for (i = 1; i < n; i++) {
        if (a[i] < min)
            min = a[i];
    }
    return min;
}

double find_max(const double a[], int n)
{
    int i;
    double max = a[0];
    for (i = 1; i < n; i++) {
        if (a[i] > max)
            max = a[i];
    }
    return max;
}

double calc_average(const double a[], int n)
{
    int i;
    double sum = 0.0;
    for (i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum / n;
}

double calc_rms(const double a[], int n)
{
    int i;
    double sumSq = 0.0;
    for (i = 0; i < n; i++) {
        sumSq += a[i] * a[i];
    }
    return sqrt(sumSq / n);
}

int count_zero_crossings(const double a[], int n)
{
    int count = 0;
    for (int i = 1; i < n; i++) {
        if ((a[i - 1] > 0 && a[i] < 0) ||
            (a[i - 1] < 0 && a[i] > 0)) {
            count++;
        }
    }
    return count;
}

// simple peak detection: a peak is higher than both neighbours
int detect_peaks(const double a[], int n)
{
    int count = 0;

    if (n < 3) {
        return 0; // need at least 3 points
    }

    for (int i = 1; i < n - 1; i++) {
        if (a[i] > a[i - 1] && a[i] > a[i + 1]) {
            printf("Peak at index %d, value = %.4f\n", i, a[i]);
            count++;
        }
    }

    return count;
}

// moving average smoothing (in-place)
void smooth_moving_average(double a[], int n, int windowSize)
{
    double temp[MAX_SAMPLES];
    int half = windowSize / 2;

    for (int i = 0; i < n; i++) {
        temp[i] = a[i];
    }

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        int count = 0;

        int start = i - half;
        int end   = i + half;

        if (start < 0) start = 0;
        if (end >= n) end = n - 1;

        for (int j = start; j <= end; j++) {
            sum += temp[j];
            count++;
        }

        a[i] = sum / count;
    }
}

// generate a simple sine wave
void generate_sine_wave(double a[], int n, double amplitude, int periods)
{
    const double TWO_PI = 6.28318530717958647692;
    for (int i = 0; i < n; i++) {
        double t = (double)i / (double)n;  // 0 to 1
        a[i] = amplitude * sin(TWO_PI * periods * t);
    }
}

// ========== Helper Functions (safe input) ==========

void flush_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

int is_integer_string(const char *s)
{
    if (s == NULL || *s == '\0') {
        return 0;
    }

    const char *p = s;

    if (*p == '+' || *p == '-') {
        p++;
    }

    if (*p == '\0') {
        return 0;
    }

    while (*p != '\0') {
        if (*p < '0' || *p > '9') {
            return 0;
        }
        p++;
    }

    return 1;
}

int safe_get_int(const char *prompt, int *out_value)
{
    char line[INPUT_BUF];

    if (out_value == NULL) return -1;

    while (1) {
        if (prompt[0] != '\0')
            printf("%s", prompt);

        if (!fgets(line, sizeof(line), stdin)) {
            flush_input_buffer();
            return -1;
        }

        if (strchr(line, '\n') == NULL) {
            flush_input_buffer();
        }

        line[strcspn(line, "\n")] = '\0';

        if (!is_integer_string(line)) {
            printf("Invalid integer, please try again.\n");
            continue;
        }

        *out_value = atoi(line);
        return 0;
    }
}

int safe_get_double(const char *prompt, double *out_value)
{
    char line[INPUT_BUF];

    if (out_value == NULL) return -1;

    while (1) {
        printf("%s", prompt);

        if (!fgets(line, sizeof(line), stdin)) {
            flush_input_buffer();
            return -1;
        }

        if (strchr(line, '\n') == NULL) {
            flush_input_buffer();
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') {
            printf("Empty input, please try again.\n");
            continue;
        }

        char *endptr = NULL;
        double val = strtod(line, &endptr);

        if (endptr == line) {
            printf("Invalid number, please try again.\n");
            continue;
        }

        while (*endptr == ' ' || *endptr == '\t') {
            endptr++;
        }

        if (*endptr != '\0') {
            printf("Invalid characters after number, please try again.\n");
            continue;
        }

        *out_value = val;
        return 0;
    }
}

int safe_get_filename(const char *prompt, char *buf, size_t buf_sz)
{
    if (buf == NULL || buf_sz == 0) return -1;

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, (int)buf_sz, stdin)) {
            flush_input_buffer();
            return -1;
        }

        if (strchr(buf, '\n') == NULL) {
            flush_input_buffer();
        }

        buf[strcspn(buf, "\n")] = '\0';

        if (buf[0] == '\0') {
            printf("Filename cannot be empty. Please try again.\n");
            continue;
        }

        return 0;
    }
}
