# Focusrite Software Engineering Placement Test
## Connor Bethel – 19/02/2025
## Github Repository containing my code: https://github.com/ConnorSdnt/Focusrite_Test

### Requirements

- C++20 Compatible compiler (g++, clang++)
- Command-line interface (Linux, MacOS, Windows)

### Compilation

I used CLion IDE to develop and test my code, however I also tested that it compiled and ran using terminal commands. 

To compile the code, use the following command:

*g++ -std=c++20 -o program_name main.cpp*

### Execution

To execute the program, use the following command/s

Main program: *./program_name*

Run tests: *./program_name –test*

### Usage

Upon entering the program, you may use the following commands:

*set-phantom-power on*	:       Turns on phantom power

*set-phantom-power 1*	:       Turns on phantom power

*set-phantom-power off*	:       Turns off phantom power

*set-phantom-power 0*	:       Turns off phantom power

*set-preamp-level <value>* :       Sets the preamp level [-127… 0]

*status*			:       Displays the current preferences

~~*set-phantom-power kittens*~~ : [[REDACTED]]

### Approach

- I started by taking the weekend to look over and understand the code. As I didn't have access to my IDE during this time

- Fixed a typo in *testMessageGenerator()*. Program was setting preamp level to -6 but checking for level -66. I believe this was just a typo.
- Implemented phantom power toggle on/off. Utilised the pre-made function *findValueString()* with minor adjustments to prevent system failure from an empty command. I.e “set-preamp-level” instead of “set-preamp-level 12”

- Implemented the bounding for set-preamp-level to [-127… 0] (inclusive). Utilised the static consts: MINUS_INFINITY_DB, and UNITY_GAIN_DB to prevent “magic numbers” within the code and improve readability. 

- Fixed the improper input handling for the *set-preamp-level* command. More specifically, cases when: the user provides something other than an integer value at the end of their command, and when the user enters a number that is out of bounds for an integer. The user is displayed the default “*command failed*” message, then re-prompted to enter a command 

- Finally, I tidied up my variable names, tidied up my debugging statements, and fixed some clang-tidy warnings.

### References for research

These are links to websites I used to research parts of the code that I was either unfamiliar with, or needed a recap on. 

Exception handling recap: https://www.geeksforgeeks.org/exception-handling-c/

“auto” keyword: https://learn.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-170

std::stoi: https://en.cppreference.com/w/cpp/string/basic_string/stol

std::optional: https://en.cppreference.com/w/cpp/utility/optional




