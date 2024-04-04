#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char data[MAX_SIZE];
    int top;
} Stack;

// this function is to initialize the stack
void initialize(Stack *s) {
    s->top = -1;
}

// this function is to check if the stack is empty
int is_empty(Stack *s) {
    return (s->top == -1);
}

// Function to push an element onto the stack
void push(Stack *s, char element) {
    if (s->top == MAX_SIZE - 1) {
        printf("Stack overflow\n");
        exit(1);
    }
    s->data[++(s->top)] = element;
}


// Function to pop an element from the stack
char pop(Stack *s) {
    if (is_empty(s)) {
        printf("Stack underflow\n");
        exit(1);
    }
    return s->data[(s->top)--];
}
// Function to get the precedence of an operator
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}
/*the following function is to validate the expression
 Returns:
-1: Expression is valid
0: Missing operator
1: Missing parentheses    */  
int is_valid_expression(char *exp) {
    Stack s = { .top = -1 }; //initialize the stack 
    int operator_found = 0; // flag to track if an operator is found 
    int parentheses_open = 0; // flag to track the number of open parentheses
    char prev_token = '\0'; // initialize prev_token to null character

    while (*exp != '\0') {
        if (*exp == '(') {
            // check if  there is a missing operator between previous token and current '('
            if (prev_token != '\0' && prev_token != '(' && prev_token != '+' && prev_token != '-' && prev_token != '*' && prev_token != '/') {
                return 0; 
            }
            push(&s, *exp);
            operator_found = 0;
            parentheses_open++;
        } else if (*exp == ')') {
            if (s.top == -1 || pop(&s) != '(') {
                return 1; // missing parentheses
            }
            parentheses_open--;
        } else if (isdigit(*exp)) {
            operator_found = 0;
        } else if (*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/') {
            operator_found = 1;
        }
        prev_token = *exp;
        exp++;
    }


    if (s.top != -1 || operator_found == 1 || parentheses_open != 0) {
        if (s.top != -1 || parentheses_open != 0) {
            return 1; // will return 1 when there is a missing parentheses
        } else {
            return 0; //will return 0 when there is a missing operator
        }
    }

  
    return -1; // will return -1 when the expression is valid 
}

// Function to check if a character is an operator
int is_operator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}



//this function is to convert the valid infix expressions to a prefix expression. 
void infix_to_prefix(char *infix_exp) {

    Stack operator_stack;  // stack to store operators
    initialize(&operator_stack);

    int length = strlen(infix_exp);
    char prefix_exp[length + 1];  // array to store the reversed infix expression
    int prefix_exp_index = 0;


    // Reverse the infix expression
    for (int i = length - 1; i >= 0; i--) {
        if (infix_exp[i] == '(')
            prefix_exp[prefix_exp_index++] = ')';
        else if (infix_exp[i] == ')')
            prefix_exp[prefix_exp_index++] = '(';
        else
            prefix_exp[prefix_exp_index++] = infix_exp[i];
    }
    prefix_exp[prefix_exp_index] = '\0';


    // Convert the reversed infix expression to prefix
    prefix_exp_index = 0;

    for (int i = 0; i < length; i++) {
        // If operand, append to prefix expression
        if (isalnum(prefix_exp[i])) {
            infix_exp[prefix_exp_index++] = prefix_exp[i];
        }
        // If opening parenthesis, push to stack
         else if (prefix_exp[i] == '(') {
            push(&operator_stack, prefix_exp[i]);
        } 
        // If closing parenthesis, pop from stack until opening parenthesis is found
        else if (prefix_exp[i] == ')') {
            while (!is_empty(&operator_stack) && operator_stack.data[operator_stack.top] != '(')
                infix_exp[prefix_exp_index++] = pop(&operator_stack);
            if (!is_empty(&operator_stack) && operator_stack.data[operator_stack.top] == '(')
                pop(&operator_stack);
        } 
         // If operator, pop operators from stack until lower precedence operator or opening parenthesis is found
        else if (is_operator(prefix_exp[i])) {
        
            while (!is_empty(&operator_stack) && precedence(operator_stack.data[operator_stack.top]) > precedence(prefix_exp[i]) && operator_stack.data[operator_stack.top] != '(')
                infix_exp[prefix_exp_index++] = pop(&operator_stack);
            push(&operator_stack, prefix_exp[i]);
        }
    }

    // pop any remaining operators from the stack
    while (!is_empty(&operator_stack))
        infix_exp[prefix_exp_index++] = pop(&operator_stack);

    // reverse the prefix expression to get the final result
    for (int i = 0; i < prefix_exp_index / 2; i++) {
        char temp = infix_exp[i];
        infix_exp[i] = infix_exp[prefix_exp_index - i - 1];
        infix_exp[prefix_exp_index - i - 1] = temp;
    }

    infix_exp[prefix_exp_index] = '\0';

    // Print the prefix notation expression
    for (int i = 0; i < prefix_exp_index; i++) {
        printf("%c ", infix_exp[i]);
    }
   
    printf("\n");
}


