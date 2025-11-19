#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Data Structures
#define MAX_NAME_LEN 100
#define MAX_ID_LEN 20
#define MAX_EMAIL_LEN 100
#define MAX_PHONE_LEN 20
#define MAX_ADDRESS_LEN 200
#define MAX_MEDICINES 1000
#define MAX_PATIENTS 1000
#define MAX_BILLS 1000
#define MAX_EMERGENCY_QUEUE 500
#define MAX_STAFF 100
#define MAX_SUPPLIERS 100
#define MAX_BILL_ITEMS 20

// Date structure
typedef struct {
    int day;
    int month;
    int year;
} Date;

// Medicine structure
typedef struct {
    char medicineId[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char scientificName[MAX_NAME_LEN];
    char type[MAX_NAME_LEN];
    char manufacturer[MAX_NAME_LEN];
    int quantity;
    int minStock;
    float price;
    float gst;
    Date expiryDate;
    Date addedDate;
} Medicine;

// Patient structure
typedef struct {
    char patientId[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int age;
    char gender[10];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char address[MAX_ADDRESS_LEN    ];
    char medicalHistory[500];
    Date registrationDate;
} Patient;

// Bill item structure
typedef struct {
    char medicineId[MAX_ID_LEN];
    char medicineName[MAX_NAME_LEN];
    int quantity;
    float price;
    float total;
} BillItem;

// Bill structure
typedef struct {
    char billId[MAX_ID_LEN];
    char patientId[MAX_ID_LEN];
    Date date;
    BillItem items[MAX_BILL_ITEMS];
    int itemCount;
    float subtotal;
    float gst;
    float total;
    char paymentMode[20];
    char status[20];
} Bill;

// Emergency medicine structure
typedef struct {
    char medicineId[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char type[MAX_NAME_LEN];
    int currentStock;
    int urgencyScore;
    char priority[20];
    int daysUntilEmpty;
    float usageRate;
    char recommendedAction[100];
    char emergencyLevel[20];
    char reason[200];
    char notes[300];
    Date addedDate;
    int manualAddition;
} EmergencyMedicine;

// Staff structure
typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char position[MAX_NAME_LEN];
    char department[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    float salary;
    Date joinDate;
} Staff;

// Supplier structure
typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char address[MAX_ADDRESS_LEN];
    char gstNumber[50];
    char paymentTerms[50];
} Supplier;

// Global arrays and counters
Medicine medicines[MAX_MEDICINES];
int medicineCount = 0;

Patient patients[MAX_PATIENTS];
int patientCount = 0;

Bill bills[MAX_BILLS];
int billCount = 0;

EmergencyMedicine emergencyQueue[MAX_EMERGENCY_QUEUE];
int emergencyQueueCount = 0;

Staff staffMembers[MAX_STAFF];
int staffCount = 0;

Supplier suppliers[MAX_SUPPLIERS];
int supplierCount = 0;

// Utility functions
Date getCurrentDate() {
    Date date;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    date.day = tm->tm_mday;
    date.month = tm->tm_mon + 1;
    date.year = tm->tm_year + 1900;
    return date;
}

int generateId(char *prefix, char *id, int count) {
    sprintf(id, "%s%04d", prefix, count + 1);
    return 1;
}

// String utilities
void toLowerString(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int stringContains(const char *haystack, const char *needle) {
    char *result = strstr(haystack, needle);
    return result != NULL;
}

// Medicine Management Functions
void addMedicine() {
    if (medicineCount >= MAX_MEDICINES) {
        printf("Medicine storage is full!\n");
        return;
    }

    Medicine med;
    generateId("MED", med.medicineId, medicineCount);
    
    printf("Adding New Medicine\n");
    printf("Medicine ID: %s\n", med.medicineId);
    
    printf("Enter Medicine Name: ");
    scanf(" %[^\n]", med.name);
    
    printf("Enter Scientific Name: ");
    scanf(" %[^\n]", med.scientificName);
    
    printf("Enter Type (Tablet/Capsule/Syrup/Injection): ");
    scanf(" %[^\n]", med.type);
    
    printf("Enter Manufacturer: ");
    scanf(" %[^\n]", med.manufacturer);
    
    printf("Enter Quantity: ");
    scanf("%d", &med.quantity);
    
    printf("Enter Minimum Stock Level: ");
    scanf("%d", &med.minStock);
    
    printf("Enter Price: ");
    scanf("%f", &med.price);
    
    printf("Enter GST (%%): ");
    scanf("%f", &med.gst);
    
    printf("Enter Expiry Date (dd mm yyyy): ");
    scanf("%d %d %d", &med.expiryDate.day, &med.expiryDate.month, &med.expiryDate.year);
    
    med.addedDate = getCurrentDate();
    
    medicines[medicineCount++] = med;
    printf("Medicine added successfully!\n");
}

void displayMedicines() {
    printf("\n=== MEDICINE LIST ===\n");
    printf("%-10s %-20s %-20s %-15s %-10s %-10s %-10s\n", 
           "ID", "Name", "Scientific", "Type", "Stock", "Price", "Expiry");
    printf("----------------------------------------------------------------------------\n");
    
    for (int i = 0; i < medicineCount; i++) {
        printf("%-10s %-20s %-20s %-15s %-10d %-10.2f %02d/%02d/%04d\n",
               medicines[i].medicineId,
               medicines[i].name,
               medicines[i].scientificName,
               medicines[i].type,
               medicines[i].quantity,
               medicines[i].price,
               medicines[i].expiryDate.day,
               medicines[i].expiryDate.month,
               medicines[i].expiryDate.year);
    }
}

void searchMedicine() {
    char searchTerm[MAX_NAME_LEN];
    printf("Enter medicine name to search: ");
    scanf(" %[^\n]", searchTerm);
    
    printf("\n=== SEARCH RESULTS ===\n");
    int found = 0;
    
    for (int i = 0; i < medicineCount; i++) {
        if (stringContains(medicines[i].name, searchTerm) || 
            stringContains(medicines[i].scientificName, searchTerm)) {
            printf("ID: %s, Name: %s, Stock: %d, Price: %.2f\n",
                   medicines[i].medicineId, medicines[i].name, 
                   medicines[i].quantity, medicines[i].price);
            found++;
        }
    }
    
    if (!found) {
        printf("No medicines found matching '%s'\n", searchTerm);
    }
}

// Patient Management Functions
void addPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Patient storage is full!\n");
        return;
    }

    Patient patient;
    generateId("PAT", patient.patientId, patientCount);
    
    printf("Adding New Patient\n");
    printf("Patient ID: %s\n", patient.patientId);
    
    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patient.name);
    
    printf("Enter Age: ");
    scanf("%d", &patient.age);
    
    printf("Enter Gender (Male/Female/Other): ");
    scanf(" %[^\n]", patient.gender);
    
    printf("Enter Phone Number: ");
    scanf(" %[^\n]", patient.phone);
    
    printf("Enter Email: ");
    scanf(" %[^\n]", patient.email);
    
    printf("Enter Address: ");
    scanf(" %[^\n]", patient.address);
    
    printf("Enter Medical History: ");
    scanf(" %[^\n]", patient.medicalHistory);
    
    patient.registrationDate = getCurrentDate();
    
    patients[patientCount++] = patient;
    printf("Patient added successfully!\n");
}

