#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_QUERY_LENGTH 2048
#define MAX_STRING_LENGTH 256
#define MAX_MEDICINES 1000
#define MAX_PATIENTS 1000
#define MAX_BILLS 1000
#define MAX_QUEUE_ITEMS 100
#define MAX_SALES_HISTORY 1000

// Data structures
typedef struct {
    int id;
    char name[100];
    char category[50];
    int stock;
    float unit_price;
    float selling_price;
    char expiry_date[20];
    char supplier[100];
} Medicine;

typedef struct {
    int id;
    char name[100];
    int age;
    char gender[10];
    char phone[15];
    char email[100];
    char address[200];
    char registration_date[20];
} Patient;

typedef struct {
    int id;
    int patient_id;
    int medicine_id;
    int quantity;
    float price;
    float total;
    char date[20];
    char status[20];
} BillItem;

typedef struct {
    int bill_id;
    int patient_id;
    float total_amount;
    char payment_method[20];
    char date[20];
    char status[20];
    int priority;
} PaymentQueueItem;

typedef struct {
    int id;
    int patient_id;
    char patient_name[100];
    float total_amount;
    char payment_method[20];
    char date[20];
    char status[20];
} SalesHistory;

// Global variables
Medicine medicines[MAX_MEDICINES];
Patient patients[MAX_PATIENTS];
BillItem bill_items[MAX_BILLS];
PaymentQueueItem queue[MAX_QUEUE_ITEMS];
SalesHistory sales_history[MAX_SALES_HISTORY];

int medicine_count = 0;
int patient_count = 0;
int bill_count = 0;
int queue_count = 0;
int sales_history_count = 0;

// Utility functions
void clear_screen() {
    system("cls");
}

void print_header(const char* title) {
    clear_screen();
    printf("\n========================================\n");
    printf("    %s\n", title);
    printf("========================================\n\n");
}

void trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

