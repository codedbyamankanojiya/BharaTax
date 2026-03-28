#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Constants for tax calculation (FY 2025-26 - New Tax Regime)
#define MAX_INCOME 1000000000  // 100 Crore limit
#define MIN_INCOME 0
#define SLAB_1_LIMIT 300000   // 3 Lakhs
#define SLAB_2_LIMIT 700000   // 7 Lakhs
#define SLAB_3_LIMIT 1000000  // 10 Lakhs
#define SLAB_4_LIMIT 1200000  // 12 Lakhs
#define SLAB_5_LIMIT 1500000  // 15 Lakhs
#define CESS_RATE 0.04        // 4% Health & Education Cess
#define MAX_DEDUCTION_80C 150000  // 1.5 Lakhs under 80C
#define STANDARD_DEDUCTION 75000   // 75,000 standard deduction for salaried
#define REBATE_87A_LIMIT 700000   // 7 Lakhs limit for Section 87A rebate
#define REBATE_87A_AMOUNT 25000   // 25,000 rebate under Section 87A

// Format strings for consistent display
#define BORDER_LINE     "+-----------------------------------------------------------+\n"
#define SECTION_BORDER  "|                                                           |\n"
#define FORMAT_AMOUNT   "|  %-25s Rs. %31s |\n"
#define FORMAT_TEXT     "|  %-52s     |\n"

// Structure to store tax details
typedef struct {
    long long gross_income;    // Changed to long long for larger income amounts
    double deductions_80c;     // Section 80C deductions
    double standard_deduction; // Standard deduction for salaried
    double taxable_income;
    double base_tax;
    double cess;
    double rebate_87a;        // Section 87A rebate
    double total_tax;
    double monthly_tax;        // Added to store monthly tax
    char assessment_year[5];
    int tax_slab;             // Added to store current tax slab
    int is_salaried;          // Flag for salaried individuals
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
    
    str[totalLen] = '\0';  // Null terminate string
    
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
            float amount = validateInput("Enter amount (Rs.): ", 0, MAX_DEDUCTION_80C);
            total_deductions += amount;
            char formatted_curr[50], formatted_remaining[50];
            
            if (total_deductions > MAX_DEDUCTION_80C) {
                printf("\nMaximum deduction limit (Rs. 1,50,000) exceeded!\n");
                printf("Considering only Rs. 1,50,000 as total deduction.\n");
                total_deductions = MAX_DEDUCTION_80C;
                break;
            }
            
            formatCurrency(formatted_curr, total_deductions);
            formatCurrency(formatted_remaining, MAX_DEDUCTION_80C - total_deductions);
            printf("\nCurrent total deductions:  Rs. %s\n", formatted_curr);
            printf("Remaining deduction limit: Rs. %s\n", formatted_remaining);
        }
    } while (choice != 6 && total_deductions < MAX_DEDUCTION_80C);
    
    return total_deductions;
}

