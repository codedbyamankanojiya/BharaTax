# Income Tax Calculator 2025-26

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language](https://img.shields.io/badge/Language-C-blue)
![Status](https://img.shields.io/badge/Status-Active-brightgreen)

A comprehensive command-line application for calculating Indian income tax liability with support for deductions, tax slabs, and detailed tax breakdowns.

## Features

- **Accurate Tax Calculation**: Implements the official Indian income tax slab system for FY 2024-25
- **Deduction Support**: Calculate tax savings with Section 80C deductions (EPF, PPF, Life Insurance, ELSS, etc.)
- **Health & Education Cess**: Automatically applies 4% cess to calculated tax amount
- **Indian Currency Formatting**: Displays amounts in proper Indian numbering system (e.g., 25,00,000)
- **Monthly Tax Breakdown**: Calculates monthly tax installments for convenient planning
- **Tax Saving Tips**: Provides recommendations for maximizing tax deductions
- **Report Generation**: Save detailed calculation reports to text files with formatted output
- **Input Validation**: Robust error handling with user-friendly prompts
- **Dynamic Assessment Year**: Automatically calculates the current financial year

## Tax Slabs (FY 2024-25)

| Income Range              | Tax Rate |
|---                        |---       |
| Up to ₹2,50,000           | Nil      |
| ₹2,50,001 - ₹5,00,000     | 5%       |
| ₹5,00,001 - ₹10,00,000    | 20%      |
| Above ₹10,00,000          | 30%      |

*Plus 4% Health & Education Cess on the calculated tax amount*

## Getting Started

### Prerequisites

- GCC compiler (or any C compiler compatible with C99 standard)
- Windows, Linux, or macOS system

### Compilation

```bash
gcc incometax.c -o income_tax_calculator
```

### Usage

```bash
./income_tax_calculator
```

The program is fully interactive. Follow the on-screen prompts to:
1. Enter your annual gross income
2. Claim deductions under Section 80C (optional)
3. View detailed tax calculation and breakdown
4. Save the report (optional)
5. Perform additional calculations

## Project Structure

```
Income-Tax-Calculator/
├── incometax.c          # Main application source code
├── LICENSE              # MIT License
└── README.md           # Project documentation
```

## How It Works

### 1. Income Input
The calculator accepts annual income amounts from ₹0 to ₹100 crores with comprehensive validation.

### 2. Deductions Calculation
Users can claim eligible deductions under Section 80C:
- Employee Provident Fund (EPF)
- Public Provident Fund (PPF)
- Life Insurance Premium
- Equity Linked Savings Scheme (ELSS)
- Other eligible investments

Maximum deduction limit: ₹1,50,000

### 3. Tax Calculation
The application calculates:
- **Taxable Income**: Gross Income - Deductions
- **Base Tax**: Applied according to applicable tax slab
- **Health & Education Cess**: 4% of calculated tax
- **Total Tax**: Base Tax + Cess
- **Monthly Installment**: Total Tax ÷ 12

### 4. Report Generation
Detailed calculation reports can be saved with formatted layout including:
- Income details
- Applicable tax slab
- Complete tax calculation breakdown
- Monthly tax liability

## Code Highlights

### Key Functions

- `validateInput()`: Robust input validation with error handling
- `calculateDeductions()`: Interactive deduction selection and limit management
- `calculateTax()`: Accurate tax computation based on income slabs
- `formatCurrency()`: Indian numbering system implementation
- `displayTaxBreakdown()`: Formatted output display
- `getCurrentAssessmentYear()`: Dynamic year calculation

### Data Structure

```c
typedef struct {
    long long gross_income;
    double deductions;
    double taxable_income;
    double base_tax;
    double cess;
    double total_tax;
    double monthly_tax;
    char assessment_year[5];
    int tax_slab;
} TaxDetails;
```

## Example Output

```
+=================== INCOME TAX CALCULATOR ===================+
|  Assessment Year: 2025-2026                              |
|  Date: 09/01/2026                                       |
+-----------------------------------------------------------+
|                     INCOME DETAILS                        |
|  Yearly Income:             Rs.             25,00,000   |
|  Less: Total Deductions:    Rs.            1,50,000    |
|  Net Taxable Income:        Rs.            23,50,000   |
+-----------------------------------------------------------+
|                    TAX SLAB APPLICABLE                    |
|  * 20% Tax (Income Rs. 5,00,001 to Rs. 10,00,000/-)    |
+-----------------------------------------------------------+
|                      TAX CALCULATION                     |
|  Income Tax Amount:         Rs.             4,70,000   |
|  Add: Health & Edu Cess:    Rs.               18,800   |
|  TOTAL TAX PAYABLE:         Rs.             4,88,800   |
+-----------------------------------------------------------+
|  Monthly Tax Installment:   Rs.               40,733   |
+-----------------------------------------------------------+
```

## Disclaimer

This calculator is designed for educational and informational purposes. It implements the standard Indian income tax slab system but may not account for:
- Special deductions for specific categories
- Rebates under various sections
- Surcharges applicable in specific cases
- Recent amendments to tax laws

**For official tax assessments, always consult a qualified tax professional or visit the Income Tax Department official website.**

## Technical Details

- **Language**: C (C99 standard)
- **Platform**: Cross-platform (Windows, Linux, macOS)
- **Input Limits**: 
  - Maximum Income: ₹100 crores
  - Minimum Income: ₹0
  - Maximum Deduction: ₹1,50,000
- **Precision**: Double precision floating-point for accurate calculations

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request with a clear description of your modifications

## Author

Created as a practical tool for accurate income tax calculation based on Indian tax regulations.

## Changelog

### Version 1.0.0 (Current)
- Initial release
- Full tax slab implementation for FY 2024-25
- Section 80C deduction support
- Report generation feature
- Indian currency formatting
- Comprehensive input validation

## Support

For questions, issues, or suggestions, please open an issue on the repository.

---

**Note**: Keep this calculator updated with the latest tax slabs as they change annually. The current implementation is based on FY 2024-25 rates and should be reviewed for subsequent financial years.