// Initialize sample data with Indian names
void initialize_sample_data() {
    // Sample medicines with Indian names
    strcpy(medicines[0].name, "Paracetamol 500mg");
    strcpy(medicines[0].category, "दर्द निवारक");
    medicines[0].stock = 100;
    medicines[0].unit_price = 5.50;
    medicines[0].selling_price = 8.99;
    strcpy(medicines[0].expiry_date, "31-12-2025");
    strcpy(medicines[0].supplier, "राजेंद्र मेडिकल");
    medicines[0].id = 1;
    medicine_count++;

    strcpy(medicines[1].name, "Azithromycin 250mg");
    strcpy(medicines[1].category, "एंटीबायोटिक");
    medicines[1].stock = 50;
    medicines[1].unit_price = 15.00;
    medicines[1].selling_price = 25.99;
    strcpy(medicines[1].expiry_date, "15-06-2025");
    strcpy(medicines[1].supplier, "सुरभि फार्मा");
    medicines[1].id = 2;
    medicine_count++;

    strcpy(medicines[2].name, "Dolo 650mg");
    strcpy(medicines[2].category, "दर्द निवारक");
    medicines[2].stock = 75;
    medicines[2].unit_price = 7.00;
    medicines[2].selling_price = 12.50;
    strcpy(medicines[2].expiry_date, "20-09-2025");
    strcpy(medicines[2].supplier, "सन फार्मा");
    medicines[2].id = 3;
    medicine_count++;

    strcpy(medicines[3].name, "Amoxicillin 500mg");
    strcpy(medicines[3].category, "एंटीबायोटिक");
    medicines[3].stock = 60;
    medicines[3].unit_price = 12.00;
    medicines[3].selling_price = 20.99;
    strcpy(medicines[3].expiry_date, "10-12-2025");
    strcpy(medicines[3].supplier, "सिप्ला");
    medicines[3].id = 4;
    medicine_count++;

    strcpy(medicines[4].name, "Crocin 500mg");
    strcpy(medicines[4].category, "दर्द निवारक");
    medicines[4].stock = 80;
    medicines[4].unit_price = 6.50;
    medicines[4].selling_price = 10.99;
    strcpy(medicines[4].expiry_date, "25-08-2025");
    strcpy(medicines[4].supplier, "रेड्डीज लैब");
    medicines[4].id = 5;
    medicine_count++;

    // Sample patients with Indian names
    strcpy(patients[0].name, "राजेश कुमार");
    patients[0].age = 35;
    strcpy(patients[0].gender, "पुरुष");
    strcpy(patients[0].phone, "9876543210");
    strcpy(patients[0].email, "rajesh.kumar@email.com");
    strcpy(patients[0].address, "123 मुखर्जी नगर, दिल्ली");
    strcpy(patients[0].registration_date, "01-01-2024");
    patients[0].id = 1;
    patient_count++;

    strcpy(patients[1].name, "प्रिया शर्मा");
    patients[1].age = 28;
    strcpy(patients[1].gender, "महिला");
    strcpy(patients[1].phone, "9876543211");
    strcpy(patients[1].email, "priya.sharma@email.com");
    strcpy(patients[1].address, "456 बांद्रा, मुंबई");
    strcpy(patients[1].registration_date, "02-01-2024");
    patients[1].id = 2;
    patient_count++;

    strcpy(patients[2].name, "अमित सिंह");
    patients[2].age = 42;
    strcpy(patients[2].gender, "पुरुष");
    strcpy(patients[2].phone, "9876543212");
    strcpy(patients[2].email, "amit.singh@email.com");
    strcpy(patients[2].address, "789 साल्ट लेक, कोलकाता");
    strcpy(patients[2].registration_date, "03-01-2024");
    patients[2].id = 3;
    patient_count++;

    strcpy(patients[3].name, "नेहा गुप्ता");
    patients[3].age = 31;
    strcpy(patients[3].gender, "महिला");
    strcpy(patients[3].phone, "9876543213");
    strcpy(patients[3].email, "neha.gupta@email.com");
    strcpy(patients[3].address, "321 इंदिरानगर, बैंगलोर");
    strcpy(patients[3].registration_date, "04-01-2024");
    patients[3].id = 4;
    patient_count++;

    strcpy(patients[4].name, "विक्रम राठौर");
    patients[4].age = 38;
    strcpy(patients[4].gender, "पुरुष");
    strcpy(patients[4].phone, "9876543214");
    strcpy(patients[4].email, "vikram.rathore@email.com");
    strcpy(patients[4].address, "654 जयपुर, राजस्थान");
    strcpy(patients[4].registration_date, "05-01-2024");
    patients[4].id = 5;
    patient_count++;

    // Sample sales history
    strcpy(sales_history[0].patient_name, "राजेश कुमार");
    sales_history[0].patient_id = 1;
    sales_history[0].total_amount = 125.50;
    strcpy(sales_history[0].payment_method, "कैश");
    strcpy(sales_history[0].date, "15-01-2024");
    strcpy(sales_history[0].status, "भुगतान हो गया");
    sales_history[0].id = 1;
    sales_history_count++;

    strcpy(sales_history[1].patient_name, "प्रिया शर्मा");
    sales_history[1].patient_id = 2;
    sales_history[1].total_amount = 89.99;
    strcpy(sales_history[1].payment_method, "कार्ड");
    strcpy(sales_history[1].date, "16-01-2024");
    strcpy(sales_history[1].status, "भुगतान हो गया");
    sales_history[1].id = 2;
    sales_history_count++;
}