// Function to calculate tax based on income (FY 2025-26 New Tax Regime)
void calculateTax(TaxDetails *tax_info) {
    // Calculate total deductions
    double total_deductions = tax_info->deductions_80c + tax_info->standard_deduction;
    
    // Calculate taxable income
    tax_info->taxable_income = (double)tax_info->gross_income - total_deductions;
    
    // Ensure taxable income is not negative
    if (tax_info->taxable_income < 0) {
        tax_info->taxable_income = 0;
    }
    
    // Determine tax slab and calculate base tax (New Regime FY 2025-26)
    if (tax_info->taxable_income <= SLAB_1_LIMIT) {
        tax_info->base_tax = 0;
        tax_info->tax_slab = 0;
    } else if (tax_info->taxable_income <= SLAB_2_LIMIT) {
        tax_info->base_tax = (tax_info->taxable_income - SLAB_1_LIMIT) * 0.05;
        tax_info->tax_slab = 5;
    } else if (tax_info->taxable_income <= SLAB_3_LIMIT) {
        tax_info->base_tax = 15000 + (tax_info->taxable_income - SLAB_2_LIMIT) * 0.10;
        tax_info->tax_slab = 10;
    } else if (tax_info->taxable_income <= SLAB_4_LIMIT) {
        tax_info->base_tax = 55000 + (tax_info->taxable_income - SLAB_3_LIMIT) * 0.15;
        tax_info->tax_slab = 15;
    } else if (tax_info->taxable_income <= SLAB_5_LIMIT) {
        tax_info->base_tax = 95000 + (tax_info->taxable_income - SLAB_4_LIMIT) * 0.20;
        tax_info->tax_slab = 20;
    } else {
        tax_info->base_tax = 155000 + (tax_info->taxable_income - SLAB_5_LIMIT) * 0.30;
        tax_info->tax_slab = 30;
    }

    // Calculate cess
    tax_info->cess = tax_info->base_tax * CESS_RATE;
    
    // Calculate rebate under Section 87A
    tax_info->rebate_87a = 0;
    if (tax_info->base_tax > 0 && tax_info->taxable_income <= REBATE_87A_LIMIT) {
        tax_info->rebate_87a = (tax_info->base_tax < REBATE_87A_AMOUNT) ? tax_info->base_tax : REBATE_87A_AMOUNT;
    }
    
    // Calculate total tax (after rebate)
    tax_info->total_tax = tax_info->base_tax + tax_info->cess - tax_info->rebate_87a;
    
    // Ensure total tax is not negative
    if (tax_info->total_tax < 0) {
        tax_info->total_tax = 0;
    }
    
    // Calculate monthly tax
    tax_info->monthly_tax = tax_info->total_tax / 12.0;
}

// Function to display tax breakdown
void displayTaxBreakdown(TaxDetails *tax_info) {
    char formatted_amount[50];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    
    // Header Section
    printf("\n+=================== BHARATAX ===================+\n");
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
    
    formatCurrency(formatted_amount, tax_info->deductions_80c);
    printf(FORMAT_AMOUNT, "Less: 80C Deductions:", formatted_amount);
    
    formatCurrency(formatted_amount, tax_info->standard_deduction);
    printf(FORMAT_AMOUNT, "Less: Standard Deduction:", formatted_amount);
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
            sprintf(slab_info, "* No Tax (Income up to Rs. 3,00,000/-)");
            break;
        case 5:
            sprintf(slab_info, "* 5%% Tax (Income Rs. 3,00,001 to Rs. 7,00,000/-)");
            break;
        case 10:
            sprintf(slab_info, "* 10%% Tax (Income Rs. 7,00,001 to Rs. 10,00,000/-)");
            break;
        case 15:
            sprintf(slab_info, "* 15%% Tax (Income Rs. 10,00,001 to Rs. 12,00,000/-)");
            break;
        case 20:
            sprintf(slab_info, "* 20%% Tax (Income Rs. 12,00,001 to Rs. 15,00,000/-)");
            break;
        case 30:
            sprintf(slab_info, "* 30%% Tax (Income above Rs. 15,00,000/-)");
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
    
    if (tax_info->rebate_87a > 0) {
        formatCurrency(formatted_amount, tax_info->rebate_87a);
        printf(FORMAT_AMOUNT, "Less: Section 87A Rebate:", formatted_amount);
    }
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
        if (tax_info->deductions_80c < MAX_DEDUCTION_80C) {
            double remaining = MAX_DEDUCTION_80C - tax_info->deductions_80c;
            formatCurrency(formatted_amount, remaining);
            printf("|  * You can still claim additional deductions of Rs. %s  |\n", formatted_amount);
            printf("|    under Section 80C to reduce your tax liability.        |\n");
        }
        if (tax_info->rebate_87a > 0) {
            formatCurrency(formatted_amount, tax_info->rebate_87a);
            printf("|  * You received Section 87A rebate of Rs. %s          |\n", formatted_amount);
        }
        printf("|  * Consider investments in PPF, ELSS, or Life Insurance    |\n");
        printf("|    for tax benefits in next financial year.            |\n");
    }
    
    printf(BORDER_LINE);
}

