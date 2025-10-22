#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "medicine_data.dat"
#define MAX_ITEMS 50

struct Medicine {
    int id;
    char name[50];
    float price;
    int quantity;
    char expiry[15];
};

// Stack for billing (undo last item)
struct BillItem {
    int med_id;
    int qty;
    float amount;
} stack[MAX_ITEMS];
int top = -1;

// Stack functions
void push(int med_id, int qty, float amount) {
    if(top < MAX_ITEMS - 1) {
        top++;
        stack[top].med_id = med_id;
        stack[top].qty = qty;
        stack[top].amount = amount;
    }
}
struct BillItem pop() {
    return stack[top--];
}
int isEmpty() {
    return top == -1;
}

// Billing module
void billing() {
    FILE *fp;
    struct Medicine med;
    int choice, id, qty;

    while(1) {
        printf("\n--- Billing ---\n1. Add Medicine\n2. Undo Last\n3. Finish Bill\nChoice: ");
        scanf("%d",&choice);

        if(choice == 1) {
            printf("Medicine ID: "); scanf("%d",&id);
            printf("Quantity: "); scanf("%d",&qty);

            fp = fopen(FILENAME,"rb+");
            if(!fp){ printf("File not found!\n"); continue; }

            int found = 0;
            while(fread(&med,sizeof(med),1,fp)) {
                if(med.id == id) {
                    if(med.quantity >= qty) {
                        med.quantity -= qty;
                        fseek(fp,-sizeof(med),SEEK_CUR);
                        fwrite(&med,sizeof(med),1,fp);

                        push(id, qty, qty*med.price);
                        printf("Added %s x%d = %.2f\n",med.name,qty,qty*med.price);
                    } else {
                        printf("Not enough stock!\n");
                    }
                    found = 1;
                    break;
                }
            }
            if(!found) printf("Medicine not found!\n");
            fclose(fp);

        } else if(choice == 2) {
            if(isEmpty()) { printf("Nothing to undo!\n"); continue; }
            struct BillItem last = pop();
            fp = fopen(FILENAME,"rb+");
            if(!fp){ printf("File not found!\n"); continue; }
            while(fread(&med,sizeof(med),1,fp)) {
                if(med.id == last.med_id) {
                    med.quantity += last.qty;
                    fseek(fp,-sizeof(med),SEEK_CUR);
                    fwrite(&med,sizeof(med),1,fp);
                    printf("Removed last item ID %d x%d\n",last.med_id,last.qty);
                    break;
                }
            }
            fclose(fp);

        } else if(choice == 3) {
            float total=0;
            printf("\n--- Bill ---\nID\tQty\tAmount\n");
            for(int i=0;i<=top;i++){
                total += stack[i].amount;
                printf("%d\t%d\t%.2f\n",stack[i].med_id,stack[i].qty,stack[i].amount);
            }
            printf("Total: %.2f\n",total);
            top=-1; // clear stack
            break;
        } else printf("Invalid choice!\n");
    }
}

// Expiry & Low Stock module
void checkAlerts() {
    FILE *fp;
    struct Medicine med;

    fp = fopen(FILENAME,"rb");
    if(!fp)
    { 
        printf("File not found!\n"); 
        return;
    }

    printf("\n--- Low Stock (quantity<10) ---\n");
    int low=0;
    while(fread(&med,sizeof(med),1,fp)) {
        if(med.quantity < 10) {
            printf("%s - Only %d left\n",med.name,med.quantity);
            low=1;
        }
    }
    if(!low) printf("No low stock medicines.\n");

    rewind(fp);
    printf("\n--- Expired Medicines (assume today 01/10/2025) ---\n");
    int exp=0;
    while(fread(&med,sizeof(med),1,fp)) {
        if(strcmp(med.expiry,"01/10/2025")<0) {
            printf("%s - Expired on %s\n",med.name,med.expiry);
            exp=1;
        }
    }
    if(!exp) printf("No expired medicines.\n");

    fclose(fp);
}


int main() {
    int choice;
    while(1) {
        printf("\n--- Your Modules ---\n1. Billing\n2. Expiry & Low Stock\n3. Exit\nChoice: ");
        scanf("%d",&choice);
        if(choice==1) billing();
        else if(choice==2) checkAlerts();
        else if(choice==3) exit(0);
        else printf("Invalid choice!\n");
    }
    return 0;
}