// Medicine Management Functions
void add_medicine() {
    print_header("Add New Medicine");
    
    if (medicine_count >= MAX_MEDICINES) {
        printf("Maximum medicine limit reached!\n");
        return;
    }
    
    Medicine new_medicine;
    new_medicine.id = medicine_count + 1;
    
    printf("Enter medicine name: ");
    fgets(new_medicine.name, sizeof(new_medicine.name), stdin);
    trim_whitespace(new_medicine.name);
    
    printf("Enter category: ");
    fgets(new_medicine.category, sizeof(new_medicine.category), stdin);
    trim_whitespace(new_medicine.category);
    
    printf("Enter stock quantity: ");
    scanf("%d", &new_medicine.stock);
    
    printf("Enter unit price: ");
    scanf("%f", &new_medicine.unit_price);
    
    printf("Enter selling price: ");
    scanf("%f", &new_medicine.selling_price);
    
    printf("Enter expiry date (DD-MM-YYYY): ");
    scanf("%s", new_medicine.expiry_date);
    
    getchar(); // Consume newline
    
    printf("Enter supplier name: ");
    fgets(new_medicine.supplier, sizeof(new_medicine.supplier), stdin);
    trim_whitespace(new_medicine.supplier);
    
    medicines[medicine_count++] = new_medicine;
    printf("\n✓ Medicine added successfully!\n");
    
    printf("\nPress Enter to continue...");
    getchar();
}

void view_medicines() {
    print_header("Medicine List");
    
    if (medicine_count == 0) {
        printf("No medicines found!\n");
        return;
    }
    
    printf("%-5s %-30s %-20s %-10s %-12s %-12s %-15s %-20s\n", 
           "ID", "Name", "Category", "Stock", "Unit Price", "Sell Price", "Expiry", "Supplier");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < medicine_count; i++) {
        printf("%-5d %-30s %-20s %-10d ₹%-11.2f ₹%-11.2f %-15s %-20s\n",
               medicines[i].id, medicines[i].name, medicines[i].category, 
               medicines[i].stock, medicines[i].unit_price, medicines[i].selling_price,
               medicines[i].expiry_date, medicines[i].supplier);
    }
    
    printf("\nTotal Medicines: %d\n", medicine_count);
    printf("\nPress Enter to continue...");
    getchar();
}

