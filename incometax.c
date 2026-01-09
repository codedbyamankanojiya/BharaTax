#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Constants for tax calculation
#define MAX_INCOME 1000000000  // 100 Crore limit
#define MIN_INCOME 0
#define SLAB_1_LIMIT 250000   // 2.5 Lakhs
#define SLAB_2_LIMIT 500000   // 5 Lakhs
#define SLAB_3_LIMIT 1000000  // 10 Lakhs
#define CESS_RATE 0.04        // 4% Health & Education Cess
#define MAX_DEDUCTION 150000  // 1.5 Lakhs under 80C

// Format strings for consistent display
#define BORDER_LINE     "+-----------------------------------------------------------+\n"
#define SECTION_BORDER  "|                                                           |\n"
#define FORMAT_AMOUNT   "|  %-25s Rs. %31s |\n"
#define FORMAT_TEXT     "|  %-52s     |\n"

// Structure to store tax details
typedef struct {
    long long gross_income;    // Changed to long long for larger income amounts
    double deductions;         // Changed to double for better precision
    double taxable_income;
    double base_tax;
    double cess;
    double total_tax;
    double monthly_tax;        // Added to store monthly tax
    char assessment_year[5];
    int tax_slab;             // Added to store current tax slab
} TaxDetails;

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to format currency in Indian format (in Rupees)
void formatCurrency(char *str, double amount) {
    long long rupees = (long long)amount;
    
    // Handle zero amount
    if (rupees == 0) {
        strcpy(str, "0");
        return;
    }
    
    // Convert to string
    char numStr[30];
    sprintf(numStr, "%lld", rupees);
    int len = strlen(numStr);
    
    // Calculate number of commas needed
    int commaCount = (len - 1) / 2;
    if (len > 3) commaCount = 1 + (len - 3) / 2;
    
    // Calculate total length including commas
    int totalLen = len + commaCount;
    
    // Place digits and commas
    int numIdx = len - 1;  // Index for reading digits
    int formatIdx = totalLen - 1;  // Index for placing digits and commas
    int digitCount = 0;  // Count digits after last comma
    
    str[totalLen] = '\0';  // Null terminate the string
    
    while (numIdx >= 0) {
        if (digitCount == 2 && numIdx > 0) {  // Place comma after every 2 digits
            str[formatIdx] = ',';
            formatIdx--;
            digitCount = 0;
        } else if (digitCount == 3 && numIdx == len - 4) {  // First comma after 3 digits
            str[formatIdx] = ',';
            formatIdx--;
            digitCount = 0;
        }
        
        str[formatIdx] = numStr[numIdx];
        formatIdx--;
        numIdx--;
        digitCount++;
    }
}

// Function to get current assessment year
void getCurrentAssessmentYear(char *year) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int current_year = tm->tm_year + 1900;
    int current_month = tm->tm_mon + 1;
    
    // If current date is after March, assessment year is next year
    if (current_month > 3) {
        sprintf(year, "%d", current_year + 1);
    } else {
        sprintf(year, "%d", current_year);
    }
}

// Function to validate numeric input with error message
int validateInput(const char *prompt, int min, int max) {
    char input[50];
    char *endptr;
    long value;
    int valid = 0;

    do {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        // Check for empty input
        if (strlen(input) == 0) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }

        // Check if input is numeric
        valid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                valid = 0;
                printf("Please enter numbers only.\n");
                break;
            }
        }
        if (!valid) continue;

        value = strtol(input, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid input format. Please enter a valid number.\n");
            valid = 0;
            continue;
        }

        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            valid = 0;
            continue;
        }

        break;
    } while (1);

    return (int)value;
}

// Function to calculate deductions
float calculateDeductions() {
    float total_deductions = 0;
    int choice;
    
    printf("\n+----------------- DEDUCTIONS SECTION 80C ------------------+\n");
    printf("|  1. EPF (Employee Provident Fund)                         |\n");
    printf("|  2. PPF (Public Provident Fund)                          |\n");
    printf("|  3. Life Insurance Premium                               |\n");
    printf("|  4. ELSS (Equity Linked Savings Scheme)                  |\n");
    printf("|  5. Other eligible investments                           |\n");
    printf("|  6. Skip deductions                                      |\n");
    printf("+-----------------------------------------------------------+\n");
    
    do {
        choice = validateInput("Enter choice (1-6): ", 1, 6);
        
        if (choice != 6) {
            float amount = validateInput("Enter amount (Rs.): ", 0, MAX_DEDUCTION);
            total_deductions += amount;
            char formatted_curr[50], formatted_remaining[50];
            
            if (total_deductions > MAX_DEDUCTION) {
                printf("\nMaximum deduction limit (Rs. 1,50,000) exceeded!\n");
                printf("Considering only Rs. 1,50,000 as total deduction.\n");
                total_deductions = MAX_DEDUCTION;
                break;
            }
            
            formatCurrency(formatted_curr, total_deductions);
            formatCurrency(formatted_remaining, MAX_DEDUCTION - total_deductions);
            printf("\nCurrent total deductions:  Rs. %s\n", formatted_curr);
            printf("Remaining deduction limit: Rs. %s\n", formatted_remaining);
        }
    } while (choice != 6 && total_deductions < MAX_DEDUCTION);
    
    return total_deductions;
}

