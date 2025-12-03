#include <stdio.h>

void CalculateRepayment(double loan, int years, float interest_Rate, int yearNo, float amount){
    if (years <= 0) return;
    if (loan < 0) return;

    // Loan after paying yearly amount
    loan = loan - amount;

    // Add interest
    double remaining = loan + loan * (interest_Rate / 100);

    printf("Year %d: Remaining Loan = %.2lf\n", yearNo, remaining);

    // Recursive Call
    CalculateRepayment(remaining, years - 1, interest_Rate, yearNo + 1, amount);
}

int main(){
    int i = 1;
    double loan = 0;
    int years = 0;
    float interest_Rate = 0;
    char ch;
    float amount;

    printf("Please enter the amount of loan: ");
    scanf("%lf", &loan);

    printf("Please enter the interest rate: ");
    scanf("%f", &interest_Rate);

    printf("Enter the number of years: ");
    scanf("%d", &years);

    printf("Enter the yearly payment amount: ");
    scanf("%f", &amount);

    CalculateRepayment(loan, years, interest_Rate, i, amount);

    printf("\nDo you want to add additional years? (Press X to add, Y to exit): ");
    scanf(" %c", &ch);

    if (ch == 'Y' || ch == 'y')
        return 0;

    if (ch == 'X' || ch == 'x'){
        printf("Enter additional number of years: ");
        scanf("%d", &years);

        printf("Enter the payment amount again: ");
        scanf("%f", &amount);

        CalculateRepayment(loan, years, interest_Rate, i, amount);
    }

    return 0;
}