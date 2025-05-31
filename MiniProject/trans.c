#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer
{
    int account_no;
    char customer_first_name[25];
    char customer_last_name[25];
    float balance;
};

void newcustomer();
void customerdetails();
void update_customerdetails();
void deposit();
void withdraw();

int main()
{
    int option;

    while (1)
    {
        printf("\n_______________________________\n");
        printf("1. New Customer\n");
        printf("2. Customer Details\n");
        printf("3. Update Customer Details\n");
        printf("4. Deposit Amount\n");
        printf("5. Withdraw Amount\n");
        printf("6. Exit\n");
        printf("_______________________________\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            newcustomer();
            break;
        case 2:
            customerdetails();
            break;
        case 3:
            update_customerdetails();
            break;
        case 4:
            deposit();
            break;
        case 5:
            withdraw();
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

// Add new customer
void newcustomer()
{
    FILE *fb, *ftxt;
    struct customer c;
    int new_account_no;
    int exists = 0;

    printf("Enter Account Number: ");
    scanf("%d", &new_account_no);

    // Check if account number already exists
    fb = fopen("customer.dat", "rb");
    if (fb != NULL)
    {
        while (fread(&c, sizeof(struct customer), 1, fb))
        {
            if (c.account_no == new_account_no)
            {
                exists = 1;
                break;
            }
        }
        fclose(fb);
    }

    if (exists)
    {
        printf("Account number already exists! Please try a different number.\n");
        return;
    }

    fb = fopen("customer.dat", "ab");
    ftxt = fopen("customer.txt", "a");

    if (fb == NULL || ftxt == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    c.account_no = new_account_no;
    printf("Enter Customer First Name: ");
    scanf("%s", c.customer_first_name);
    printf("Enter Customer Last Name: ");
    scanf("%s", c.customer_last_name);
    printf("Enter Account Balance in Rs: ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(struct customer), 1, fb);
    fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);

    printf("Customer added successfully!\n");

    fclose(fb);
    fclose(ftxt);
}

// Display all customers
void customerdetails()
{
    FILE *fb, *ftxt;
    struct customer c;

    fb = fopen("customer.dat", "rb");
    ftxt = fopen("customer.txt", "w");

    if (fb == NULL || ftxt == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nCustomer Account Details:\n");
    printf("--------------------------------------------------\n");
    while (fread(&c, sizeof(struct customer), 1, fb))
    {
        printf("Account No: %d\n", c.account_no);
        printf("Name      : %s %s\n", c.customer_first_name, c.customer_last_name);
        printf("Balance   : %.2f\n", c.balance);
        printf("--------------------------------------------------\n");

        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);
    }

    fclose(fb);
    fclose(ftxt);
}

// Update customer name
void update_customerdetails()
{
    FILE *fb;
    struct customer c;
    int account_no, found = 0;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account Number to update: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb))
    {
        if (c.account_no == account_no)
        {
            found = 1;
            printf("Current Info:\n");
            printf("Name    : %s %s\n", c.customer_first_name, c.customer_last_name);
            
            printf("\nEnter new First Name: ");
            scanf("%s", c.customer_first_name);
            printf("Enter new Last Name: ");
            scanf("%s", c.customer_last_name);


            fseek(fb, -sizeof(struct customer), SEEK_CUR); // Move back one record
            fwrite(&c, sizeof(struct customer), 1, fb);     // Overwrite record

            printf("Customer updated successfully.\n");
            break;
        }
    }

    fclose(fb);

    if (!found)
    {
        printf("Account number not found.\n");
        return;
    }

    customerdetails(); // refresh text file
}

// Deposit amount
void deposit()
{
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account Number to deposit into: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb))
    {
        if (c.account_no == account_no)
        {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);

            if (amount <= 0)
            {
                printf("Invalid amount!\n");
                fclose(fb);
                return;
            }

            c.balance += amount;

            fseek(fb, -sizeof(struct customer), SEEK_CUR); // Move back one record
            fwrite(&c, sizeof(struct customer), 1, fb);     // Overwrite record

            printf("Deposit successful. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);

    if (!found)
    {
        printf("Account number not found.\n");
    }
    
}

// Withdraw amount
void withdraw()
{
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account Number to withdraw from: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb))
    {
        if (c.account_no == account_no)
        {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > c.balance)
            {
                printf("Invalid amount! Withdrawal failed.\n");
                fclose(fb);
                return;
            }

            c.balance -= amount;

            fseek(fb, -sizeof(struct customer), SEEK_CUR); // Move back one record
            fwrite(&c, sizeof(struct customer), 1, fb);     // Overwrite record

            printf("Withdrawal successful. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);

    if (!found)
    {
        printf("Account number not found.\n");
    }

}