// Function to calculate tax based on income
void calculateTax(TaxDetails *tax_info) {
    // Calculate taxable income
    tax_info->taxable_income = (double)tax_info->gross_income - tax_info->deductions;
    
    // Determine tax slab and calculate base tax
    if (tax_info->taxable_income <= SLAB_1_LIMIT) {
        tax_info->base_tax = 0;
        tax_info->tax_slab = 0;
    } else if (tax_info->taxable_income <= SLAB_2_LIMIT) {
        tax_info->base_tax = (tax_info->taxable_income - SLAB_1_LIMIT) * 0.05;
        tax_info->tax_slab = 5;
    } else if (tax_info->taxable_income <= SLAB_3_LIMIT) {
        tax_info->base_tax = 12500 + (tax_info->taxable_income - SLAB_2_LIMIT) * 0.20;
        tax_info->tax_slab = 20;
    } else {
        tax_info->base_tax = 112500 + (tax_info->taxable_income - SLAB_3_LIMIT) * 0.30;
        tax_info->tax_slab = 30;
    }

    // Calculate cess
    tax_info->cess = tax_info->base_tax * CESS_RATE;
    
    // Calculate total and monthly tax
    tax_info->total_tax = tax_info->base_tax + tax_info->cess;
    tax_info->monthly_tax = tax_info->total_tax / 12.0;
}