void search_medicine() {
    print_header("Search Medicine");
    
    char search_term[100];
    printf("Enter medicine name to search: ");
    fgets(search_term, sizeof(search_term), stdin);
    trim_whitespace(search_term);
    
    printf("\nSearch Results:\n");
    printf("%-5s %-30s %-20s %-10s %-12s %-12s %-15s %-20s\n", 
           "ID", "Name", "Category", "Stock", "Unit Price", "Sell Price", "Expiry", "Supplier");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < medicine_count; i++) {
        if (strstr(medicines[i].name, search_term) != NULL) {
            printf("%-5d %-30s %-20s %-10d ₹%-11.2f ₹%-11.2f %-15s %-20s\n",
                   medicines[i].id, medicines[i].name, medicines[i].category, 
                   medicines[i].stock, medicines[i].unit_price, medicines[i].selling_price,
                   medicines[i].expiry_date, medicines[i].supplier);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No medicines found matching '%s'\n", search_term);
    } else {
        printf("\nFound %d medicine(s)\n", found);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void delete_medicine() {
    print_header("Delete Medicine");
    
    view_medicines();
    
    int id;
    printf("Enter medicine ID to delete: ");
    scanf("%d", &id);
    getchar();
    
    int found = 0;
    for (int i = 0; i < medicine_count; i++) {
        if (medicines[i].id == id) {
            // Shift all remaining medicines
            for (int j = i; j < medicine_count - 1; j++) {
                medicines[j] = medicines[j + 1];
            }
            medicine_count--;
            found = 1;
            printf("✓ Medicine deleted successfully!\n");
            break;
        }
    }
    
    if (!found) {
        printf("❌ Medicine with ID %d not found!\n", id);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Patient Management Functions
void add_patient() {
    print_header("Add New Patient");
    
    if (patient_count >= MAX_PATIENTS) {
        printf("Maximum patient limit reached!\n");
        return;
    }
    
    Patient new_patient;
    new_patient.id = patient_count + 1;
    
    printf("Enter patient name: ");
    fgets(new_patient.name, sizeof(new_patient.name), stdin);
    trim_whitespace(new_patient.name);
    
    printf("Enter age: ");
    scanf("%d", &new_patient.age);
    getchar();
    
    printf("Enter gender (पुरुष/महिला): ");
    fgets(new_patient.gender, sizeof(new_patient.gender), stdin);
    trim_whitespace(new_patient.gender);
    
    printf("Enter phone number (10 digits): ");
    fgets(new_patient.phone, sizeof(new_patient.phone), stdin);
    trim_whitespace(new_patient.phone);
    
    // Validate phone number
    if (strlen(new_patient.phone) != 10) {
        printf("❌ Phone number must be exactly 10 digits!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    for (int i = 0; i < strlen(new_patient.phone); i++) {
        if (!isdigit(new_patient.phone[i])) {
            printf("❌ Phone number must contain only digits!\n");
            printf("\nPress Enter to continue...");
            getchar();
            return;
        }
    }
    
    printf("Enter email (must contain @ and .com): ");
    fgets(new_patient.email, sizeof(new_patient.email), stdin);
    trim_whitespace(new_patient.email);
    
    // Validate email
    if (strstr(new_patient.email, "@") == NULL || strstr(new_patient.email, ".com") == NULL) {
        printf("❌ Email must contain '@' and '.com'!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printf("Enter address: ");
    fgets(new_patient.address, sizeof(new_patient.address), stdin);
    trim_whitespace(new_patient.address);
    
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(new_patient.registration_date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    patients[patient_count++] = new_patient;
    printf("\n✓ Patient added successfully!\n");
    
    printf("\nPress Enter to continue...");
    getchar();
}

void view_patients() {
    print_header("Patient List");
    
    if (patient_count == 0) {
        printf("No patients found!\n");
        return;
    }
    
    printf("%-5s %-25s %-8s %-10s %-15s %-25s %-30s %-15s\n", 
           "ID", "Name", "Age", "Gender", "Phone", "Email", "Address", "Reg. Date");
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < patient_count; i++) {
        printf("%-5d %-25s %-8d %-10s %-15s %-25s %-30s %-15s\n",
               patients[i].id, patients[i].name, patients[i].age, patients[i].gender,
               patients[i].phone, patients[i].email, patients[i].address, patients[i].registration_date);
    }
    
    printf("\nTotal Patients: %d\n", patient_count);
    printf("\nPress Enter to continue...");
    getchar();
}

void search_patient() {
    print_header("Search Patient");
    
    char search_term[100];
    printf("Enter patient name to search: ");
    fgets(search_term, sizeof(search_term), stdin);
    trim_whitespace(search_term);
    
    printf("\nSearch Results:\n");
    printf("%-5s %-25s %-8s %-10s %-15s %-25s %-30s %-15s\n", 
           "ID", "Name", "Age", "Gender", "Phone", "Email", "Address", "Reg. Date");
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < patient_count; i++) {
        if (strstr(patients[i].name, search_term) != NULL) {
            printf("%-5d %-25s %-8d %-10s %-15s %-25s %-30s %-15s\n",
                   patients[i].id, patients[i].name, patients[i].age, patients[i].gender,
                   patients[i].phone, patients[i].email, patients[i].address, patients[i].registration_date);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No patients found matching '%s'\n", search_term);
    } else {
        printf("\nFound %d patient(s)\n", found);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void delete_patient() {
    print_header("Delete Patient");
    
    view_patients();
    
    int id;
    printf("Enter patient ID to delete: ");
    scanf("%d", &id);
    getchar();
    
    int found = 0;
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == id) {
            // Shift all remaining patients
            for (int j = i; j < patient_count - 1; j++) {
                patients[j] = patients[j + 1];
            }
            patient_count--;
            found = 1;
            printf("✓ Patient deleted successfully!\n");
            break;
        }
    }
    
    if (!found) {
        printf("❌ Patient with ID %d not found!\n", id);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Billing Functions
void create_bill() {
    print_header("Create New Bill");
    
    if (patient_count == 0) {
        printf("❌ No patients available! Please add patients first.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    if (medicine_count == 0) {
        printf("❌ No medicines available! Please add medicines first.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    // Select patient
    printf("Available Patients:\n");
    for (int i = 0; i < patient_count; i++) {
        printf("%d. %s (%s)\n", patients[i].id, patients[i].name, patients[i].phone);
    }
    
    int patient_id;
    printf("\nEnter patient ID: ");
    scanf("%d", &patient_id);
    getchar();
    
    // Validate patient
    int patient_found = 0;
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == patient_id) {
            patient_found = 1;
            break;
        }
    }
    
    if (!patient_found) {
        printf("❌ Patient not found!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    // Add medicines to bill
    float total_amount = 0.0;
    char choice;
    
    do {
        printf("\nAvailable Medicines:\n");
        for (int i = 0; i < medicine_count; i++) {
            printf("%d. %s - ₹%.2f (Stock: %d)\n", medicines[i].id, medicines[i].name, medicines[i].selling_price, medicines[i].stock);
        }
        
        int medicine_id, quantity;
        printf("\nEnter medicine ID: ");
        scanf("%d", &medicine_id);
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        getchar();
        
        // Validate medicine
        int medicine_found = 0;
        for (int i = 0; i < medicine_count; i++) {
            if (medicines[i].id == medicine_id) {
                medicine_found = 1;
                if (quantity > medicines[i].stock) {
                    printf("❌ Insufficient stock! Available: %d\n", medicines[i].stock);
                } else {
                    // Add to bill items
                    BillItem item;
                    item.id = bill_count + 1;
                    item.patient_id = patient_id;
                    item.medicine_id = medicine_id;
                    item.quantity = quantity;
                    item.price = medicines[i].selling_price;
                    item.total = quantity * medicines[i].selling_price;
                    
                    // Get current date
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    sprintf(item.date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                    strcpy(item.status, "बिल में");
                    
                    bill_items[bill_count++] = item;
                    total_amount += item.total;
                    
                    // Update stock
                    medicines[i].stock -= quantity;
                    
                    printf("✓ Added %d x %s to bill\n", quantity, medicines[i].name);
                }
                break;
            }
        }
        
        if (!medicine_found) {
            printf("❌ Medicine not found!\n");
        }
        
        printf("\nAdd more medicines? (y/n): ");
        scanf("%c", &choice);
        getchar();
        
    } while (choice == 'y' || choice == 'Y');
    
    // Process payment
    if (total_amount > 0) {
        printf("\n=== BILL SUMMARY ===\n");
        printf("Patient ID: %d\n", patient_id);
        printf("Total Amount: ₹%.2f\n", total_amount);
        
        printf("\nPayment Methods:\n");
        printf("1. कैश\n");
        printf("2. कार्ड\n");
        printf("3. UPI\n");
        
        int payment_choice;
        printf("Select payment method: ");
        scanf("%d", &payment_choice);
        getchar();
        
        char payment_method[20];
        switch (payment_choice) {
            case 1: strcpy(payment_method, "कैश"); break;
            case 2: strcpy(payment_method, "कार्ड"); break;
            case 3: strcpy(payment_method, "UPI"); break;
            default: strcpy(payment_method, "कैश"); break;
        }
        
        // Add to payment queue
        PaymentQueueItem queue_item;
        queue_item.bill_id = bill_count;
        queue_item.patient_id = patient_id;
        queue_item.total_amount = total_amount;
        strcpy(queue_item.payment_method, payment_method);
        
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(queue_item.date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        strcpy(queue_item.status, "लंबित");
        
        // Calculate priority based on amount
        if (total_amount > 1000) queue_item.priority = 1;
        else if (total_amount > 500) queue_item.priority = 2;
        else queue_item.priority = 3;
        
        queue[queue_count++] = queue_item;
        
        // Add to sales history
        SalesHistory sale;
        sale.id = sales_history_count + 1;
        sale.patient_id = patient_id;
        sale.total_amount = total_amount;
        strcpy(sale.payment_method, payment_method);
        strcpy(sale.date, queue_item.date);
        strcpy(sale.status, "भुगतान हो गया");
        
        // Get patient name
        for (int i = 0; i < patient_count; i++) {
            if (patients[i].id == patient_id) {
                strcpy(sale.patient_name, patients[i].name);
                break;
            }
        }
        
        sales_history[sales_history_count++] = sale;
        
        printf("\n✓ Bill created successfully!\n");
        printf("✓ Added to payment queue with priority %d\n", queue_item.priority);
        printf("✓ Payment processed successfully!\n");
    } else {
        printf("\n❌ No items added to bill!\n");
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void view_sales_history() {
    print_header("Sales History");
    
    if (sales_history_count == 0) {
        printf("No sales history found!\n");
        return;
    }
    
    printf("%-5s %-25s %-12s %-15s %-15s %-15s\n", 
           "ID", "Patient Name", "Amount", "Payment Method", "Date", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < sales_history_count; i++) {
        printf("%-5d %-25s ₹%-11.2f %-15s %-15s %-15s\n",
               sales_history[i].id, sales_history[i].patient_name, sales_history[i].total_amount,
               sales_history[i].payment_method, sales_history[i].date, sales_history[i].status);
    }
    
    printf("\nTotal Sales: %d\n", sales_history_count);
    
    float total_revenue = 0.0;
    for (int i = 0; i < sales_history_count; i++) {
        total_revenue += sales_history[i].total_amount;
    }
    printf("Total Revenue: ₹%.2f\n", total_revenue);
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Payment Queue Functions
void add_to_queue() {
    // This is handled automatically in create_bill()
    printf("Payments are automatically added to queue when bills are created.\n");
    printf("\nPress Enter to continue...");
    getchar();
}

void view_queue() {
    print_header("Payment Queue");
    
    if (queue_count == 0) {
        printf("No items in payment queue!\n");
        return;
    }
    
    printf("%-8s %-10s %-12s %-15s %-15s %-8s\n", 
           "Bill ID", "Patient ID", "Amount", "Payment Method", "Date", "Priority");
    printf("--------------------------------------------------------------------------------\n");
    
    // Sort by priority
    for (int i = 0; i < queue_count - 1; i++) {
        for (int j = 0; j < queue_count - i - 1; j++) {
            if (queue[j].priority > queue[j + 1].priority) {
                PaymentQueueItem temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < queue_count; i++) {
        printf("%-8d %-10d ₹%-10.2f %-15s %-15s %-8d\n",
               queue[i].bill_id, queue[i].patient_id, queue[i].total_amount,
               queue[i].payment_method, queue[i].date, queue[i].priority);
    }
    
    printf("\nTotal Queue Items: %d\n", queue_count);
    printf("\nPress Enter to continue...");
    getchar();
}

void process_next_payment() {
    print_header("Process Next Payment");
    
    if (queue_count == 0) {
        printf("No payments to process!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    // Find highest priority payment
    int highest_priority_index = 0;
    for (int i = 1; i < queue_count; i++) {
        if (queue[i].priority < queue[highest_priority_index].priority) {
            highest_priority_index = i;
        }
    }
    
    printf("Processing payment with highest priority (Priority %d):\n", queue[highest_priority_index].priority);
    printf("Bill ID: %d\n", queue[highest_priority_index].bill_id);
    printf("Patient ID: %d\n", queue[highest_priority_index].patient_id);
    printf("Amount: ₹%.2f\n", queue[highest_priority_index].total_amount);
    printf("Payment Method: %s\n", queue[highest_priority_index].payment_method);
    
    // Remove from queue
    for (int i = highest_priority_index; i < queue_count - 1; i++) {
        queue[i] = queue[i + 1];
    }
    queue_count--;
    
    printf("\n✓ Payment processed successfully!\n");
    printf("✓ Removed from payment queue\n");
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Report Functions
void generate_monthly_report() {
    print_header("Monthly Sales Report");
    
    if (sales_history_count == 0) {
        printf("No sales data available!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printf("=== MONTHLY SALES REPORT ===\n\n");
    
    float total_revenue = 0.0;
    int total_transactions = sales_history_count;
    
    for (int i = 0; i < sales_history_count; i++) {
        total_revenue += sales_history[i].total_amount;
    }
    
    printf("Total Revenue: ₹%.2f\n", total_revenue);
    printf("Total Transactions: %d\n", total_transactions);
    printf("Average Transaction Value: ₹%.2f\n", total_revenue / total_transactions);
    
    printf("\nRecent Transactions:\n");
    printf("%-5s %-25s %-12s %-15s\n", "ID", "Patient Name", "Amount", "Date");
    printf("------------------------------------------------\n");
    
    int start = (sales_history_count > 5) ? sales_history_count - 5 : 0;
    for (int i = start; i < sales_history_count; i++) {
        printf("%-5d %-25s ₹%-11.2f %-15s\n",
               sales_history[i].id, sales_history[i].patient_name, 
               sales_history[i].total_amount, sales_history[i].date);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void generate_inventory_report() {
    print_header("Inventory Report");
    
    if (medicine_count == 0) {
        printf("No medicines in inventory!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printf("=== INVENTORY REPORT ===\n\n");
    
    printf("Total Medicines: %d\n", medicine_count);
    
    int low_stock_count = 0;
    float total_inventory_value = 0.0;
    
    printf("\nLow Stock Items (Stock < 20):\n");
    printf("%-5s %-30s %-10s %-12s\n", "ID", "Name", "Stock", "Unit Price");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < medicine_count; i++) {
        total_inventory_value += medicines[i].stock * medicines[i].unit_price;
        
        if (medicines[i].stock < 20) {
            printf("%-5d %-30s %-10d ₹%-11.2f\n",
                   medicines[i].id, medicines[i].name, medicines[i].stock, medicines[i].unit_price);
            low_stock_count++;
        }
    }
    
    if (low_stock_count == 0) {
        printf("No low stock items!\n");
    }
    
    printf("\nTotal Inventory Value: ₹%.2f\n", total_inventory_value);
    printf("Low Stock Items: %d\n", low_stock_count);
    
    printf("\nPress Enter to continue...");
    getchar();
}

void generate_patient_report() {
    print_header("Patient Report");
    
    if (patient_count == 0) {
        printf("No patients registered!\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printf("=== PATIENT REPORT ===\n\n");
    
    printf("Total Patients: %d\n", patient_count);
    
    int male_count = 0, female_count = 0;
    float total_age = 0.0;
    
    for (int i = 0; i < patient_count; i++) {
        total_age += patients[i].age;
        if (strcmp(patients[i].gender, "पुरुष") == 0) {
            male_count++;
        } else if (strcmp(patients[i].gender, "महिला") == 0) {
            female_count++;
        }
    }
    
    printf("Male Patients: %d\n", male_count);
    printf("Female Patients: %d\n", female_count);
    printf("Average Age: %.1f\n", total_age / patient_count);
    
    printf("\nRecent Registrations:\n");
    printf("%-5s %-25s %-8s %-15s\n", "ID", "Name", "Age", "Registration Date");
    printf("------------------------------------------------\n");
    
    int start = (patient_count > 5) ? patient_count - 5 : 0;
    for (int i = start; i < patient_count; i++) {
        printf("%-5d %-25s %-8d %-15s\n",
               patients[i].id, patients[i].name, patients[i].age, patients[i].registration_date);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Main Menu
void display_main_menu() {
    printf("\n=== PHARMACY MANAGEMENT SYSTEM ===\n\n");
    printf("1. Medicine Management\n");
    printf("   1.1 Add Medicine\n");
    printf("   1.2 View Medicines\n");
    printf("   1.3 Search Medicine\n");
    printf("   1.4 Delete Medicine\n\n");
    
    printf("2. Patient Management\n");
    printf("   2.1 Add Patient\n");
    printf("   2.2 View Patients\n");
    printf("   2.3 Search Patient\n");
    printf("   2.4 Delete Patient\n\n");
    
    printf("3. Billing\n");
    printf("   3.1 Create Bill\n");
    printf("   3.2 View Sales History\n\n");
    
    printf("4. Payment Queue\n");
    printf("   4.1 View Queue\n");
    printf("   4.2 Process Next Payment\n\n");
    
    printf("5. Reports\n");
    printf("   5.1 Monthly Sales Report\n");
    printf("   5.2 Inventory Report\n");
    printf("   5.3 Patient Report\n\n");
    
    printf("0. Exit\n\n");
    printf("Enter your choice: ");
}

int main() {
    // Initialize sample data
    initialize_sample_data();
    
    int choice, sub_choice;
    
    do {
        clear_screen();
        display_main_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline
        
        switch (choice) {
            case 1:
                clear_screen();
                printf("=== MEDICINE MANAGEMENT ===\n\n");
                printf("1. Add Medicine\n");
                printf("2. View Medicines\n");
                printf("3. Search Medicine\n");
                printf("4. Delete Medicine\n");
                printf("0. Back to Main Menu\n\n");
                printf("Enter your choice: ");
                scanf("%d", &sub_choice);
                getchar();
                
                switch (sub_choice) {
                    case 1: add_medicine(); break;
                    case 2: view_medicines(); break;
                    case 3: search_medicine(); break;
                    case 4: delete_medicine(); break;
                    case 0: break;
                    default: printf("Invalid choice!\n"); break;
                }
                break;
                
            case 2:
                clear_screen();
                printf("=== PATIENT MANAGEMENT ===\n\n");
                printf("1. Add Patient\n");
                printf("2. View Patients\n");
                printf("3. Search Patient\n");
                printf("4. Delete Patient\n");
                printf("0. Back to Main Menu\n\n");
                printf("Enter your choice: ");
                scanf("%d", &sub_choice);
                getchar();
                
                switch (sub_choice) {
                    case 1: add_patient(); break;
                    case 2: view_patients(); break;
                    case 3: search_patient(); break;
                    case 4: delete_patient(); break;
                    case 0: break;
                    default: printf("Invalid choice!\n"); break;
                }
                break;
                
            case 3:
                clear_screen();
                printf("=== BILLING ===\n\n");
                printf("1. Create Bill\n");
                printf("2. View Sales History\n");
                printf("0. Back to Main Menu\n\n");
                printf("Enter your choice: ");
                scanf("%d", &sub_choice);
                getchar();
                
                switch (sub_choice) {
                    case 1: create_bill(); break;
                    case 2: view_sales_history(); break;
                    case 0: break;
                    default: printf("Invalid choice!\n"); break;
                }
                break;
                
            case 4:
                clear_screen();
                printf("=== PAYMENT QUEUE ===\n\n");
                printf("1. View Queue\n");
                printf("2. Process Next Payment\n");
                printf("0. Back to Main Menu\n\n");
                printf("Enter your choice: ");
                scanf("%d", &sub_choice);
                getchar();
                
                switch (sub_choice) {
                    case 1: view_queue(); break;
                    case 2: process_next_payment(); break;
                    case 0: break;
                    default: printf("Invalid choice!\n"); break;
                }
                break;
                
            case 5:
                clear_screen();
                printf("=== REPORTS ===\n\n");
                printf("1. Monthly Sales Report\n");
                printf("2. Inventory Report\n");
                printf("3. Patient Report\n");
                printf("0. Back to Main Menu\n\n");
                printf("Enter your choice: ");
                scanf("%d", &sub_choice);
                getchar();
                
                switch (sub_choice) {
                    case 1: generate_monthly_report(); break;
                    case 2: generate_inventory_report(); break;
                    case 3: generate_patient_report(); break;
                    case 0: break;
                    default: printf("Invalid choice!\n"); break;
                }
                break;
                
            case 0:
                printf("\nThank you for using Pharmacy Management System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
                printf("\nPress Enter to continue...");
                getchar();
                break;
        }
        
    } while (choice != 0);
    
    return 0;
}
