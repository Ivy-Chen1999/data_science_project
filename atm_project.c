#include <stdio.h>
#include <string.h>

#define Max_Attempts 3


int verifypin (FILE *,char*);
void displayMenu(void);
void withDraw(double *);
void deposit(double *);
void checkBalance(const double *);
int readchoice (void);
int readwithdraw(void);
double readdeposit(void);
void clearBuffer(void);


int main(void) {

    char accNumber[256];
    FILE *accFile;
    double saldo;
    int choice;
    char pinCode[256];

    printf("*** Welcome to use the ATM! ***");

    do {
        printf("\nGive account number: ");

        fgets(accNumber, 256, stdin);
        if (accNumber[strlen(accNumber) - 1] == '\n')
            accNumber[strlen(accNumber) - 1] = '\0';
        else
            clearBuffer();

        strcat(accNumber, ".acc");

        if ((accFile = fopen(accNumber, "r+")) != NULL) {

            for(int attempts = 1; attempts <=Max_Attempts;attempts++){

                printf("\nGive your account PIN code (%d. try): ",attempts);

                fgets(pinCode, 256, stdin);
                if( pinCode[strlen(pinCode)-1] == '\n')
                    pinCode[strlen(pinCode)-1] = '\0';
                else
                    clearBuffer();

                if (verifypin(accFile,pinCode)){

                    fscanf(accFile, "%lf", &saldo);

                    do{
                        displayMenu();
                        choice = readchoice();

                        switch (choice) {
                            case 1 :
                                withDraw(&saldo);
                                break;
                            case 2 :
                                deposit(&saldo);
                                break;
                            case 3 :
                                checkBalance(&saldo);
                                break;
                            case 9 :
                                fseek(accFile,(int)strlen(pinCode)+1, SEEK_SET);
                                fprintf(accFile, "%.2lf\n", saldo);
                                printf("The balance %.2lf saved to the account file \"%s\"",saldo,accNumber);
                                fclose(accFile);
                                return 0;
                            default:
                                break;
                        }

                    }while(1);
                }

                else {
                    printf("***** Wrong PIN code! (Press enter)");
                }

                rewind(accFile);

                if(attempts >= Max_Attempts){
                    printf("\n***** Too many attempts!\nYour account is closed");
                    break;
                }

            }

        }else{
            printf("Wrong account number!");
        }

    }while(1);


}

int verifypin (FILE *accFile,char *pinCode){

    char checkPin[256];
    int status;

    fgets(checkPin, 256, accFile);
    if (checkPin[strlen(checkPin)-1] == '\n')
        checkPin[strlen(checkPin)-1] = '\0';
    if (checkPin[strlen(checkPin)-1] == '\r')
        checkPin[strlen(checkPin)-1] = '\0';

    status = strcmp(pinCode, checkPin) == 0;
    return status;

}

void displayMenu(void){

    printf("Select an option...\n");
    printf("1. = Withdrawal\n");
    printf("2. = Deposit\n");
    printf("3. = Query balance\n");
    printf("9. = Quit\n");
    printf("Your choice: ");

}

void withDraw (double  *saldo){

    int amount;
    int total_fifty = 0;
    int total_twenty = 0;

    do {
        amount = readwithdraw();
    }while(amount > (*saldo - 20));

    if(amount == 0){
        printf("\n**********\nCancelled.\n**********\n");
        printf("\n\n***** Balance: %.2lf\n\n",*saldo);
    }

    else{
        if (amount >= 100){
            if(amount % 100 % 20 == 0||(amount % 100) % 50 == 0||(amount % 100) % 50 % 20 == 0){
                total_fifty += 2 * (int)(amount/100);
                amount -= 50 * total_fifty;
            }
            else{
                total_fifty += (1 + 2 * ((int)(amount/100) - 1)) ;
                amount -= 50 * total_fifty;
            }
        }

        if (amount >= 50) {
            int fifty_bills = amount / 50;
            if ( amount % 50 == 0) {
                total_fifty += fifty_bills;
            }
            else if (amount % 50 % 20 == 0) {
                total_fifty += fifty_bills;
                total_twenty += (int)((amount % 50) / 20);
            } else if(amount % 50 % 20 != 0) {
                total_twenty += (int)amount/20;
            }
        }

        if (amount < 50 && amount % 20 == 0) {
            total_twenty += amount/20;
        }

        int sumresult = (total_fifty * 50)+(total_twenty * 20);
        *saldo -= sumresult;

        printf("\n**************\nWithdrawal ok.\n*************\n\n");
        printf("Amount %d:\n",sumresult);
        printf("*** Notes 50: %d\n", total_fifty);
        printf("*** Notes 20: %d\n", total_twenty);
        printf("Check: (%d * 50) + (%d * 20) = %d.\n",total_fifty,total_twenty,sumresult);
        printf("\n\n***** Balance: %.2lf\n\n",*saldo);
    }
}

void deposit(double *saldo){

    double value;

    do {
        value = readdeposit();
    }while(value > (2000- *saldo));

    if(value == 0){
        printf("\n**********\nCancelled.\n**********\n");
        printf("\n\n***** Balance: %.2lf\n\n",*saldo);
    }

    else{
        *saldo += value;
        printf("\n***********\nDeposit ok.\n***********\n\n");
        printf("***** Balance: %.2lf\n\n",*saldo);
    }
}

void checkBalance(const double *saldo){

    printf("\n***** Balance: %.2lf\n",*saldo);

}

int readchoice(void){

    char input[50];
    int value;

    while(1){
        fgets(input,sizeof(input),stdin);

        if(sscanf(input,"%d",&value)!=1){
            printf("\nYou did not enter an integer!\n\n");
            displayMenu();
        }
        else if(value != 1 && value != 2 && value != 3 && value != 9 ){
            printf("\nWrong option, try again.\n\n");
            displayMenu();
        }
        else
            break;
    }
    return value;
}

int readwithdraw(void) {

    char input[50];
    int value;

    while (1) {
        printf("\nGive the amount to withdraw (full tens: 20, 40-1000): ");
        fgets(input,sizeof(input),stdin);

        if(sscanf(input,"%d",&value)!=1){
            printf("\nYou did not enter an integer!\n");
        }
        else if (value % 10 == 0 && value >= 20 && value < 1000 && value !=30 || value == 0) {
            return value;
        }
    }
}

double readdeposit(void){

    char input[50];
    double value;

    while (1) {
        printf("\nGive the money to deposit (min. 10.00, max. 1000.00 max. (max. saldo 2000.00)):");
        fgets(input,sizeof(input),stdin);

        if(sscanf(input,"%lf",&value)!=1){
            printf("\nYou did not enter an real number!\n");
        }
        else if (value >=10.0 && value <=1000.0 || value == 0 ) {
            return value;
        }
    }
}

void clearBuffer(void)
{
    while( fgetc(stdin) != '\n');
}