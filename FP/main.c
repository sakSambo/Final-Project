#include <stdio.h>
#include <stdlib.h>

#include "zener.h"

void viewPreviousResults() {
    FILE *file = fopen("zener_results.txt", "r"); // Open the file in read mode

    if (!file) { // Check if the file exists
        printf("No previous results found.\n"); // Display message if file is missing
        return; // Exit function
    }

    printf("\n===== Previous Calculations =====\n");

    char ch;
    // Read and print the file character by character until end of file
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch); // Print each character to the console
    }
    fclose(file); // Close the file after reading
    printf("\n=================================\n");
}

int main() {
    char choice; // Variable to store user input for repeating the program

    do {
        userParameters(); // Call function to take user input and perform calculations

        // Ask the user if they want to view previous calculations
        printf("\nDo you want to view previous calculations? (y/n): ");
        while (getchar() != '\n'); // Clear the input buffer to avoid reading previous inputs
        scanf(" %c", &choice); // Read user's choice

        if (choice == 'y' || choice == 'Y') { // If user enters 'y' or 'Y'
            viewPreviousResults(); // Display previous calculations
        }

        // Ask the user if they want to perform another calculation
        printf("\nDo you want to perform another calculation? (y/n): ");
        while (getchar() != '\n'); // Clear input buffer again
        scanf(" %c", &choice); // Read user's choice

    } while (choice == 'y' || choice == 'Y'); // Loop again if user wants another calculation

    printf("Thank you for using the Zener diode calculator. Goodbye!\n");

    return 0;
}