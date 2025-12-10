# ELEC2645 Unit 2 Project – Engineering Waveform & Maths Tool

This project is a command line interface (CLI) program written in C.  
The program shows a main menu with numbered options. Each menu item calls
a specific function that does a waveform analysis task or a small
engineering / maths calculation based on user input.  

The tool has two main parts:

- **Waveform module** – enter or generate samples, then calculate basic
  info (min, max, peak-to-peak), DC and RMS, crest factor, zero-crossings,
  smoothing, peak detection, classification, and a simple frequency
  estimate using zero-crossings.

- **Maths & utility module** – basic calculator, Ohm’s law, resistor
  calculator (series and parallel), quadratic solver, unit converter,
  sensor (ADC → real units) helper, RC filter helper, and a small
  AI-style help menu that explains key concepts in simple words.

Unlike the early unit activities, this program includes **safe input
handling** (`safe_get_int`, `safe_get_double`, `safe_get_filename`) so
that invalid text is rejected and the program does not crash.

### 1 Run code

The main program is in `main.c` (Engineering Waveform & Maths Tool).  
It needs compiling before it can run.

In the terminal you can type:

```bash
gcc main.c -lm -o main.out

The -lm flag links the math library because the program uses functions
like sqrt, fabs and sin.
Then run:
./main.out

This will show the main menu with options 0–21.
Type a menu number, press Enter, follow the prompts, and then press
b to go back to the main menu when you are finished with that item.

### 1 Run code
The program `main.c` needs compiling before it can run. In the terminal you can type `gcc main.c -o main.out` and then `./main.out` to run the new program.




### 2 The project
The end product is the completed code for both waveform tools and maths tools within a one menu-based application. The key menu items are:
Waveform processing (1, 3–10, 12, 18). 
 1: Manually input the samples into samples[].
3: Display the basic info (min, max, peak-to-peak).
 4: DC offset and RMS. 
 5: Crest factor (peak / RMS).
  6: Count zero-crossings. 
  7: Save a complete text report to a file. 
  8: You can declare the signal as “DC / AC / mixed / flat.” 
  9: Smooth samples with a moving average. 
  10: Detect peaks. 
  12: Generate a test sine wave. 
  18: From zero-crossings calculate frequency and period estimate. Maths & utility tools (13–17, 19–21). 
  13: Simple calculator (+, −, ×, ÷). 
  14: Ohm’s law (find V, I or R). 
  15: The series/parallel resistor calculator. 
  16: Quadratic equation solver (real and complex roots).
  17: Unit converter with a short explanation (V↔mV, Ω↔kΩ, Hz↔kHz, m↔mm). 
   19: Sensor helper (ADC → temperature, pressure, acceleration). 
   20: RC filter helper (fc, R or C).
    21: AI-style assistant that explains RMS, crest factor, etc. All menu routing is performed by main_menu(), get_user_input() and select_menu_item() and print results safely using printf.


### 3 Testing the program
To test in the first place, we use Menu 12 to create and run a clean sine wave. Menus 3–6, 8, 9, 10, 18 to verify min, max, peak-to-peak, RMS, crest factor, zero-crossings and frequency are all in fact accurate. Input small manual sequences (e.g. [1, -1, 1, -1]) to demonstrate that zero-crossings, smoothing, and peak detection behave as expected. Look for obviously wrong inputs (letters instead of numbers, very large numbers, empty lines) to confirm safe_get_int and safe_get_double reject the input and ask again without crashing. For the maths tools (13–17, 19–20) use simple values where you already know the answer (e.g. Ohm’s Law with V=10 V, R=5 Ω should give I=2 A) to confirm the formulas. If every option runs, then you should return reasonably sensible outputs and safely to the main menu. When you press b the program will simply return you to the menu as expected.

### 4 Submit Solution
Following the satisfaction of programming, you ought to make modifications and push them so that their marker or autograder can see your changes. To accomplish this: switch to the Source Control tab on the left-hand side of VS Code. Click + by each file to stage the files you have changed (this is usually just main.c, your README.md, and any extra files you added). Or stage all the files by adding the + at the top of the list. Type in a meaningful commit message in the text box and hit Commit. Then, hit Sync Changes and forward your updates to the remote repository on GitHub. If the green button does not change to Sync Changes you can still submit by clicking the ... above the message box and selecting Push. If an autograder is used, it will run in the background. You can check your results on the repository page. You can find the repository link from the original GitHub Classroom invite, or in VS Code, by clicking the Explorer tab, right-clicking in the blank area under your files, selecting Share, and copying the github.dev link