// Function to display tax breakdown
void displayTaxBreakdown(TaxDetails *tax_info) {
    char formatted_amount[50];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    
    // Header Section
    printf("\n+=================== INCOME TAX CALCULATOR ===================+\n");
    printf(SECTION_BORDER);
    printf("|  Assessment Year: %s-%d                              |\n", 
           tax_info->assessment_year, atoi(tax_info->assessment_year) + 1);
    printf("|  Date: %02d/%02d/%d                                       |\n", 
           tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    printf(BORDER_LINE);
    
    // Income Details Section
    printf("|                     INCOME DETAILS                         |\n");
    printf(SECTION_BORDER);
    
    formatCurrency(formatted_amount, tax_info->gross_income);
    printf(FORMAT_AMOUNT, "Yearly Income:", formatted_amount);
    
    formatCurrency(formatted_amount, tax_info->deductions);
    printf(FORMAT_AMOUNT, "Less: Total Deductions:", formatted_amount);
    printf("|                          -------------------------------   |\n");
    
    formatCurrency(formatted_amount, tax_info->taxable_income);
    printf(FORMAT_AMOUNT, "Net Taxable Income:", formatted_amount);
    printf(BORDER_LINE);
    
    // Tax Slab Section
    printf("|                    TAX SLAB APPLICABLE                      |\n");
    printf(SECTION_BORDER);
    
    char slab_info[100];
    switch(tax_info->tax_slab) {
        case 0:
            sprintf(slab_info, "* No Tax (Income up to Rs. 2,50,000/-)");
            break;
        case 5:
            sprintf(slab_info, "* 5%% Tax (Income Rs. 2,50,001 to Rs. 5,00,000/-)");
            break;
        case 20:
            sprintf(slab_info, "* 20%% Tax (Income Rs. 5,00,001 to Rs. 10,00,000/-)");
            break;
        case 30:
            sprintf(slab_info, "* 30%% Tax (Income above Rs. 10,00,000/-)");
            break;
    }
    printf(FORMAT_TEXT, slab_info);
    printf(BORDER_LINE);
    
    // Tax Calculation Section
    printf("|                      TAX CALCULATION                       |\n");
    printf(SECTION_BORDER);
    
    formatCurrency(formatted_amount, tax_info->base_tax);
    printf(FORMAT_AMOUNT, "Income Tax Amount:", formatted_amount);
    
    formatCurrency(formatted_amount, tax_info->cess);
    printf(FORMAT_AMOUNT, "Add: Health & Edu Cess:", formatted_amount);
    printf("|                           -------------------------------   |\n");
    
    formatCurrency(formatted_amount, tax_info->total_tax);
    printf(FORMAT_AMOUNT, "TOTAL TAX PAYABLE:", formatted_amount);
    printf(BORDER_LINE);
    
    // Monthly Tax Section
    formatCurrency(formatted_amount, tax_info->monthly_tax);
    printf(FORMAT_AMOUNT, "Monthly Tax Installment:", formatted_amount);
    
    // Tax Saving Tips
    if (tax_info->total_tax > 0) {
        printf(BORDER_LINE);
        printf("|                        TAX SAVING TIPS                       |\n");
        printf(SECTION_BORDER);
        if (tax_info->deductions < MAX_DEDUCTION) {
            double remaining = MAX_DEDUCTION - tax_info->deductions;
            formatCurrency(formatted_amount, remaining);
            printf("|  * You can still claim additional deductions of Rs. %s  |\n", formatted_amount);
            printf("|    under Section 80C to reduce your tax liability.        |\n");
        }
        printf("|  * Consider investments in PPF, ELSS, or Life Insurance    |\n");
        printf("|    for tax benefits in the next financial year.            |\n");
    }
    
    printf(BORDER_LINE);
}

int main() {
    TaxDetails tax_info;
    char choice;

    do {
        system("cls");  // Clear screen
        printf("+=============== INCOME TAX CALCULATOR 2025-26 ===============+\n");
        printf("|                                                           |\n");
        printf("|  Welcome to the Income Tax Calculator                     |\n");
        printf("|  This calculator helps you estimate your income tax       |\n");
        printf("|  liability for the financial year 2024-25                |\n");
        printf("|                                                           |\n");
        printf(BORDER_LINE);
        
        // Get current assessment year
        getCurrentAssessmentYear(tax_info.assessment_year);
        
        // Get income details
        tax_info.gross_income = validateInput("Enter your annual income (in Rs.): ", MIN_INCOME, MAX_INCOME);
        
        // Calculate deductions if income > SLAB_1_LIMIT
        if (tax_info.gross_income > SLAB_1_LIMIT) {
            printf("\nWould you like to claim deductions under Section 80C? (y/n): ");
            scanf(" %c", &choice);
            clearInputBuffer();
            
            if (tolower(choice) == 'y') {
                tax_info.deductions = calculateDeductions();
            } else {
                tax_info.deductions = 0;
            }
        } else {
            tax_info.deductions = 0;
            printf("\nNo tax liability. Deductions not applicable.\n");
        }

        // Calculate tax
        calculateTax(&tax_info);
        
        // Display tax breakdown
        displayTaxBreakdown(&tax_info);

        // Save option
        printf("\nWould you like to save this calculation? (y/n): ");
        scanf(" %c", &choice);
        clearInputBuffer();
        
        if (tolower(choice) == 'y') {
            char filename[100];
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            
            sprintf(filename, "tax_calculation_%02d%02d%d_%02d%02d.txt", 
                    tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
                    tm->tm_hour, tm->tm_min);
            
            FILE *fp = fopen(filename, "w");
            if (fp != NULL) {
                char formatted_amount[50];
                
                fprintf(fp, "╔════════════════ TAX CALCULATION REPORT ════════════════╗\n");
                fprintf(fp, "║                                                        ║\n");
                fprintf(fp, "║  Date: %02d/%02d/%d                                    ║\n", 
                        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
                fprintf(fp, "║  Assessment Year: %s-%d                                ║\n", 
                        tax_info.assessment_year, atoi(tax_info.assessment_year) + 1);
                fprintf(fp, "╠════════════════════════════════════════════════════════╣\n");
                
                formatCurrency(formatted_amount, tax_info.gross_income);
                fprintf(fp, "║  INCOME DETAILS                                        ║\n");
                fprintf(fp, "║  Annual Income:         Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.deductions);
                fprintf(fp, "║  Total Deductions:      Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.taxable_income);
                fprintf(fp, "║  Taxable Income:        Rs. %-28s  ║\n", formatted_amount);
                fprintf(fp, "╠════════════════════════════════════════════════════════╣\n");
                
                fprintf(fp, "║  TAX CALCULATION                                       ║\n");
                formatCurrency(formatted_amount, tax_info.base_tax);
                fprintf(fp, "║  Base Tax:              Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.cess);
                fprintf(fp, "║  Health & Edu. Cess:    Rs. %-28s  ║\n", formatted_amount);
                fprintf(fp, "╠════════════════════════════════════════════════════════╣\n");
                
                formatCurrency(formatted_amount, tax_info.total_tax);
                fprintf(fp, "║  TOTAL TAX PAYABLE:     Rs. %-28s  ║\n", formatted_amount);
                
                float monthly_tax = tax_info.total_tax / 12;
                formatCurrency(formatted_amount, monthly_tax);
                fprintf(fp, "║  Monthly Tax Liability: Rs. %-28s  ║\n", formatted_amount);
                fprintf(fp, "╚════════════════════════════════════════════════════════╝\n");
                fclose(fp);
                printf("Calculation saved to %s\n", filename);
            } else {
                printf("Error saving calculation to file.\n");
            }
        }

        printf("\nDo you want to calculate again? (y/n): ");
        scanf(" %c", &choice);
        clearInputBuffer();

    } while (tolower(choice) == 'y');

    printf("\nThank you for using the Income Tax Calculator!\n");
    return 0;
}