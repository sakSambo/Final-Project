#include <stdio.h>  // Standard input-output library for printf, scanf, etc.
#include <stdlib.h> // Standard library for system functions like memory allocation

#include "zener.h"  // Custom header file that contains function declarations

void userParameters() {
    double Vin, Vz, Rs, RL, Prs, Pz_max; // Circuit parameters
    int result; // Variable to store input validation result

    // Infinite loop to ensure valid input is received before proceeding
    while (1) {
        // Prompting user for circuit parameters
        printf("Enter supply voltage (Vin) in volts: ");
        result = scanf("%lf", &Vin);
        if (result != 1 || Vin < 0) { // Validate input
            printf("\nERROR: Invalid input! Supply voltage (Vin) must be a non-negative number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue; // Restart loop
        }

        printf("Enter Zener breakdown voltage (Vz) in volts: ");
        result = scanf("%lf", &Vz);
        if (result != 1 || Vz < 0) {
            printf("\nERROR: Invalid input! Zener breakdown voltage (Vz) must be a non-negative number.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter series resistance (Rs) in ohms: ");
        result = scanf("%lf", &Rs);
        if (result != 1 || Rs < 0) {
            printf("\nERROR: Invalid input! Series resistance (Rs) must be a non-negative number.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter series resistor power rating (Prs) in watts: ");
        result = scanf("%lf", &Prs);
        if (result != 1 || Prs < 0) {
            printf("\nERROR: Invalid input! Series resistor power rating must be a non-negative number.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter load resistance (RL) in ohms: ");
        result = scanf("%lf", &RL);
        if (result != 1 || RL < 0) {
            printf("\nERROR: Invalid input! Load resistance (RL) must be a non-negative number.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("Enter Zener diode power rating (Pz_max) in watts: ");
        result = scanf("%lf", &Pz_max);
        if (result != 1 || Pz_max < 0) {
            printf("\nERROR: Invalid input! Zener diode power rating must be a non-negative number.\n");
            while (getchar() != '\n');
            continue;
        }

        // Call function to calculate and display circuit parameters
        calculateZenerParameters(Vin, Vz, Rs, RL, Prs, Pz_max);
        break; // Exit loop after successful input
    }
}

void calculateZenerParameters(double Vin, double Vz, double Rs, double RL, double Prs, double Pz_max) {
    double Vout, Iout, Pz, Irs, Prs_dissipated; // Calculated parameters
    FILE *file = fopen("zener_results.txt", "a"); // Open file in append mode

    if (!file) { // Check if file opened successfully
        printf("ERROR: Could not open file for writing.\n");
        return;
    }

    // Check if the Zener diode is in breakdown mode
    if (Vin < Vz) {
        printf("\n====================================================\n");
        printf("WARNING: The Zener diode is NOT in breakdown mode.\n");
        printf("Output Voltage (Vout): %.2f V (equals supply voltage as no regulation is occurring)\n", Vin);
        printf("====================================================\n\n");

        // Log warning to file
        fprintf(file, "WARNING: The Zener diode is NOT in breakdown mode.\n");
        fprintf(file, "Output Voltage (Vout): %.2f V (equals supply voltage)\n\n", Vin); //after printing, there's nth to do so it exit prematurely
    } else {
        // Perform calculations
        Vout = Vz;                    // Output voltage remains at Zener voltage
        Iout = Vout / RL;              // Load current calculation (Ohm's Law)
        Irs = (Vin - Vout) / Rs;       // Current through series resistor
        Prs_dissipated = (Irs * Irs) * Rs; // Power dissipated in series resistor
        Pz = Vout * (Irs - Iout);      // Power dissipated in the Zener diode

        // Print and log warnings based on safety limits
        if (Irs > 1.0) {
            printf("\nWARNING: Excessive current through series resistor!\n");
            fprintf(file, "WARNING: Excessive current through series resistor!\n");
        }
        if (Iout > (100.0 / RL)) {
            printf("\nWARNING: High load current detected!\n");
            fprintf(file, "WARNING: High load current detected!\n");
        }
        if (Prs_dissipated > Prs) {
            printf("\nWARNING: Excessive power dissipation in Series Resistor!\n");
            fprintf(file, "WARNING: Excessive power dissipation in Series Resistor!\n");
        }
        if (Pz > Pz_max) {
            printf("\nWARNING: High power dissipation in Zener diode!\n");
            fprintf(file, "WARNING: High power dissipation in Zener diode!\n");
        }

        // Display calculated results
        printf("\n=== Zener Diode Voltage Regulator Results ===\n");
        printf("Output Voltage (Vout): %.2f V\n", Vout);
        printf("Load Current (Iout): %.2f mA\n", Iout * 1000);
        printf("Current Through Series Resistor (Irs): %.2f mA\n", Irs * 1000);
        printf("Power Dissipated by Series Resistor (Prs): %.2f mW\n", Prs_dissipated * 1000);
        printf("Power Dissipated by Zener Diode (Pz): %.2f mW\n", Pz * 1000);

        // Log results to file
        fprintf(file, "\n=== Zener Diode Voltage Regulator Results ===\n");
        fprintf(file, "Output Voltage (Vout): %.2f V\n", Vout);
        fprintf(file, "Load Current (Iout): %.2f mA\n", Iout * 1000);
        fprintf(file, "Current Through Series Resistor (Irs): %.2f mA\n", Irs * 1000);
        fprintf(file, "Power Dissipated by Series Resistor (Prs): %.2f mW\n", Prs_dissipated * 1000);
        fprintf(file, "Power Dissipated by Zener Diode (Pz): %.2f mW\n\n", Pz * 1000);
    }

    fclose(file); // Close file after writing
}