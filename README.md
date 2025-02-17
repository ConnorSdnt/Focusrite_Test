# Focusrite Software Engineering Placement Test

This folder contains the technical test for Focusrite software placement applicants. It involves a partially-written C++ command line program for setting the controls on a (fake) audio interface. There will be multiple tasks which include fixing problems and adding new features.

Be sure to pay close attention to these instructions.

It is up to you what compiler/IDE/workflow you use, but please detail your choice in your submission, so we can compile what you have written.

## Submission instructions

Your submission should take the form of a repository/folder containing:

- Your code
- Any other files required to make your program work
- A README file (txt or markdown) containing instructions on how to compile and run your program, and explaining how you approached the tasks. You are not expected to write paragraphs of explanations; bullet points will suffice!

You are to send your submission by email, in the form of a link. We will accept either:

- A link to your submission hosted on a cloud storage provider (Google Drive, iCloud, Dropbox etc.)
- A link to a git repository (on GitHub, GitLab, Bitbucket, etc.) containing your submission

Please **do not** include any compiled binaries (e.g. `.exe` or `.o` files) in your submission. We will compile your code ourselves.

## How to complete the tasks

These tasks are designed to help us understand your skillset and abilities, including what you have learnt so far during your studies. There may be some concepts in there that you have not seen before - but that's ok, do your best!

While working on these tasks, feel free to use any resources you like to help complete the tasks, but you must suitably credit or reference these in your code (in whatever way you wish). Bear in mind that you may be asked to explain the code, and to talk about the process you took in completing the tasks.

For the code itself, you may use the STL (C++20) or the C stdlib but no other additional libraries. For input/output at runtime, you should only use the standard input/output streams (i.e. `std::cin`/`std::cout`).

## The tasks

1. Find and fix any problems.
2. Add the ability to set/get "phantom power" on the device. This is a control which can either be toggled on or off.
    - The user should be able to turn it on with any of these commands:
      - `set-phantom-power on`
      - `set-phantom-power 1`
    - ...and off with any of these commands:
      - `set-phantom-power off`
      - `set-phantom-power 0`
    - any other value, e.g. `set-phantom-power kittens`, should be rejected (sorry kittens).
    - The user should then be able to verify the new state of the phantom power control by using the pre-existing command, `status`.
3. Make it so the program rejects setting the preamp level to a value outside of the range -127 (minus infinity) and 0 (unity gain). It should not set the preamp level to anything in this scenario, and it should print a suitable error message to the user instead.

**Good luck!**