int main() {
    TaxDetails tax_info;
    char choice;

    do {
        system("cls");  // Clear screen
        printf("+=============== BHARATAX 2025-26 ===============+\n");
        printf("|                                                           |\n");
        printf("|  Welcome to BharaTax - Indian Income Tax Calculator     |\n");
        printf("|  This calculator helps you estimate your income tax       |\n");
        printf("|  liability for financial year 2025-26 (New Regime)    |\n");
        printf("|                                                           |\n");
        printf(BORDER_LINE);
        
        // Get current assessment year
        getCurrentAssessmentYear(tax_info.assessment_year);
        
        // Get income details
        tax_info.gross_income = validateInput("Enter your annual income (in Rs.): ", MIN_INCOME, MAX_INCOME);
        
        // Initialize deductions
        tax_info.deductions_80c = 0;
        tax_info.standard_deduction = STANDARD_DEDUCTION; // Apply standard deduction for salaried
        tax_info.is_salaried = 1; // Assume salaried by default
        
        // Calculate deductions if income > SLAB_1_LIMIT
        if (tax_info.gross_income > SLAB_1_LIMIT) {
            printf("\nWould you like to claim deductions under Section 80C? (y/n): ");
            scanf(" %c", &choice);
            clearInputBuffer();
            
            if (tolower(choice) == 'y') {
                tax_info.deductions_80c = calculateDeductions();
            } else {
                tax_info.deductions_80c = 0;
            }
        } else {
            tax_info.deductions_80c = 0;
            tax_info.standard_deduction = 0; // No standard deduction if no tax
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
            
            sprintf(filename, "bharatax_calculation_%02d%02d%d_%02d%02d.txt", 
                    tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
                    tm->tm_hour, tm->tm_min);
            
            FILE *fp = fopen(filename, "w");
            if (fp != NULL) {
                char formatted_amount[50];
                
                fprintf(fp, "╔══════════════ BHARATAX TAX REPORT ════════════════╗\n");
                fprintf(fp, "║                                                        ║\n");
                fprintf(fp, "║  Date: %02d/%02d/%d                                    ║\n", 
                        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
                fprintf(fp, "║  Assessment Year: %s-%d                                ║\n", 
                        tax_info.assessment_year, atoi(tax_info.assessment_year) + 1);
                fprintf(fp, "╠══════════════════════════════════════════════════════╣\n");
                
                formatCurrency(formatted_amount, tax_info.gross_income);
                fprintf(fp, "║  INCOME DETAILS                                        ║\n");
                fprintf(fp, "║  Annual Income:         Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.deductions_80c);
                fprintf(fp, "║  80C Deductions:       Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.standard_deduction);
                fprintf(fp, "║  Standard Deduction:    Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.taxable_income);
                fprintf(fp, "║  Taxable Income:        Rs. %-28s  ║\n", formatted_amount);
                fprintf(fp, "╠══════════════════════════════════════════════════════╣\n");
                
                fprintf(fp, "║  TAX CALCULATION                                       ║\n");
                formatCurrency(formatted_amount, tax_info.base_tax);
                fprintf(fp, "║  Base Tax:              Rs. %-28s  ║\n", formatted_amount);
                
                formatCurrency(formatted_amount, tax_info.cess);
                fprintf(fp, "║  Health & Edu. Cess:    Rs. %-28s  ║\n", formatted_amount);
                
                if (tax_info.rebate_87a > 0) {
                    formatCurrency(formatted_amount, tax_info.rebate_87a);
                    fprintf(fp, "║  Section 87A Rebate:   Rs. %-28s  ║\n", formatted_amount);
                    fprintf(fp, "╠══════════════════════════════════════════════════════╣\n");
                    
                    formatCurrency(formatted_amount, tax_info.total_tax);
                    fprintf(fp, "║  TOTAL TAX PAYABLE:     Rs. %-28s  ║\n", formatted_amount);
                } else {
                    fprintf(fp, "╠══════════════════════════════════════════════════════╣\n");
                    
                    formatCurrency(formatted_amount, tax_info.total_tax);
                    fprintf(fp, "║  TOTAL TAX PAYABLE:     Rs. %-28s  ║\n", formatted_amount);
                }
                
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

    printf("\nThank you for using BharaTax!\n");
    return 0;
}
