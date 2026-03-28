# BharaTax - Indian Income Tax Calculator 2026

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language](https://img.shields.io/badge/Language-C-blue)
![Status](https://img.shields.io/badge/Status-Active-brightgreen)

BharaTax - A comprehensive command-line application for calculating Indian income tax liability with support for deductions, tax slabs, and detailed tax breakdowns based on the new tax regime for FY 2026.

## Features

- **Accurate Tax Calculation**: Implements the new Indian income tax slab system for FY 2026
- **Deduction Support**: Calculate tax savings with Section 80C deductions (EPF, PPF, Life Insurance, ELSS, etc.)
- **Health & Education Cess**: Automatically applies 4% cess to calculated tax amount
- **Indian Currency Formatting**: Displays amounts in proper Indian numbering system (e.g., 25,00,000)
- **Standard Deduction**: Automatically applies ₹75,000 standard deduction for salaried individuals
- **Section 87A Rebate**: Automatically calculates rebate for income up to ₹7,00,000
- **Monthly Tax Breakdown**: Calculates monthly tax installments for convenient planning
- **Tax Saving Tips**: Provides recommendations for maximizing tax deductions
- **Report Generation**: Save detailed calculation reports to text files with formatted output

## Tax Slabs (FY 2026 - New Tax Regime)

| Income Range              | Tax Rate |
|---------------------------|----------|
| Up to ₹3,00,000           | Nil      |
| ₹3,00,001 - ₹7,00,000     | 5%       |
| ₹7,00,001 - ₹10,00,000    | 10%      |
| ₹10,00,001 - ₹12,00,000   | 15%      |
| ₹12,00,001 - ₹15,00,000   | 20%      |
| Above ₹15,00,000          | 30%      |

*Plus 4% Health & Education Cess on calculated tax amount*
*Standard deduction of ₹75,000 available for salaried individuals*
*Section 87A rebate of up to ₹25,000 for taxable income up to ₹7,00,000*

## Getting Started

### Prerequisites

- GCC compiler (or any C compiler compatible with C99 standard)
- Windows, Linux, or macOS system

### Compilation

```bash
gcc bharaTax.c -o bharaTax -std=c99
```

### Usage

```bash
./bharaTax
```

The program is fully interactive. Follow the on-screen prompts to:
1. Enter your annual gross income
2. Claim deductions under Section 80C (optional)
3. View detailed tax calculation and breakdown including standard deduction and Section 87A rebate
4. Save report (optional)
5. Perform additional calculations

## Project Structure

```
BharaTax/
├── bharaTax.c             # Main application source code
├── LICENSE                # MIT License
└── README.md              # Project documentation
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

Maximum deduction limit: ₹1,50,000 under Section 80C
Standard deduction: ₹75,000 for salaried individuals

### 3. Tax Calculation
The application calculates:
- **Taxable Income**: Gross Income - (80C Deductions + Standard Deduction)
- **Base Tax**: Applied according to applicable tax slab
- **Section 87A Rebate**: Up to ₹25,000 for income ≤ ₹7,00,000
- **Health & Education Cess**: 4% of calculated tax
- **Total Tax**: Base Tax + Cess - Section 87A Rebate
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
- `calculateTax()`: Accurate tax computation based on new income slabs
- `formatCurrency()`: Indian numbering system implementation
- `displayTaxBreakdown()`: Formatted output display with rebate information
- `getCurrentAssessmentYear()`: Dynamic year calculation

### Data Structure

```c
typedef struct {
    long long gross_income;
    double deductions_80c;     // Section 80C deductions
    double standard_deduction; // Standard deduction for salaried
    double taxable_income;
    double base_tax;
    double cess;
    double rebate_87a;        // Section 87A rebate
    double total_tax;
    double monthly_tax;
    char assessment_year[5];
    int tax_slab;
    int is_salaried;          // Flag for salaried individuals
} TaxDetails;
```

## Example Output

```
+===================      BHARATAX      ===================+
|  Assessment Year: 2026-2027                              |
|  Date: 09/01/2026                                        |
+----------------------------------------------------------+
|                     INCOME DETAILS                       |
|  Yearly Income:             Rs.             25,00,000    |
|  Less: 80C Deductions:      Rs.              1,50,000    |
|  Less: Standard Deduction:  Rs.                75,000    |
|  Net Taxable Income:        Rs.             23,50,000    |
+----------------------------------------------------------+
|                    TAX SLAB APPLICABLE                   |
|  * 10% Tax (Income Rs. 7,00,001 to Rs. 10,00,000/-)      |
+----------------------------------------------------------+
|                      TAX CALCULATION                     |
|  Income Tax Amount:         Rs.             4,70,000     |
|  Add: Health & Edu Cess:    Rs.               18,800     |
|  Less: Section 87A Rebate:  Rs.               25,000     |
|  TOTAL TAX PAYABLE:         Rs.             4,63,800     |
+----------------------------------------------------------+
|  Monthly Tax Installment:   Rs.               38,650     |
+----------------------------------------------------------+
```

## Disclaimer

This calculator is designed for educational and informational purposes. It implements the new Indian income tax slab system for FY 2025-26 but may not account for:
- Special deductions for specific categories
- Rebates under various sections (except Section 87A which is included)
- Surcharges applicable in specific cases
- Recent amendments to tax laws

**For official tax assessments, always consult a qualified tax professional or visit the Income Tax Department official website.**

## Technical Details

- **Language**: C (C99 standard)
- **Platform**: Cross-platform (Windows, Linux, macOS)
- **Tax Regime**: New Tax Regime FY 2025-26
- **Input Limits**: 
  - Maximum Income: ₹100 crores
  - Minimum Income: ₹0
  - Maximum 80C Deduction: ₹1,50,000
  - Standard Deduction: ₹75,000 (salaried)
  - Section 87A Rebate: Up to ₹25,000 (income ≤ ₹7,00,000)
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
Aman Kanojiya
https://codedbyamankanojiya.vercel.app


Created as a practical tool for accurate income tax calculation based on Indian tax regulations.

## Changelog

### Version 2.0.0 (Current)
- Updated to BharaTax branding
- Full tax slab implementation for FY 2026 new regime
- Section 80C deduction support
- Standard deduction for salaried individuals
- Section 87A rebate implementation
- Enhanced report generation with rebate details
- Comprehensive input validation

## Support

For questions, issues, or suggestions, please open an issue on the repository.
- GitHub: [@codedbyamankanojiya](https://github.com/codedbyamankanojiya)

**Note**: Keep this calculator updated with latest tax slabs as they change annually. The current implementation is based on FY 2026 new tax regime rates and should be reviewed for subsequent financial years.
