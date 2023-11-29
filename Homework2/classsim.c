#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//2D ARRAY WITH 4 EXAMS AND 100 STUDENTS REMEMBER TO CHANGE 
#define NUM_EXAMS 4
#define NUM_STUDENTS 100

int test_scores[NUM_STUDENTS][NUM_EXAMS];

// Randomly generate scores based off bouuuuuunds
int generate_random_score(int lower_bound, int upper_bound) {
    return rand() % (upper_bound - lower_bound + 1) + lower_bound;
}
//main function IMPORTANT
int main() {
    

    // Test Scores and Curves
    double curves[NUM_EXAMS];
    double exam_averages[NUM_EXAMS];

    for (int i = 0; i < NUM_EXAMS; i++) {
        int lower_bound, upper_bound;
        
        
        //MASTERS PORTION MAKE SURE YOU ASK FOR EACH TEST
        //MAKE SURE YOU SHOW AVERAGE FOR TEST THEN ASK FOR CURVE
        //Lower Bound
        printf("Enter the lower bound for exam %d: ", i + 1);
        scanf("%d", &lower_bound);
        
        //upper 
        printf("Enter the upper bound for exam %d: ", i + 1);
        scanf("%d", &upper_bound);

        // Calculate the average for the current exam after bounds have been inputed ...read the instructions this time...
        double average = 0.0;
        for (int j = 0; j < NUM_STUDENTS; j++) {
            test_scores[j][i] = generate_random_score(lower_bound, upper_bound);
            average += test_scores[j][i];
        }
        average /= NUM_STUDENTS;
        exam_averages[i] = average;

        // show average for the exam
        printf("The average for this exam %d is %f.\n", i + 1, average);
        
        
        //CALCULATE IF YOU NEED THE CURVE
        if (average < 75.0) {
            printf("Enter the curve (wow they are stupid) %d: ", i + 1);
            scanf("%lf", &curves[i]);
        } else {
            curves[i] = 0.0; // Good job No Curve!
        }
    }

    // Calculate final scores
    for (int i = 0; i < NUM_STUDENTS; i++) {
        double final_score = 0.0;
        for (int j = 0; j < NUM_EXAMS; j++) {
            final_score += test_scores[i][j] + curves[j];
        }
        final_score /= NUM_EXAMS;

        // Print calculated final scores
        printf("Student %d: AVG %f\n", i + 1, final_score);
    }
     
     //woop woop done
    return 0;
}