void displayPatients() {
    printf("\n=== PATIENT LIST ===\n");
    printf("%-10s %-20s %-5s %-15s %-20s\n", 
           "ID", "Name", "Age", "Phone", "Email");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < patientCount; i++) {
        printf("%-10s %-20s %-5d %-15s %-20s\n",
               patients[i].patientId,
               patients[i].name,
               patients[i].age,
               patients[i].phone,
               patients[i].email);
    }
}

// Billing Functions
void createBill() {
    if (billCount >= MAX_BILLS) {
        printf("Bill storage is full!\n");
        return;
    }

    Bill bill;
    generateId("BILL", bill.billId, billCount);
    bill.date = getCurrentDate();
    bill.itemCount = 0;
    bill.subtotal = 0;
    bill.gst = 0;
    bill.total = 0;
    
    printf("Creating New Bill\n");
    printf("Bill ID: %s\n", bill.billId);
    
    // Display patients for selection
    displayPatients();
    printf("Enter Patient ID: ");
    scanf(" %[^\n]", bill.patientId);
    
    // Add items to bill
    char choice;
    do {
        if (bill.itemCount >= MAX_BILL_ITEMS) {
            printf("Maximum items reached for this bill!\n");
            break;
        }
        
        displayMedicines();
        printf("Enter Medicine ID: ");
        scanf(" %[^\n]", bill.items[bill.itemCount].medicineId);
        
        // Find medicine
        int medIndex = -1;
        for (int i = 0; i < medicineCount; i++) {
            if (strcmp(medicines[i].medicineId, bill.items[bill.itemCount].medicineId) == 0) {
                medIndex = i;
                break;
            }
        }
        
        if (medIndex == -1) {
            printf("Medicine not found!\n");
            continue;
        }
        
        printf("Enter Quantity: ");
        scanf("%d", &bill.items[bill.itemCount].quantity);
        
        if (medicines[medIndex].quantity < bill.items[bill.itemCount].quantity) {
            printf("Insufficient stock! Available: %d\n", medicines[medIndex].quantity);
            continue;
        }
        
        // Calculate item total
        strcpy(bill.items[bill.itemCount].medicineName, medicines[medIndex].name);
        bill.items[bill.itemCount].price = medicines[medIndex].price;
        bill.items[bill.itemCount].total = bill.items[bill.itemCount].quantity * 
                                          medicines[medIndex].price;
        
        // Update medicine stock
        medicines[medIndex].quantity -= bill.items[bill.itemCount].quantity;
        
        bill.subtotal += bill.items[bill.itemCount].total;
        bill.itemCount++;
        
        printf("Item added. Continue? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    
    // Calculate GST and total
    bill.gst = bill.subtotal * 0.18; // 18% GST
    bill.total = bill.subtotal + bill.gst;
    
    printf("Enter Payment Mode (Cash/Card/UPI): ");
    scanf(" %[^\n]", bill.paymentMode);
    
    strcpy(bill.status, "Paid");
    
    bills[billCount++] = bill;
    
    // Display bill
    printf("\n=== BILL GENERATED ===\n");
    printf("Bill ID: %s\n", bill.billId);
    printf("Patient ID: %s\n", bill.patientId);
    printf("Date: %02d/%02d/%04d\n", bill.date.day, bill.date.month, bill.date.year);
    printf("\nItems:\n");
    for (int i = 0; i < bill.itemCount; i++) {
        printf("%-20s %5d %10.2f %10.2f\n", 
               bill.items[i].medicineName, 
               bill.items[i].quantity, 
               bill.items[i].price, 
               bill.items[i].total);
    }
    printf("\nSubtotal: %.2f\n", bill.subtotal);
    printf("GST (18%%): %.2f\n", bill.gst);
    printf("Total: %.2f\n", bill.total);
    printf("Payment Mode: %s\n", bill.paymentMode);
}

// Emergency Medicine Queue Functions
int calculateUsageRate(const char *medicineId) {
    int totalUsage = 0;
    Date thirtyDaysAgo = getCurrentDate();
    
    // Calculate date 30 days ago (simplified)
    if (thirtyDaysAgo.day > 30) {
        thirtyDaysAgo.day -= 30;
    } else {
        thirtyDaysAgo.day += 30;
        thirtyDaysAgo.month--;
        if (thirtyDaysAgo.month < 1) {
            thirtyDaysAgo.month += 12;
            thirtyDaysAgo.year--;
        }
    }
    
    for (int i = 0; i < billCount; i++) {
        if (bills[i].date.year > thirtyDaysAgo.year ||
            (bills[i].date.year == thirtyDaysAgo.year && bills[i].date.month > thirtyDaysAgo.month) ||
            (bills[i].date.year == thirtyDaysAgo.year && bills[i].date.month == thirtyDaysAgo.month && 
             bills[i].date.day >= thirtyDaysAgo.day)) {
            
            for (int j = 0; j < bills[i].itemCount; j++) {
                if (strcmp(bills[i].items[j].medicineId, medicineId) == 0) {
                    totalUsage += bills[i].items[j].quantity;
                }
            }
        }
    }
    
    return totalUsage / 30; // Average per day
}

void calculateUrgency(Medicine *med, EmergencyMedicine *emergencyMed) {
    int usageRate = calculateUsageRate(med->medicineId);
    int stock = med->quantity;
    int minStock = med->minStock;
    
    // Base urgency calculation
    if (stock <= 0) {
        emergencyMed->urgencyScore = 100;
        strcpy(emergencyMed->priority, "critical");
        strcpy(emergencyMed->recommendedAction, "Immediate Order Required");
        strcpy(emergencyMed->emergencyLevel, "Critical");
    } else if (stock < minStock / 2) {
        emergencyMed->urgencyScore = 80;
        strcpy(emergencyMed->priority, "critical");
        strcpy(emergencyMed->recommendedAction, "Immediate Order Required");
        strcpy(emergencyMed->emergencyLevel, "Critical");
    } else if (stock < minStock) {
        emergencyMed->urgencyScore = 60;
        strcpy(emergencyMed->priority, "high");
        strcpy(emergencyMed->recommendedAction, "Order This Week");
        strcpy(emergencyMed->emergencyLevel, "High");
    } else if (stock < minStock * 1.5) {
        emergencyMed->urgencyScore = 40;
        strcpy(emergencyMed->priority, "medium");
        strcpy(emergencyMed->recommendedAction, "Plan Refill Soon");
        strcpy(emergencyMed->emergencyLevel, "Medium");
    } else {
        emergencyMed->urgencyScore = 20;
        strcpy(emergencyMed->priority, "low");
        strcpy(emergencyMed->recommendedAction, "Monitor Stock");
        strcpy(emergencyMed->emergencyLevel, "Normal");
    }
    
    // Check if it's an emergency medicine
    char *emergencyMedicines[] = {"paracetamol", "insulin", "aspirin", "salbutamol", "adrenaline", "oxygen"};
    char lowerName[MAX_NAME_LEN];
    strcpy(lowerName, med->name);
    toLowerString(lowerName);
    
    int isEmergencyMed = 0;
    for (int i = 0; i < 6; i++) {
        if (stringContains(lowerName, emergencyMedicines[i])) {
            isEmergencyMed = 1;
            break;
        }
    }
    
    if (isEmergencyMed) {
        emergencyMed->urgencyScore += 20;
        if (strcmp(emergencyMed->priority, "high") == 0) {
            strcpy(emergencyMed->priority, "critical");
            strcpy(emergencyMed->emergencyLevel, "Critical");
        } else if (strcmp(emergencyMed->priority, "medium") == 0) {
            strcpy(emergencyMed->priority, "high");
            strcpy(emergencyMed->emergencyLevel, "High");
        }
    }
    
    // Boost for high usage rate
    if (usageRate > 5) {
        emergencyMed->urgencyScore += 15;
    }
    
    emergencyMed->usageRate = usageRate;
    emergencyMed->daysUntilEmpty = stock > 0 ? (stock / (usageRate > 0 ? usageRate : 1)) : 0;
    
    // Cap urgency score at 100
    if (emergencyMed->urgencyScore > 100) {
        emergencyMed->urgencyScore = 100;
    }
}

void updateEmergencyQueue() {
    emergencyQueueCount = 0;
    
    for (int i = 0; i < medicineCount; i++) {
        if (medicines[i].quantity <= medicines[i].minStock) {
            EmergencyMedicine emergencyMed;
            
            strcpy(emergencyMed.medicineId, medicines[i].medicineId);
            strcpy(emergencyMed.name, medicines[i].name);
            strcpy(emergencyMed.type, medicines[i].type);
            emergencyMed.currentStock = medicines[i].quantity;
            
            calculateUrgency(&medicines[i], &emergencyMed);
            
            strcpy(emergencyMed.reason, "Auto-detected low stock");
            strcpy(emergencyMed.notes, "Automatically added to emergency queue");
            emergencyMed.addedDate = getCurrentDate();
            emergencyMed.manualAddition = 0;
            
            emergencyQueue[emergencyQueueCount++] = emergencyMed;
        }
    }
    
    printf("Emergency queue updated with %d medicines\n", emergencyQueueCount);
}

void addMedicineToEmergencyQueue() {
    if (emergencyQueueCount >= MAX_EMERGENCY_QUEUE) {
        printf("Emergency queue is full!\n");
        return;
    }
    
    displayMedicines();
    char medicineId[MAX_ID_LEN];
    printf("Enter Medicine ID to add to emergency queue: ");
    scanf(" %[^\n]", medicineId);
    
    // Find medicine
    int medIndex = -1;
    for (int i = 0; i < medicineCount; i++) {
        if (strcmp(medicines[i].medicineId, medicineId) == 0) {
            medIndex = i;
            break;
        }
    }
    
    if (medIndex == -1) {
        printf("Medicine not found!\n");
        return;
    }
    
    // Check if already in queue
    for (int i = 0; i < emergencyQueueCount; i++) {
        if (strcmp(emergencyQueue[i].medicineId, medicineId) == 0) {
            printf("Medicine already in emergency queue!\n");
            return;
        }
    }
    
    EmergencyMedicine emergencyMed;
    
    strcpy(emergencyMed.medicineId, medicines[medIndex].medicineId);
    strcpy(emergencyMed.name, medicines[medIndex].name);
    strcpy(emergencyMed.type, medicines[medIndex].type);
    emergencyMed.currentStock = medicines[medIndex].quantity;
    
    printf("Enter Urgency Level (critical/high/medium/low): ");
    scanf(" %[^\n]", emergencyMed.priority);
    
    printf("Enter Reason for Priority: ");
    scanf(" %[^\n]", emergencyMed.reason);
    
    printf("Enter Additional Notes: ");
    scanf(" %[^\n]", emergencyMed.notes);
    
    calculateUrgency(&medicines[medIndex], &emergencyMed);
    
    // Override priority based on user input
    if (strcmp(emergencyMed.priority, "critical") == 0) {
        emergencyMed.urgencyScore = 90;
        strcpy(emergencyMed.recommendedAction, "Immediate Order Required");
        strcpy(emergencyMed.emergencyLevel, "Critical");
    } else if (strcmp(emergencyMed.priority, "high") == 0) {
        emergencyMed.urgencyScore = 70;
        strcpy(emergencyMed.recommendedAction, "Order This Week");
        strcpy(emergencyMed.emergencyLevel, "High");
    } else if (strcmp(emergencyMed.priority, "medium") == 0) {
        emergencyMed.urgencyScore = 50;
        strcpy(emergencyMed.recommendedAction, "Plan Refill Soon");
        strcpy(emergencyMed.emergencyLevel, "Medium");
    } else {
        emergencyMed.urgencyScore = 30;
        strcpy(emergencyMed.recommendedAction, "Monitor Stock");
        strcpy(emergencyMed.emergencyLevel, "Normal");
    }
    
    emergencyMed.addedDate = getCurrentDate();
    emergencyMed.manualAddition = 1;
    
    emergencyQueue[emergencyQueueCount++] = emergencyMed;
    printf("Medicine added to emergency queue successfully!\n");
}

void displayEmergencyQueue() {
    printf("\n=== EMERGENCY MEDICINE QUEUE ===\n");
    printf("%-10s %-20s %-15s %-10s %-10s %-10s %-20s\n", 
           "ID", "Name", "Priority", "Stock", "Urgency", "Days", "Action");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < emergencyQueueCount; i++) {
        printf("%-10s %-20s %-15s %-10d %-10d %-10d %-20s\n",
               emergencyQueue[i].medicineId,
               emergencyQueue[i].name,
               emergencyQueue[i].priority,
               emergencyQueue[i].currentStock,
               emergencyQueue[i].urgencyScore,
               emergencyQueue[i].daysUntilEmpty,
               emergencyQueue[i].recommendedAction);
    }
}

void filterEmergencyQueue() {
    char filter[20];
    printf("Enter priority filter (critical/high/medium/low): ");
    scanf(" %[^\n]", filter);
    
    printf("\n=== FILTERED EMERGENCY QUEUE (%s) ===\n", filter);
    printf("%-10s %-20s %-15s %-10s %-10s %-10s\n", 
           "ID", "Name", "Priority", "Stock", "Urgency", "Days");
    printf("------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < emergencyQueueCount; i++) {
        if (strcmp(emergencyQueue[i].priority, filter) == 0) {
            printf("%-10s %-20s %-15s %-10d %-10d %-10d\n",
                   emergencyQueue[i].medicineId,
                   emergencyQueue[i].name,
                   emergencyQueue[i].priority,
                   emergencyQueue[i].currentStock,
                   emergencyQueue[i].urgencyScore,
                   emergencyQueue[i].daysUntilEmpty);
            found++;
        }
    }
    
    if (!found) {
        printf("No medicines found with priority: %s\n", filter);
    }
}

void generateEmergencyReport() {
    printf("\n=== EMERGENCY MEDICINE REPORT ===\n");
    printf("Generated on: %02d/%02d/%04d\n", getCurrentDate().day, getCurrentDate().month, getCurrentDate().year);
    
    int critical = 0, high = 0, medium = 0, low = 0;
    
    for (int i = 0; i < emergencyQueueCount; i++) {
        if (strcmp(emergencyQueue[i].priority, "critical") == 0) critical++;
        else if (strcmp(emergencyQueue[i].priority, "high") == 0) high++;
        else if (strcmp(emergencyQueue[i].priority, "medium") == 0) medium++;
        else low++;
    }
    
    printf("\nSUMMARY:\n");
    printf("Critical Medicines: %d\n", critical);
    printf("High Priority: %d\n", high);
    printf("Medium Priority: %d\n", medium);
    printf("Low Priority: %d\n", low);
    printf("Total in Queue: %d\n", emergencyQueueCount);
    
    printf("\nCRITICAL MEDICINES (Immediate Action Required):\n");
    for (int i = 0; i < emergencyQueueCount; i++) {
        if (strcmp(emergencyQueue[i].priority, "critical") == 0) {
            printf("- %s (Stock: %d, Days until empty: %d)\n", 
                   emergencyQueue[i].name, emergencyQueue[i].currentStock, emergencyQueue[i].daysUntilEmpty);
        }
    }
    
    printf("\nRECOMMENDED ACTIONS:\n");
    if (critical > 0) printf("1. Order all critical medicines immediately\n");
    if (high > 0) printf("2. Place orders for high priority medicines this week\n");
    if (medium > 0) printf("3. Plan refill for medium priority medicines\n");
}

// Analytics Functions
void displayAnalytics() {
    printf("\n=== PHARMACY ANALYTICS ===\n");
    printf("Total Medicines: %d\n", medicineCount);
    printf("Total Patients: %d\n", patientCount);
    printf("Total Bills: %d\n", billCount);
    printf("Emergency Queue Items: %d\n", emergencyQueueCount);
    
    int lowStockCount = 0;
    for (int i = 0; i < medicineCount; i++) {
        if (medicines[i].quantity < medicines[i].minStock) {
            lowStockCount++;
        }
    }
    printf("Low Stock Medicines: %d\n", lowStockCount);
    
    float totalRevenue = 0;
    for (int i = 0; i < billCount; i++) {
        totalRevenue += bills[i].total;
    }
    printf("Total Revenue: %.2f\n", totalRevenue);
    
    // Top selling medicines
    printf("\nTop Selling Medicines:\n");
    int *sales = (int *)calloc(medicineCount, sizeof(int));
    
    for (int i = 0; i < billCount; i++) {
        for (int j = 0; j < bills[i].itemCount; j++) {
            for (int k = 0; k < medicineCount; k++) {
                if (strcmp(medicines[k].medicineId, bills[i].items[j].medicineId) == 0) {
                    sales[k] += bills[i].items[j].quantity;
                    break;
                }
            }
        }
    }
    
    // Find top 5
    for (int rank = 1; rank <= 5 && rank <= medicineCount; rank++) {
        int maxIndex = -1, maxSales = -1;
        for (int i = 0; i < medicineCount; i++) {
            if (sales[i] > maxSales) {
                maxSales = sales[i];
                maxIndex = i;
            }
        }
        
        if (maxIndex != -1) {
            printf("%d. %s - %d units sold\n", rank, medicines[maxIndex].name, sales[maxIndex]);
            sales[maxIndex] = -1; // Mark as processed
        }
    }
    
    free(sales);
}

// Data persistence functions
void saveData() {
    FILE *file;
    
    // Save medicines
    file = fopen("medicines.dat", "wb");
    if (file) {
        fwrite(&medicineCount, sizeof(int), 1, file);
        fwrite(medicines, sizeof(Medicine), medicineCount, file);
        fclose(file);
    }
    
    // Save patients
    file = fopen("patients.dat", "wb");
    if (file) {
        fwrite(&patientCount, sizeof(int), 1, file);
        fwrite(patients, sizeof(Patient), patientCount, file);
        fclose(file);
    }
    
    // Save bills
    file = fopen("bills.dat", "wb");
    if (file) {
        fwrite(&billCount, sizeof(int), 1, file);
        fwrite(bills, sizeof(Bill), billCount, file);
        fclose(file);
    }
    
    // Save emergency queue
    file = fopen("emergency.dat", "wb");
    if (file) {
        fwrite(&emergencyQueueCount, sizeof(int), 1, file);
        fwrite(emergencyQueue, sizeof(EmergencyMedicine), emergencyQueueCount, file);
        fclose(file);
    }
    
    printf("Data saved successfully!\n");
}

void loadData() {
    FILE *file;
    
    // Load medicines
    file = fopen("medicines.dat", "rb");
    if (file) {
        fread(&medicineCount, sizeof(int), 1, file);
        fread(medicines, sizeof(Medicine), medicineCount, file);
        fclose(file);
    }
    
    // Load patients
    file = fopen("patients.dat", "rb");
    if (file) {
        fread(&patientCount, sizeof(int), 1, file);
        fread(patients, sizeof(Patient), patientCount, file);
        fclose(file);
    }
    
    // Load bills
    file = fopen("bills.dat", "rb");
    if (file) {
        fread(&billCount, sizeof(int), 1, file);
        fread(bills, sizeof(Bill), billCount, file);
        fclose(file);
    }
    
    // Load emergency queue
    file = fopen("emergency.dat", "rb");
    if (file) {
        fread(&emergencyQueueCount, sizeof(int), 1, file);
        fread(emergencyQueue, sizeof(EmergencyMedicine), emergencyQueueCount, file);
        fclose(file);
    }
    
    printf("Data loaded successfully!\n");
}

// Main menu functions
void medicineMenu() {
    int choice;
    do {
        printf("\n=== MEDICINE MANAGEMENT ===\n");
        printf("1. Add Medicine\n");
        printf("2. Display All Medicines\n");
        printf("3. Search Medicine\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addMedicine(); break;
            case 2: displayMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

void patientMenu() {
    int choice;
    do {
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1. Add Patient\n");
        printf("2. Display All Patients\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

void billingMenu() {
    int choice;
    do {
        printf("\n=== BILLING ===\n");
        printf("1. Create New Bill\n");
        printf("2. Display Bills\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: createBill(); break;
            case 2: 
                printf("\n=== BILL HISTORY ===\n");
                for (int i = 0; i < billCount; i++) {
                    printf("Bill ID: %s, Patient: %s, Date: %02d/%02d/%04d, Total: %.2f\n",
                           bills[i].billId, bills[i].patientId, 
                           bills[i].date.day, bills[i].date.month, bills[i].date.year,
                           bills[i].total);
                }
                break;
            case 3: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

void emergencyMenu() {
    int choice;
    do {
        printf("\n=== EMERGENCY MEDICINE MODULE ===\n");
        printf("1. Update Emergency Queue\n");
        printf("2. Display Emergency Queue\n");
        printf("3. Add Medicine to Emergency Queue\n");
        printf("4. Filter by Priority\n");
        printf("5. Generate Emergency Report\n");
        printf("6. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: updateEmergencyQueue(); break;
            case 2: displayEmergencyQueue(); break;
            case 3: addMedicineToEmergencyQueue(); break;
            case 4: filterEmergencyQueue(); break;
            case 5: generateEmergencyReport(); break;
            case 6: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

int main() {
    loadData();
    
    int choice;
    do {
        printf("\n=== PHARMACY MANAGEMENT SYSTEM ===\n");
        printf("1. Medicine Management\n");
        printf("2. Patient Management\n");
        printf("3. Billing\n");
        printf("4. Emergency Medicine Module\n");
        printf("5. Analytics\n");
        printf("6. Save Data\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: medicineMenu(); break;
            case 2: patientMenu(); break;
            case 3: billingMenu(); break;
            case 4: emergencyMenu(); break;
            case 5: displayAnalytics(); break;
            case 6: saveData(); break;
            case 7: 
                saveData();
                printf("Thank you for using Pharmacy Management System!\n");
                break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);
    
    return 0;
}