int evaluate_prefix_expression(char *prefix_exp) {
    Stack operand_stack;  // Stack to store operands
    initialize(&operand_stack);
    int length = strlen(prefix_exp);

    // Traverse the expression from right to left
    for (int i = length - 1; i >= 0; i--) {

        if (isdigit(prefix_exp[i])) {
            // If operand, push to stack
            int operand = prefix_exp[i] - '0';  // Convert character to integer
            push(&operand_stack, operand);
        } else if (is_operator(prefix_exp[i])) {
            // If operator, pop two operands from stack and perform the operation
            int operand1 = pop(&operand_stack);
            int operand2 = pop(&operand_stack);
            int result;

            switch (prefix_exp[i]) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
                default:
                    printf("Invalid operator\n");
                    return 0;
            }

            // Push the result back to the stack
            push(&operand_stack, result);
        } else {
            printf("Invalid character in the expression\n");
            return 0;
        }
    }

    // the final result will be the top element of the stack
    int final_result = pop(&operand_stack);
    return final_result;
}

//function to print the results on an output file 
void write_equation_to_file(const char* equation, int validity, int exp_num) {
    FILE* fp = fopen("output.txt", "a");  

    fprintf(fp, "Expression %d : %s",exp_num, equation);
    fprintf(fp, "Validity: %s\n", (validity == -1) ? "Valid" : "Invalid");

    fprintf(fp, "\n");
    fclose(fp);
}

//this function is to print the menu for the user 
void menu (){

    printf("\n\n>>Menu<<\nPlease provide us the number of your choice: \n");
    printf("1. Read the file\n");
    printf("2. Check the validity of equations (either valid or not). \n");
    printf("3. Convert valid equations from infix to prefix and print the prefix expressions. \n");
    printf("4. Evaluate the prefix expressions and print the results of each equation on a separate line:\n");
    printf("5. Print invalid equations. This option should print all invalid equations from the input file.\n");
    printf("6. Print all equations to a file (output.txt)\n");
    printf("7. Exit \n");
}


