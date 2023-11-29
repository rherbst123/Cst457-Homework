#include <stdio.h>

//prototype for getBMI Method
double getBMI(int weight, int height);
//prototype for output method
void printBMIResult(double bmi);

int main() {

    //we need a quit variable
    char quit = 'X';
    //while user selects anything by n or N
    while (quit != 'n' && quit != 'N') {
        
        //hello mess
        printf("Let's calculate your BMI\n");
        //what is your weight
        //weight var
        int weight = 0;
        //prompt
        printf("What is your weight (in lbs)?\n");
        //get it
        scanf("%d", &weight);
        //what is your height?
        //height var
        int height = 0;
        //prompt
        printf("What is your height (in inches)?\n");
        //get it
        scanf("%d", &height);

        //call BMI method
        double bmi = getBMI(weight, height);
        //output part 1
        printf("Given your weight: %d and your height: %d, your BMI is: %lf which makes you ", weight, height, bmi);
        //output part 2
        printBMIResult(bmi);
        //finally add a newline
        printf("\n");
        //decide whether to loop again or not
        //prompt
        printf("Do you wish to calculate your BMI?\n");
        //read in the choice
        scanf(" %c", &quit);

    }
    //good bye message
    printf("Thank you and goodbye\n");
    return 0;
}

//get BMI method
double getBMI(int weight, int height) {
    //calculate BMI
    double bmi = ((double)weight / (height * height)) * 703;
    //return it
    return bmi;
}

//output BMI value method
void printBMIResult(double bmi) {
    if (bmi < 18.5) {
        printf("underweight");
    } else if (bmi < 25) {
        printf("normal/healthy");
    } else if (bmi < 30) {
        printf("overweight");
    } else {
        printf("obsese");
    }
}