int main() {
    char input_file_name[100]; // save the provided file's name 
    FILE *fp;


    char exp[100]; 
    char *e;
    int exp_num = 0;
    int choice;
    int flag_thereis_valid = 0; //flag to see if there is a valid epressions in the provided file or not. 
    int flag_thereis_invalid = 0;  //flag to see if there is an invalid epressions in the provided file or not. 
    int flag_data_isread= 0 ;//flag to check if the data is read from the file or still not 

    while (choice !=7){
    menu(); // to display the menu options 
    scanf("%d", &choice);

 if (choice == 1) {
            FILE* fp;
            while (1) { // looping until the user enters a corrct file name
                printf("Enter the name of the input file: ");
                scanf("%s", input_file_name);
                fp = fopen(input_file_name, "r");
               
                if (fp == NULL) {
                    printf("Error opening file. Please enter a correct file name.\n");
                } else {
                    printf("Data read successfully!\n"); 
                    flag_data_isread=1; 
                
                   
                    fclose(fp);  // close the file after use
                    break;  // exit the loop since a valid file name was provided
                }
            }
    }
    else if (choice == 2 ){
        if(flag_data_isread==0){//to make sure that the data is read before doing any operation. 
            printf("You should read the expressions from the file first.\n");

        }
    else {
    fp = fopen(input_file_name, "r");
    while (fgets(exp, 100, fp) != NULL) {
        exp_num++;


        printf("\nExpression %d: %s is ", exp_num, exp); 

        int result = is_valid_expression(exp); //calling the is valid function to print the validity of the expression 
        if (result == -1) { //valid
            printf("valid.\n");
        } else if (result == 0) { // missing operator 
            printf("invalid due to missing operator.\n");
        } else if (result == 1) { // missing parentheses 
            printf("invalid due to missing parentheses.\n");
        }
    }

    exp_num = 0; // after each choice, clear the expression index to recount from 0.
    fclose(fp);
    }
    }
    else if (choice == 3 ){
        
        if(flag_data_isread==0){
            printf("You should read the expressions from the file first.\n");
        }
    else { // if data is read, do the following code 
       
    fp = fopen(input_file_name, "r");
    while (fgets(exp, 100, fp) != NULL) {
        exp_num++;
        int result = is_valid_expression(exp);
        if (result == -1) {

        printf("\n\n Expression %d: %s and it's prefix notation is: ", exp_num, exp);
        infix_to_prefix(exp); //calling the function to print the result of converting from infix to prefix the valid expressions 
        flag_thereis_valid +=1; // indicator that now sure there is at least one valid expression 
       }
    }
        fclose(fp);

    if (flag_thereis_valid == 0){ //if there is no valid expression. print a clarifying note  
        printf("No valid expressions in the provided file.\n");
       }
    exp_num = 0;
    }
    }
    else if (choice == 4){
         if(flag_data_isread==0){
            printf("You should read the expressions from the file first.\n");

        }
    else {

         fp = fopen(input_file_name, "r");
         while (fgets(exp, 100, fp) != NULL) {
        exp_num++;
        int result = is_valid_expression(exp);
        if (result == -1) {
         infix_to_prefix(exp);
         int evaluate = evaluate_prefix_expression(exp); //calling the evaliation function 
         printf("Equation No: %d -> %d\n\n", exp_num, evaluate);
        }
    }
    fclose(fp);
    exp_num = 0;

    }
    }
    else if (choice == 5){
         if(flag_data_isread==0){
            printf("You should read the expressions from the file first.\n");

        }

 
    else { //See the invalid expressions and print them 
         fp = fopen(input_file_name, "r");
        while (fgets(exp, 100, fp) != NULL) {
        exp_num++;
        int result = is_valid_expression(exp);
        if (result == 0 || result == 1) {
        printf("\nExpression %d: %s is INVALID\n", exp_num, exp);
        flag_thereis_invalid +=1; // indicator that now sure there is at least one invalid expression 
        }    
    }
            fclose(fp);

        if (flag_thereis_invalid == 0){ //if there is no invalid expression. print a clarifying note  
        printf("No Invalid expressions in the provided file.\n");
        }

    exp_num = 0;
    }
    }
    else if (choice == 6){
         if(flag_data_isread==0){
            printf("You should read the expressions from the file first.\n");

        }
    else {
        fp = fopen(input_file_name, "r");
        while (fgets(exp, 100, fp) != NULL) {
        exp_num++;
        int result = is_valid_expression(exp); 
    write_equation_to_file(exp, result,exp_num); // to print the results on the output file 
  
    }
     fclose(fp);
    exp_num = 0;
    printf("Output has been displayed on the output.txt file.\n");
    }
    }
    else if (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7) {
        printf("Invalid operator, please try again.\n");

    }
}   
if (choice == 7)
    printf("Exit. Thank you for using the program\n");
    return 0;
}